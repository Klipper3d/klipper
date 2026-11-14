<a id="beaglebone"></a>
# Beaglebone

[English](../Beaglebone.md) | **Português brasileiro**

Este documento descreve o processo para executar o Klipper em um Beaglebone PRU.

<a id="building-an-os-image"></a>
## Construindo uma imagem de sistema operacional

Comece instalando a imagem [Debian 11.7 2023-09-02 4GB microSD IoT](https://beagleboard.org/latest-images). Pode-se executar a imagem a partir de um cartão micro-SD ou da eMMC integrada. Se estiver usando a eMMC, instale-a na eMMC agora seguindo as instruções do link acima.

Em seguida, acesse a máquina Beaglebone via ssh (`ssh debian@beaglebone` -- a senha é `temppwd`).

Antes de começar a instalar o Klipper, você precisa liberar espaço adicional. Há 3 opções para fazer isso:

1. remover alguns recursos de "Demonstração" da BeagleBone
1. se você inicializou pelo cartão SD, e ele é maior que 4Gb - você pode expandir o sistema de arquivos atual para ocupar todo o espaço do cartão
1. executar a opção nº 1 e a nº 2 juntas.

Para remover alguns recursos "Demo" da BeagleBone, execute estes comandos

```
sudo apt remove bb-node-red-installer
sudo apt remove bb-code-server
```

Para expandir o sistema de arquivos para o tamanho total do seu cartão SD, execute este comando; não é necessário reiniciar.

```
sudo growpart /dev/mmcblk0 1
sudo resize2fs /dev/mmcblk0p1
```


Instale o Klipper executando os seguintes comandos:

```
git clone https://github.com/Klipper3d/klipper.git
./klipper/scripts/install-beaglebone.sh
```

Após instalar o Klipper, você precisa decidir que tipo de implantação necessita, mas observe que a BeagleBone é um hardware baseado em 3.3v e, na maioria dos casos, você não pode conectar pinos diretamente a hardware baseado em 5v ou 12v sem placas de conversão.

Como o Klipper tem arquitetura multimódulo na BeagleBone, você pode atender a muitos casos de uso diferentes, mas os gerais são os seguintes:

Caso de uso 1: Use a BeagleBone apenas como sistema hospedeiro para executar o Klipper e software adicional como OctoPrint/Fluidd + Moonraker/... e essa configuração controlará microcontroladores externos via conexões serial/usb/canbus.

Caso de uso 2: Use a BeagleBone com placa de expansão (cape) como a placa CRAMPS. Nessa configuração, a BeagleBone hospedará o Klipper + software adicional, e controlará a placa de expansão com os núcleos PRU da BeagleBone (2 núcleos adicionais de 200Mh, 32Bit).

Caso de uso 3: É o mesmo que o "Caso de uso 1", mas adicionalmente você deseja controlar os GPIOs da BeagleBone em alta velocidade utilizando os núcleos PRU para descarregar a CPU principal.

<a id="installing-octoprint"></a>
## Instalação do Octoprint

Pode-se então instalar o Octoprint ou ignorar totalmente esta seção se desejar outro software:

```
git clone https://github.com/foosel/OctoPrint.git
cd OctoPrint/
virtualenv venv
./venv/bin/python setup.py install
```

E configure o OctoPrint para começar na inicialização:

```
sudo cp ~/OctoPrint/scripts/octoprint.init /etc/init.d/octoprint
sudo chmod +x /etc/init.d/octoprint
sudo cp ~/OctoPrint/scripts/octoprint.default /etc/default/octoprint
sudo update-rc.d octoprint defaults
```

É necessário modificar o arquivo de configuração **/etc/default/octoprint** do OctoPrint. É preciso alterar o usuário `OCTOPRINT_USER` para `debian`, alterar `NICELEVEL` para `0`, descomentar as configurações `BASEDIR`, `CONFIGFILE` e `DAEMON` e alterar as referências de `/home/pi/` para `/home/debian/`:

```
sudo nano /etc/default/octoprint
```

Em seguida, inicie o serviço Octoprint:

```
sudo systemctl start octoprint
```
Aguarde 1-2 minutos e certifique-se de que o servidor web do OctoPrint esteja acessível - ele deve estar em: <http://beaglebone:5000/>

<a id="building-the-beaglebone-pru-micro-controller-code-pru-firmware"></a>
## Compilação do código do microcontrolador PRU da BeagleBone (firmware da PRU)

Esta seção é obrigatória para o "Caso de uso 2" e o "Caso de uso 3" mencionados acima; você deve ignorá-la para o "Caso de uso 1".

Verifique se os dispositivos necessários estão presentes

```
sudo beagle-version
```
Você deve verificar se a saída contém o carregamento bem-sucedido dos drivers "remoteproc" e a presença dos núcleos PRU; no Kernel 5.10 eles devem ser "remoteproc1" e "remoteproc2" (4a334000.pru, 4a338000.pru). Verifique também se muitos GPIOs estão carregados; eles terão o aspecto "Allocated GPIO id=0 name='P8_03'". Normalmente está tudo certo e nenhuma configuração de hardware é necessária. Se algo estiver faltando - tente alterar as opções "uboot overlays" ou os cape-overlays. Apenas para referência, segue parte da saída de uma configuração funcional de BeagleBone Black com placa CRAMPS:

```
model:[TI_AM335x_BeagleBone_Black]
UBOOT: Booted Device-Tree:[am335x-boneblack-uboot-univ.dts]
UBOOT: Loaded Overlay:[BB-ADC-00A0.bb.org-overlays]
UBOOT: Loaded Overlay:[BB-BONE-eMMC1-01-00A0.bb.org-overlays]
kernel:[5.10.168-ti-r71]
/boot/uEnv.txt Settings:
uboot_overlay_options:[enable_uboot_overlays=1]
uboot_overlay_options:[disable_uboot_overlay_video=0]
uboot_overlay_options:[disable_uboot_overlay_audio=1]
uboot_overlay_options:[disable_uboot_overlay_wireless=1]
uboot_overlay_options:[enable_uboot_cape_universal=1]
pkg:[bb-cape-overlays]:[4.14.20210821.0-0~bullseye+20210821]
pkg:[bb-customizations]:[1.20230720.1-0~bullseye+20230720]
pkg:[bb-usb-gadgets]:[1.20230414.0-0~bullseye+20230414]
pkg:[bb-wl18xx-firmware]:[1.20230414.0-0~bullseye+20230414]
.............
.............

```

Para compilar o código de microcontrolador do Klipper, comece configurando-o para "Beaglebone PRU"; para "BeagleBone Black", desabilite adicionalmente as opções "Support GPIO Bit-banging devices" e desabilite "Support LCD devices" dentro de "Optional features", pois elas não caberão na memória de firmware PRU de 8Kb; depois saia e salve a configuração:

```
cd ~/klipper/
make menuconfig
```

Para compilar e instalar o novo código de microcontrolador PRU, execute:

```
sudo service klipper stop
make flash
sudo service klipper start
```
Após a execução dos comandos anteriores, seu firmware PRU deve estar pronto e iniciado. Para verificar se está tudo certo, você pode executar o seguinte comando

```
dmesg
```
e compare as últimas mensagens com este exemplo, que indica que tudo iniciou corretamente:

```
[   71.105499] remoteproc remoteproc1: 4a334000.pru is available
[   71.157155] remoteproc remoteproc2: 4a338000.pru is available
[   73.256287] remoteproc remoteproc1: powering up 4a334000.pru
[   73.279246] remoteproc remoteproc1: Booting fw image am335x-pru0-fw, size 97112
[   73.285807]  remoteproc1#vdev0buffer: registered virtio0 (type 7)
[   73.285836] remoteproc remoteproc1: remote processor 4a334000.pru is now up
[   73.286322] remoteproc remoteproc2: powering up 4a338000.pru
[   73.313717] remoteproc remoteproc2: Booting fw image am335x-pru1-fw, size 188560
[   73.313753] remoteproc remoteproc2: header-less resource table
[   73.329964] remoteproc remoteproc2: header-less resource table
[   73.348321] remoteproc remoteproc2: remote processor 4a338000.pru is now up
[   73.443355] virtio_rpmsg_bus virtio0: creating channel rpmsg-pru addr 0x1e
[   73.443727] virtio_rpmsg_bus virtio0: msg received with no recipient
[   73.444352] virtio_rpmsg_bus virtio0: rpmsg host is online
[   73.540993] rpmsg_pru virtio0.rpmsg-pru.-1.30: new rpmsg_pru device: /dev/rpmsg_pru30
```
observe "/dev/rpmsg_pru30" - é seu futuro dispositivo serial para a configuração da mcu principal. É necessário que este dispositivo esteja presente; se estiver ausente - seus núcleos PRU não iniciaram corretamente.

<a id="building-and-installing-linux-host-micro-controller-code"></a>
## Compilação e instalação do código do microcontrolador hospedeiro Linux

Esta seção é obrigatória para o "Caso de uso 2" e opcional para o "Caso de uso 3" mencionados acima

Também é necessário compilar e instalar o código do micro-controlador para um processo de host Linux. Configure-o uma segunda vez para um "processo Linux":

```
make menuconfig
```

Em seguida, instale este código de micro-controlador também:

```
sudo service klipper stop
make flash
sudo service klipper start
```
observe "/tmp/klipper_host_mcu" - será seu futuro dispositivo serial para "mcu host". Se esse arquivo não existir - consulte o arquivo "scripts/klipper-mcu.service", ele foi instalado pelos comandos anteriores e é responsável por ele.

Observe, para o "Caso de uso 2", o seguinte: quando você definir a configuração da impressora, deve sempre usar os sensores de temperatura da "mcu host", pois ADCs não estão presentes na "mcu" padrão (núcleos PRU). Exemplos de configuração de "sensor_pin" para a extrusora e para a mesa aquecida estão disponíveis em "generic-cramps.cfg". Você pode usar qualquer outro GPIO diretamente da "mcu host" referenciando-os desta forma "host:gpiochip1/gpio17", mas isso deve ser evitado, pois criará carga adicional na CPU principal e muito provavelmente você não poderá usá-los para controle de motores de passo.

<a id="remaining-configuration"></a>
## Configuração restante

Conclua a instalação configurando o Klipper seguindo as instruções no documento principal [Instalação](Installation.md#configuring-klipper).

<a id="printing-on-the-beaglebone"></a>
## Impressão no Beaglebone

Infelizmente, o processador da Beaglebone às vezes pode ter dificuldade para executar bem o OctoPrint. Travamentos de impressão são conhecidos por ocorrer em impressões complexas (a impressora pode se mover mais rápido do que o OctoPrint consegue enviar comandos de movimento). Se isso ocorrer, considere usar o recurso "virtual_sdcard" (consulte [Referência de configuração](Config_Reference.md#virtual_sdcard) para detalhes) para imprimir diretamente do Klipper e desabilite quaisquer opções de registro DEBUG ou VERBOSE, caso as tenha habilitado.

<a id="avr-micro-controller-code-build"></a>
## Compilação do código do microcontrolador AVR

Este ambiente tem tudo para compilar o código necessário do microcontrolador, exceto AVR. Os pacotes AVR foram removidos por causa de conflito com os pacotes PRU. Se você ainda quiser compilar o código do microcontrolador AVR neste ambiente, precisa remover os pacotes PRU e instalar os pacotes AVR executando os seguintes comandos

```
sudo apt-get remove gcc-pru
sudo apt-get install avrdude gcc-avr binutils-avr avr-libc
```
se você precisar restaurar os pacotes PRU - remova os pacotes ARV antes disso

```
sudo apt-get remove avrdude gcc-avr binutils-avr avr-libc
sudo apt-get install gcc-pru
```


<a id="hardware-pin-designation"></a>
## Designação de pinos de hardware

A BeagleBone é muito flexível em termos de designação de pinos, o mesmo pino pode ser configurado para funções diferentes, mas sempre uma única função por pino, e a mesma função pode estar presente em pinos diferentes. Portanto você não pode ter múltiplas funções em um único pino nem ter a mesma função em múltiplos pinos. Exemplo: P9_20 - i2c2_sda/can0_tx/spi1_cs0/gpio0_12/uart1_ctsn P9_19 - i2c2_scl/can0_rx/spi1_cs1/gpio0_13/uart1_rtsn P9_24 - i2c1_scl/can1_rx/gpio0_15/uart1_tx P9_26 - i2c1_sda/can1_tx/gpio0_14/uart1_rx

A designação de pinos é definida usando "overlays" especiais que serão carregados durante a inicialização do Linux. Eles são configurados editando o arquivo /boot/uEnv.txt com permissões elevadas

```
sudo editor /boot/uEnv.txt
```
e definindo qual funcionalidade carregar; por exemplo, para habilitar CAN1 você precisa definir o overlay correspondente

```
uboot_overlay_addr4=/lib/firmware/BB-CAN1-00A0.dtbo
```
Este overlay BB-CAN1-00A0.dtbo reconfigurará todos os pinos necessários para CAN1 e criará o dispositivo CAN no Linux. Qualquer alteração nos overlays exigirá reinicialização do sistema para ser aplicada. Se você precisar entender quais pinos estão envolvidos em algum overlay - você pode analisar os arquivos-fonte neste local: /opt/sources/bb.org-overlays/src/arm/ ou pesquisar informações nos fóruns da BeagleBone.

<a id="enabling-hardware-spi"></a>
## Habilitação do SPI de hardware

A BeagleBone geralmente tem múltiplos barramentos SPI de hardware, por exemplo a BeagleBone Black pode ter 2 deles, eles podem operar até 48Mhz, mas geralmente são limitados a 16Mhz pelo Device-tree do Kernel. Por padrão, na BeagleBone Black alguns dos pinos SPI1 são configurados para saída de áudio HDMI, para habilitar totalmente o SPI1 de 4 fios você precisa desabilitar o áudio HDMI e habilitar o SPI1. Para fazer isso edite o arquivo /boot/uEnv.txt com permissões elevadas

```
sudo editor /boot/uEnv.txt
```
descomente a variável

```
disable_uboot_overlay_audio=1
```

em seguida, descomente a variável e defina-a desta forma

```
uboot_overlay_addr4=/lib/firmware/BB-SPIDEV1-00A0.dtbo
```
Salve as alterações em /boot/uEnv.txt e reinicie a placa. Agora você tem SPI1 habilitado. Para verificar sua presença, execute o comando

```
ls /dev/spidev1.*
```
Observe que a BeagleBone geralmente é um hardware baseado em 3.3v e, para usar dispositivos SPI de 5V, você precisa adicionar um chip de conversão de nível (Level-Shifting), por exemplo SN74CBTD3861, SN74LVC1G34 ou similar. Se você estiver usando a placa CRAMPS - ela já contém chip de conversão de nível e os pinos SPI1 ficarão disponíveis na porta P503, e eles podem aceitar hardware de 5v; consulte os esquemas da placa CRAMPS para referências de pinos.

<a id="enabling-hardware-i2c"></a>
## Habilitação do I2C de hardware

A BeagleBone geralmente tem múltiplos barramentos I2C de hardware, por exemplo a BeagleBone Black pode ter 3 deles, eles suportam velocidade de até 400Kbit no modo rápido. Por padrão, na BeagleBone Black há dois deles (i2c-1 e i2c-2), geralmente ambos já configurados e presentes na P9, o terceiro ic2-0 geralmente é reservado para uso interno. Se você estiver usando a placa CRAMPS, então o i2c-2 está presente na porta P303 com nível de 3.3v. Se você quiser obter o I2c-1 na placa CRAMPS - você pode obtê-lo nos pinos Extruder1.Step, Extruder1.Dir, eles também são de 3.3v, verifique os esquemáticos da placa CRAMPS para referências de pinos. Overlays relacionados, para [Designação de pinos de hardware](#hardware-pin-designation): I2C1(100Kbit): BB-I2C1-00A0.dtbo I2C1(400Kbit): BB-I2C1-FAST-00A0.dtbo I2C2(100Kbit): BB-I2C2-00A0.dtbo I2C2(400Kbit): BB-I2C2-FAST-00A0.dtbo

<a id="enabling-hardware-uartserialcan"></a>
## Habilitação de UART(serial)/CAN de hardware

A BeagleBone tem até 6 barramentos UART(serial) de hardware (até 3Mbit) e até 2 barramentos CAN de hardware (1Mbit). UART1(RX,TX) e CAN1(TX,RX) e I2C2(SDA,SCL) usam os mesmos pinos - portanto você precisa escolher o que usar. UART1(CTSN,RTSN) e CAN0(TX,RX) e I2C1(SDA,SCL) usam os mesmos pinos - portanto você precisa escolher o que usar. Todos os pinos relacionados a UART/CAN são de 3.3v, portanto você precisará usar chips/placas transceptoras como SN74LVC2G241DCUR (para UART), SN65HVD230 (para CAN), TTL-RS485 (para RS-485) ou algo semelhante que possa converter sinais de 3.3v para níveis apropriados.

Overlays relacionados, para [Designação dos pinos de hardware](#hardware-pin-designation) CAN0: BB-CAN0-00A0.dtbo CAN1: BB-CAN1-00A0.dtbo UART0: - usado para Console UART1(RX,TX): BB-UART1-00A0.dtbo UART1(RTS,CTS): BB-UART1-RTSCTS-00A0.dtbo UART2(RX,TX): BB-UART2-00A0.dtbo UART3(RX,TX): BB-UART3-00A0.dtbo UART4(RS-485): BB-UART4-RS485-00A0.dtbo UART5(RX,TX): BB-UART5-00A0.dtbo
