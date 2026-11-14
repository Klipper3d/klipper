<a id="bootloaders"></a>
# Bootloaders

[English](../Bootloaders.md) | **Português brasileiro**

Este documento fornece informações sobre bootloaders comuns encontrados em microcontroladores suportados pelo Klipper.

O bootloader é um software de terceiros executado no microcontrolador quando ele é ligado pela primeira vez. Ele é tipicamente usado para gravar um novo aplicativo (por exemplo, Klipper) no microcontrolador sem exigir hardware especializado. Infelizmente, não há um padrão amplo da indústria para gravação em microcontroladores, nem há um bootloader padrão que funcione em todos os microcontroladores. Pior ainda, é comum que cada bootloader exija um conjunto diferente de etapas para gravar um aplicativo.

Se for possível gravar um bootloader em um microcontrolador, geralmente também é possível usar esse mecanismo para gravar um aplicativo, mas deve-se ter cuidado ao fazer isso, pois pode-se remover inadvertidamente o bootloader. Em contraste, um bootloader geralmente permitirá ao usuário apenas gravar um aplicativo. Portanto, recomenda-se usar um bootloader para gravar um aplicativo sempre que possível.

Este documento tenta descrever bootloaders comuns, as etapas necessárias para gravar um bootloader e as etapas necessárias para gravar um aplicativo. Este documento não é uma referência oficial; ele destina-se como uma coletânea de informações úteis que os desenvolvedores do Klipper acumularam.

<a id="avr-micro-controllers"></a>
## Microcontroladores AVR

Em geral, o projeto Arduino é uma boa referência para bootloaders e procedimentos de gravação nos microcontroladores Atmel Atmega de 8 bits. Em particular, o arquivo "boards.txt": <https://github.com/arduino/Arduino/blob/1.8.5/hardware/arduino/avr/boards.txt> é uma referência útil.

Para gravar o próprio bootloader, os chips AVR exigem uma ferramenta externa de gravação de hardware (que se comunica com o chip usando SPI). Essa ferramenta pode ser adquirida (por exemplo, pesquise na web por "avr isp", "arduino isp" ou "usb tiny isp"). Também é possível usar outro Arduino ou Raspberry Pi para gravar um bootloader AVR (por exemplo, pesquise na web por "program an avr using raspberry pi"). Os exemplos abaixo presumem que um dispositivo do tipo "AVR ISP Mk2" esteja em uso.

O programa "avrdude" é a ferramenta mais comum usada para gravar chips atmega (tanto gravação de bootloader quanto gravação de aplicativo).

<a id="atmega2560"></a>
### Atmega2560

Este chip é normalmente encontrado no "Arduino Mega" e é muito comum em placas de impressora 3D.

Para gravar o próprio bootloader use algo como:

```
wget 'https://github.com/arduino/Arduino/raw/1.8.5/hardware/arduino/avr/bootloaders/stk500v2/stk500boot_v2_mega2560.hex'

avrdude -cavrispv2 -patmega2560 -P/dev/ttyACM0 -b115200 -e -u -U lock:w:0x3F:m -U efuse:w:0xFD:m -U hfuse:w:0xD8:m -U lfuse:w:0xFF:m
avrdude -cavrispv2 -patmega2560 -P/dev/ttyACM0 -b115200 -U flash:w:stk500boot_v2_mega2560.hex
avrdude -cavrispv2 -patmega2560 -P/dev/ttyACM0 -b115200 -U lock:w:0x0F:m
```

Para gravar um aplicativo use algo como:

```
avrdude -cwiring -patmega2560 -P/dev/ttyACM0 -b115200 -D -Uflash:w:out/klipper.elf.hex:i
```

<a id="atmega1280"></a>
### Atmega1280

Este chip é normalmente encontrado em versões anteriores do "Arduino Mega".

Para gravar o próprio bootloader use algo como:

```
wget 'https://github.com/arduino/Arduino/raw/1.8.5/hardware/arduino/avr/bootloaders/atmega/ATmegaBOOT_168_atmega1280.hex'

avrdude -cavrispv2 -patmega1280 -P/dev/ttyACM0 -b115200 -e -u -U lock:w:0x3F:m -U efuse:w:0xF5:m -U hfuse:w:0xDA:m -U lfuse:w:0xFF:m
avrdude -cavrispv2 -patmega1280 -P/dev/ttyACM0 -b115200 -U flash:w:ATmegaBOOT_168_atmega1280.hex
avrdude -cavrispv2 -patmega1280 -P/dev/ttyACM0 -b115200 -U lock:w:0x0F:m
```

Para gravar um aplicativo use algo como:

```
avrdude -carduino -patmega1280 -P/dev/ttyACM0 -b57600 -D -Uflash:w:out/klipper.elf.hex:i
```

<a id="atmega1284p"></a>
### Atmega1284p

Este chip é comumente encontrado em placas de impressora 3D estilo "Melzi".

Para gravar o próprio bootloader use algo como:

```
wget 'https://github.com/Lauszus/Sanguino/raw/1.0.2/bootloaders/optiboot/optiboot_atmega1284p.hex'

avrdude -cavrispv2 -patmega1284p -P/dev/ttyACM0 -b115200 -e -u -U lock:w:0x3F:m -U efuse:w:0xFD:m -U hfuse:w:0xDE:m -U lfuse:w:0xFF:m
avrdude -cavrispv2 -patmega1284p -P/dev/ttyACM0 -b115200 -U flash:w:optiboot_atmega1284p.hex
avrdude -cavrispv2 -patmega1284p -P/dev/ttyACM0 -b115200 -U lock:w:0x0F:m
```

Para gravar um aplicativo use algo como:

```
avrdude -carduino -patmega1284p -P/dev/ttyACM0 -b115200 -D -Uflash:w:out/klipper.elf.hex:i
```

Observe que várias placas estilo "Melzi" vêm pré-carregadas com um bootloader que usa taxa de transmissão de 57600. Neste caso, para gravar um aplicativo use algo como isto:

```
avrdude -carduino -patmega1284p -P/dev/ttyACM0 -b57600 -D -Uflash:w:out/klipper.elf.hex:i
```

<a id="at90usb1286"></a>
### At90usb1286

Este documento não aborda o método para gravar um bootloader no At90usb1286 nem aborda a gravação geral de aplicativos neste dispositivo.

O dispositivo Teensy++ da pjrc.com vem com um bootloader proprietário. Ele exige uma ferramenta de gravação personalizada de <https://github.com/PaulStoffregen/teensy_loader_cli>. Pode-se gravar uma aplicação com ela usando algo como:

```
teensy_loader_cli --mcu=at90usb1286 out/klipper.elf.hex -v
```

<a id="atmega168"></a>
### Atmega168

O atmega168 tem espaço limitado de flash. Se usar um bootloader, recomenda-se usar o bootloader Optiboot. Para gravar esse bootloader use algo como:

```
wget 'https://github.com/arduino/Arduino/raw/1.8.5/hardware/arduino/avr/bootloaders/optiboot/optiboot_atmega168.hex'

avrdude -cavrispv2 -patmega168 -P/dev/ttyACM0 -b115200 -e -u -U lock:w:0x3F:m -U efuse:w:0x04:m -U hfuse:w:0xDD:m -U lfuse:w:0xFF:m
avrdude -cavrispv2 -patmega168 -P/dev/ttyACM0 -b115200 -U flash:w:optiboot_atmega168.hex
avrdude -cavrispv2 -patmega168 -P/dev/ttyACM0 -b115200 -U lock:w:0x0F:m
```

Para gravar um aplicativo via bootloader Optiboot use algo como:

```
avrdude -carduino -patmega168 -P/dev/ttyACM0 -b115200 -D -Uflash:w:out/klipper.elf.hex:i
```

<a id="sam3-micro-controllers-arduino-due"></a>
## Microcontroladores SAM3 (Arduino Due)

Não é comum usar um bootloader com o mcu SAM3. O próprio chip tem uma ROM que permite que a flash seja programada pela porta serial de 3,3V ou por USB.

Para habilitar a ROM, o pino "erase" é mantido em nível alto durante um reset, o que apaga o conteúdo da flash e faz a ROM executar. Em um Arduino Due, essa sequência pode ser realizada configurando uma taxa de transmissão de 1200 na "programming usb port" (a porta USB mais próxima da fonte de alimentação).

O código em <https://github.com/shumatech/BOSSA> pode ser usado para programar o SAM3. Recomenda-se usar a versão 1.9 ou posterior.

Para gravar um aplicativo use algo como:

```
bossac -U -p /dev/ttyACM0 -a -e -w out/klipper.bin -v -b
bossac -U -p /dev/ttyACM0 -R
```

<a id="sam4-micro-controllers-duet-wifi"></a>
## Microcontroladores SAM4 (Duet Wifi)

Não é comum usar um bootloader com o mcu SAM4. O próprio chip tem uma ROM que permite que a flash seja programada pela porta serial de 3,3V ou por USB.

Para habilitar a ROM, o pino "erase" é mantido em nível alto durante um reset, o que apaga o conteúdo da flash e faz a ROM executar.

O código em <https://github.com/shumatech/BOSSA> pode ser usado para programar o SAM4. É necessário usar a versão `1.8.0` ou superior.

Para gravar um aplicativo use algo como:

```
bossac --port=/dev/ttyACM0 -b -U -e -w -v -R out/klipper.bin
```

<a id="samdc21-micro-controllers-duet3d-toolboard-1lc"></a>
## Microcontroladores SAMDC21 (Duet3D Toolboard 1LC)

O SAMC21 é gravado via interface ARM Serial Wire Debug (SWD). Isso é comumente feito com um dongle de hardware SWD dedicado. Como alternativa, pode-se usar um [Raspberry Pi com OpenOCD](#running-openocd-on-the-raspberry-pi).

Ao usar OpenOCD com o SAMC21, etapas extras devem ser tomadas para primeiro colocar o chip em modo Cold Plugging se a placa usar os pinos SWD para outros fins. Se usar OpenOCD em um Raspberry Pi, isso pode ser feito executando os seguintes comandos antes de invocar o OpenOCD.

```
SWCLK=25
SWDIO=24
SRST=18

echo "Exporting SWCLK and SRST pins."
echo $SWCLK > /sys/class/gpio/export
echo $SRST > /sys/class/gpio/export
echo "out" > /sys/class/gpio/gpio$SWCLK/direction
echo "out" > /sys/class/gpio/gpio$SRST/direction

echo "Setting SWCLK low and pulsing SRST."
echo "0" > /sys/class/gpio/gpio$SWCLK/value
echo "0" > /sys/class/gpio/gpio$SRST/value
echo "1" > /sys/class/gpio/gpio$SRST/value

echo "Unexporting SWCLK and SRST pins."
echo $SWCLK > /sys/class/gpio/unexport
echo $SRST > /sys/class/gpio/unexport
```

Para gravar um programa com OpenOCD use a seguinte configuração de chip:

```
source [find target/at91samdXX.cfg]
```
Obtenha um programa; por exemplo, o klipper pode ser compilado para este chip. Grave com comandos OpenOCD semelhantes a:

```
at91samd chip-erase
at91samd bootloader 0
program out/klipper.elf verify
```

<a id="samd21-micro-controllers-arduino-zero"></a>
## Microcontroladores SAMD21 (Arduino Zero)

O bootloader do SAMD21 é gravado via interface ARM Serial Wire Debug (SWD). Isso é comumente feito com um dongle de hardware SWD dedicado. Como alternativa, pode-se usar um [Raspberry Pi com OpenOCD](#running-openocd-on-the-raspberry-pi).

Para gravar um bootloader com OpenOCD use a seguinte configuração de chip:

```
source [find target/at91samdXX.cfg]
```
Obtenha um bootloader - por exemplo:

```
wget 'https://github.com/arduino/ArduinoCore-samd/raw/1.8.3/bootloaders/zero/samd21_sam_ba.bin'
```
Grave com comandos OpenOCD semelhantes a:

```
at91samd bootloader 0
program samd21_sam_ba.bin verify
```

O bootloader mais comum no SAMD21 é o encontrado no "Arduino Zero". Ele usa um bootloader de 8KiB (a aplicação deve ser compilada com endereço inicial de 8KiB). Pode-se entrar neste bootloader clicando duas vezes no botão de reset. Para gravar uma aplicação, use algo como:

```
bossac -U -p /dev/ttyACM0 --offset=0x2000 -w out/klipper.bin -v -b -R
```

Em contraste, o "Arduino M0" usa um bootloader de 16KiB (a aplicação deve ser compilada com endereço inicial de 16KiB). Para gravar uma aplicação neste bootloader, reinicie o microcontrolador e execute o comando de gravação nos primeiros segundos após a inicialização - algo como:

```
avrdude -c stk500v2 -p atmega2560 -P /dev/ttyACM0 -u -Uflash:w:out/klipper.elf.hex:i
```

<a id="samd51-micro-controllers-adafruit-metro-m4-and-similar"></a>
## Microcontroladores SAMD51 (Adafruit Metro-M4 e semelhantes)

Como o SAMD21, o bootloader do SAMD51 é gravado via interface ARM Serial Wire Debug (SWD). Para gravar um bootloader com [OpenOCD em um Raspberry Pi](#running-openocd-on-the-raspberry-pi) use a seguinte configuração de chip:

```
source [find target/atsame5x.cfg]
```
Obtenha um bootloader - vários bootloaders estão disponíveis em <https://github.com/adafruit/uf2-samdx1/releases/latest>. Por exemplo:

```
wget 'https://github.com/adafruit/uf2-samdx1/releases/download/v3.7.0/bootloader-itsybitsy_m4-v3.7.0.bin'
```
Grave com comandos OpenOCD semelhantes a:

```
at91samd bootloader 0
program bootloader-itsybitsy_m4-v3.7.0.bin verify
at91samd bootloader 16384
```

O SAMD51 usa um bootloader de 16KiB (a aplicação deve ser compilada com endereço inicial de 16KiB). Para gravar uma aplicação, use algo como:

```
bossac -U -p /dev/ttyACM0 --offset=0x4000 -w out/klipper.bin -v -b -R
```

<a id="stm32f103-micro-controllers-blue-pill-devices"></a>
## Microcontroladores STM32F103 (dispositivos Blue Pill)

Os dispositivos STM32F103 têm uma ROM que pode gravar um bootloader ou aplicação via serial de 3,3V. Normalmente seria ligar os pinos PA10 (Rx do MCU) e PA9 (Tx do MCU) a um adaptador UART de 3,3V. Para acessar a ROM, deve-se conectar o pino "boot 0" em nível alto e o pino "boot 1" em nível baixo, e então reiniciar o dispositivo. O pacote "stm32flash" pode então ser usado para gravar o dispositivo usando algo como:

```
stm32flash -w out/klipper.bin -v -g 0 /dev/ttyAMA0
```

Observe que se alguém estiver usando um Raspberry Pi para a serial de 3,3V, o protocolo stm32flash usa um modo de paridade serial que a "mini UART" do Raspberry Pi não suporta. Consulte <https://www.raspberrypi.com/documentation/computers/configuration.html#configuring-uarts> para detalhes sobre como habilitar a uart completa nos pinos GPIO do Raspberry Pi.

Após a gravação, retorne tanto "boot 0" quanto "boot 1" para nível baixo para que futuros resets inicializem a partir da flash.

<a id="stm32f103-with-stm32duino-bootloader"></a>
### STM32F103 com bootloader stm32duino

O projeto "stm32duino" tem um bootloader capaz de USB - veja: <https://github.com/rogerclarkmelbourne/STM32duino-bootloader>

Este bootloader pode ser gravado via serial 3.3V com algo como:

```
wget 'https://github.com/rogerclarkmelbourne/STM32duino-bootloader/raw/master/binaries/generic_boot20_pc13.bin'

stm32flash -w generic_boot20_pc13.bin -v -g 0 /dev/ttyAMA0
```

Este bootloader usa 8KiB de espaço de flash (o aplicativo deve ser compilado com endereço inicial de 8KiB). Grave um aplicativo com algo como:

```
dfu-util -d 1eaf:0003 -a 2 -R -D out/klipper.bin
```

O bootloader normalmente executa apenas por um curto período após a inicialização. Pode ser necessário cronometrar o comando acima para que execute enquanto o bootloader ainda estiver ativo (o bootloader piscará um led da placa enquanto estiver em execução). Como alternativa, configure o pino "boot 0" para nível baixo e o pino "boot 1" para nível alto para permanecer no bootloader após um reset.

<a id="stm32f103-with-hid-bootloader"></a>
### STM32F103 com bootloader HID

O [bootloader HID](https://github.com/Serasidis/STM32_HID_Bootloader) é um bootloader compacto, sem necessidade de driver, capaz de gravar via USB. Também está disponível um [fork com builds específicos para a SKR Mini E3 1.2](https://github.com/Arksine/STM32_HID_Bootloader/releases/latest).

Para placas STM32F103 genéricas, como a blue pill, é possível gravar o bootloader via serial 3.3V usando stm32flash conforme observado na seção stm32duino acima, substituindo o nome do arquivo pelo binário do bootloader HID desejado (ou seja: hid_generic_pc13.bin para a blue pill).

Não é possível usar stm32flash para a SKR Mini E3 pois o pino boot0 está ligado diretamente ao terra e não está disponível nos pinos de cabeçalho. Recomenda-se usar um STLink V2 com STM32CubeProgrammer para gravar o bootloader. Se você não tiver acesso a um STLink também é possível usar um [Raspberry Pi e OpenOCD](#running-openocd-on-the-raspberry-pi) com a seguinte configuração de chip:

```
source [find target/stm32f1x.cfg]
```
Se desejar, você pode fazer um backup da flash atual com o seguinte comando. Observe que pode levar algum tempo para concluir:

```
flash read_bank 0 btt_skr_mini_e3_backup.bin
```
por fim, você pode gravar com comandos semelhantes a:

```
stm32f1x mass_erase 0
program hid_btt_skr_mini_e3.bin verify 0x08000000
```
OBSERVAÇÕES:

- O exemplo acima apaga o chip e então programa o bootloader. Independentemente do método escolhido para gravação, recomenda-se apagar o chip antes de gravar.
- Antes de gravar a SKR Mini E3 com este bootloader, esteja ciente de que você não poderá mais atualizar o firmware via cartão SD.
- Pode ser necessário manter pressionado o botão de reset da placa ao iniciar o OpenOCD. Ele deve exibir algo como:
  ```
  Open On-Chip Debugger 0.10.0+dev-01204-gc60252ac-dirty (2020-04-27-16:00)
  Licensed under GNU GPL v2
  For bug reports, read
          http://openocd.org/doc/doxygen/bugs.html
  DEPRECATED! use 'adapter speed' not 'adapter_khz'
  Info : BCM2835 GPIO JTAG/SWD bitbang driver
  Info : JTAG and SWD modes enabled
  Info : clock speed 40 kHz
  Info : SWD DPIDR 0x1ba01477
  Info : stm32f1x.cpu: hardware has 6 breakpoints, 4 watchpoints
  Info : stm32f1x.cpu: external reset detected
  Info : starting gdb server for stm32f1x.cpu on 3333
  Info : Listening on port 3333 for gdb connections
  ```
Depois disso, você pode soltar o botão de reset.

Este bootloader requer 2KiB de espaço de flash (o aplicativo deve ser compilado com endereço inicial de 2KiB).

O programa hid-flash é usado para enviar um binário ao bootloader. Você pode instalar este software com os seguintes comandos:

```
sudo apt install libusb-1.0
cd ~/klipper/lib/hidflash
make
```

Se o bootloader estiver em execução você pode gravar com algo como:

```
~/klipper/lib/hidflash/hid-flash ~/klipper/out/klipper.bin
```
alternativamente, você pode usar `make flash` para gravar o klipper diretamente:

```
make flash FLASH_DEVICE=1209:BEBA
```
OU se o klipper já tiver sido gravado anteriormente:

```
make flash FLASH_DEVICE=/dev/ttyACM0
```

Pode ser necessário entrar manualmente no bootloader, isso pode ser feito configurando "boot 0" em nível baixo e "boot 1" em nível alto. Na SKR Mini E3 o "Boot 1" não está disponível, então pode ser feito configurando o pino PA2 em nível baixo se você gravou "hid_btt_skr_mini_e3.bin". Este pino é identificado como "TX0" no cabeçalho TFT no documento "PIN" da SKR Mini E3. Há um pino de terra próximo ao PA2 que você pode usar para puxar PA2 para nível baixo.

<a id="stm32f103stm32f072-with-msc-bootloader"></a>
### STM32F103/STM32F072 com bootloader MSC

O [bootloader MSC](https://github.com/Telekatz/MSC-stm32f103-bootloader) é um bootloader sem necessidade de driver capaz de gravar via USB.

É possível gravar o bootloader via serial 3.3V usando stm32flash conforme observado na seção stm32duino acima, substituindo o nome do arquivo pelo binário do bootloader MSC desejado (ou seja: MSCboot-Bluepill.bin para a blue pill).

Para placas STM32F072 também é possível gravar o bootloader via USB (via DFU) com algo como:

```
 dfu-util -d 0483:df11 -a 0 -R -D  MSCboot-STM32F072.bin -s0x08000000:leave
```

Este bootloader usa 8KiB ou 16KiB de espaço de flash, consulte a descrição do bootloader (o aplicativo deve ser compilado com o endereço inicial correspondente).

O bootloader pode ser ativado pressionando o botão de reset da placa duas vezes. Assim que o bootloader for ativado, a placa aparece como um pen drive USB no qual o arquivo klipper.bin pode ser copiado.

<a id="stm32f103stm32f0x2-with-canboot-bootloader"></a>
### STM32F103/STM32F0x2 com bootloader CanBoot

O bootloader [CanBoot](https://github.com/Arksine/CanBoot) oferece uma opção para enviar o firmware do Klipper via CANBUS. O próprio bootloader é derivado do código-fonte do Klipper. Atualmente o CanBoot suporta os modelos STM32F103, STM32F042 e STM32F072.

Recomenda-se usar um programador ST-Link para gravar o CanBoot, porém deve ser possível gravar usando `stm32flash` em dispositivos STM32F103, e `dfu-util` em dispositivos STM32F042/STM32F072. Consulte as seções anteriores deste documento para instruções sobre esses métodos de gravação, substituindo `canboot.bin` pelo nome do arquivo quando apropriado. O repositório CanBoot ligado acima fornece instruções para compilar o bootloader.

Na primeira vez que o CanBoot for gravado ele deve detectar que não há aplicativo presente e entrar no bootloader. Se isso não ocorrer é possível entrar no bootloader pressionando o botão de reset duas vezes em sucessão.

O utilitário `flashtool.py` fornecido na pasta `lib/katapult` pode ser usado para enviar o firmware do Klipper. O UUID do dispositivo é necessário para gravar. Se você não tiver um UUID é possível consultar os nós que estão atualmente executando o bootloader:

```
python3 flash_can.py -q
```
Isso retornará UUIDs para todos os nós conectados ainda sem UUID atribuído. Isso deve incluir todos os nós atualmente no bootloader.

Depois de obter um UUID, você pode enviar o firmware com o seguinte comando:

```
python3 flash_can.py -i can0 -f ~/klipper/out/klipper.bin -u aabbccddeeff
```

Onde `aabbccddeeff` é substituído pelo seu UUID. Observe que as opções `-i` e `-f` podem ser omitidas, elas assumem por padrão `can0` e `~/klipper/out/klipper.bin` respectivamente.

Ao compilar o Klipper para uso com CanBoot, selecione a opção de Bootloader de 8 KiB.

<a id="stm32f4-micro-controllers-skr-pro-11"></a>
## Microcontroladores STM32F4 (SKR Pro 1.1)

Os microcontroladores STM32F4 vêm equipados com um bootloader de sistema integrado capaz de gravar via USB (via DFU), serial 3.3V e vários outros métodos (consulte o documento AN2606 da ST para mais informações). Algumas placas STM32F4, como a SKR Pro 1.1, não conseguem entrar no bootloader DFU. O bootloader HID está disponível para placas baseadas em STM32F405/407 caso o usuário prefira gravar via USB em vez de usar o cartão SD. Observe que pode ser necessário configurar e compilar uma versão específica para sua placa; um [build para a SKR Pro 1.1 está disponível aqui](https://github.com/Arksine/STM32_HID_Bootloader/releases/latest).

A menos que sua placa seja compatível com DFU, o método de gravação mais acessível provavelmente é via serial 3.3V, que segue o mesmo procedimento de [gravar o STM32F103 usando stm32flash](#stm32f103-micro-controllers-blue-pill-devices). Por exemplo:

```
wget https://github.com/Arksine/STM32_HID_Bootloader/releases/download/v0.5-beta/hid_bootloader_SKR_PRO.bin

stm32flash -w hid_bootloader_SKR_PRO.bin -v -g 0 /dev/ttyAMA0
```

Este bootloader requer 16Kib de espaço de flash no STM32F4 (o aplicativo deve ser compilado com endereço inicial de 16KiB).

Como no STM32F1, o STM32F4 usa a ferramenta hid-flash para enviar binários ao MCU. Consulte as instruções acima para detalhes sobre como compilar e usar o hid-flash.

Pode ser necessário entrar manualmente no bootloader, isso pode ser feito configurando "boot 0" em nível baixo, "boot 1" em nível alto e conectando o dispositivo. Após a programação estar concluída desconecte o dispositivo e retorne "boot 1" para nível baixo para que o aplicativo seja carregado.

<a id="lpc176x-micro-controllers-smoothieboards"></a>
## Microcontroladores LPC176x (Smoothieboards)

Este documento não descreve o método para gravar um bootloader em si - consulte: <http://smoothieware.org/flashing-the-bootloader> para mais informações sobre esse tópico.

É comum que as Smoothieboards venham com um bootloader de: <https://github.com/triffid/LPC17xx-DFU-Bootloader>. Ao usar este bootloader, o aplicativo deve ser compilado com endereço inicial de 16KiB. A maneira mais fácil de gravar um aplicativo com este bootloader é copiar o arquivo do aplicativo (por ex., `out/klipper.bin`) para um arquivo chamado `firmware.bin` em um cartão SD e então reiniciar o microcontrolador com esse cartão SD.

<a id="running-openocd-on-the-raspberry-pi"></a>
## Executando OpenOCD no Raspberry PI

OpenOCD é um pacote de software que pode realizar gravação e depuração de chips em baixo nível. Ele pode usar os pinos GPIO em um Raspberry Pi para comunicar com diversos chips ARM.

Esta seção descreve como instalar e iniciar o OpenOCD. Ela é derivada das instruções em: <https://learn.adafruit.com/programming-microcontrollers-using-openocd-on-raspberry-pi>

Comece baixando e compilando o software (cada etapa pode levar vários minutos e a etapa "make" pode levar mais de 30 minutos):

```
sudo apt-get update
sudo apt-get install autoconf libtool telnet
mkdir ~/openocd
cd ~/openocd/
git clone http://openocd.zylin.com/openocd
cd openocd
./bootstrap
./configure --enable-sysfsgpio --enable-bcm2835gpio --prefix=/home/pi/openocd/install
make
make install
```

<a id="configure-openocd"></a>
### Configurando o OpenOCD

Crie um arquivo de configuração do OpenOCD:

```
nano ~/openocd/openocd.cfg
```

Use uma configuração semelhante à seguinte:

```
# Usa pinos da RPi: GPIO25 para SWDCLK, GPIO24 para SWDIO, GPIO18 para nRST
source [find interface/raspberrypi2-native.cfg]
bcm2835gpio_swd_nums 25 24
bcm2835gpio_srst_num 18
transport select swd

# Usar fio de reset por hardware para resets do chip
reset_config srst_only
adapter_nsrst_delay 100
adapter_nsrst_assert_width 100

# Especificar o tipo de chip
source [find target/atsame5x.cfg]

# Definir a velocidade do adaptador
adapter_khz 40

# Conectar ao chip
init
targets
reset halt
```

<a id="wire-the-raspberry-pi-to-the-target-chip"></a>
### Ligando o Raspberry Pi ao chip de destino

Desligue tanto o Raspberry Pi quanto o chip de destino antes de fazer a fiação! Verifique se o chip de destino usa 3.3V antes de conectar a um Raspberry Pi!

Conecte GND, SWDCLK, SWDIO e RST no chip de destino a GND, GPIO25, GPIO24 e GPIO18 respectivamente no Raspberry Pi.

Em seguida ligue o Raspberry Pi e forneça energia ao chip de destino.

<a id="run-openocd"></a>
### Executando o OpenOCD

Execute o OpenOCD:

```
cd ~/openocd/
sudo ~/openocd/install/bin/openocd -f ~/openocd/openocd.cfg
```

O acima deve fazer o OpenOCD emitir algumas mensagens de texto e então aguardar (ele não deve retornar imediatamente ao prompt do shell Unix). Se o OpenOCD sair sozinho ou se continuar emitindo mensagens de texto, verifique novamente a fiação.

Uma vez que o OpenOCD esteja em execução e estável, pode-se enviar comandos a ele via telnet. Abra outra sessão ssh e execute o seguinte:

```
telnet 127.0.0.1 4444
```

(Pode-se sair do telnet pressionando ctrl+] e então executando o comando "quit".)

<a id="openocd-and-gdb"></a>
### OpenOCD e gdb

É possível usar OpenOCD com gdb para depurar o Klipper. Os comandos a seguir assumem que se está executando gdb em uma máquina desktop.

Adicione o seguinte ao arquivo de configuração do OpenOCD:

```
bindto 0.0.0.0
gdb_port 44444
```

Reinicie o OpenOCD no Raspberry Pi e então execute o seguinte comando Unix na máquina desktop:

```
cd /path/to/klipper/
gdb out/klipper.elf
```

Dentro do gdb execute:

```
target remote octopi:44444
```

(Substitua "octopi" pelo nome de host do Raspberry Pi.) Uma vez que o gdb esteja em execução é possível definir pontos de interrupção (breakpoints) e inspecionar registradores.
