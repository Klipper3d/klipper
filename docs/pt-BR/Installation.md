<a id="installation"></a>
# Instalação

[English](../Installation.md) | **Português brasileiro**

Estas instruções presumem que o software será executado em um hospedeiro baseado em Linux executando uma interface compatível com o Klipper. Recomenda-se que um SBC (Small Board Computer), como um Raspberry Pi ou dispositivo Linux baseado em Debian, seja usado como máquina hospedeira (consulte o [FAQ](FAQ.md#can-i-run-klipper-on-something-other-than-a-raspberry-pi-3) para outras opções).

Para os fins destas instruções, hospedeiro refere-se ao dispositivo Linux e mcu refere-se à placa da impressora. SBC refere-se ao termo Small Board Computer, como o Raspberry Pi.

<a id="obtain-a-klipper-configuration-file"></a>
## Obter um arquivo de configuração do Klipper

A maioria das configurações do Klipper é determinada por um "arquivo de configuração da impressora" printer.cfg, que será armazenado no hospedeiro. Um arquivo de configuração apropriado pode frequentemente ser encontrado procurando no [diretório config](../../config) por um arquivo que comece com o prefixo "printer-" correspondente à impressora de destino. O arquivo de configuração do Klipper contém informações técnicas sobre a impressora que serão necessárias durante a instalação.

Se não houver um arquivo de configuração de impressora apropriado no diretório do Klipper, tente pesquisar no site do fabricante da impressora para ver se existe algum arquivo de configuração apropriado.

Se nenhum arquivo de configuração adequado puder ser encontrado, mas o tipo de placa de controle da impressora for conhecido, então procure por um [arquivo de configuração](../../config) apropriado começando com um prefixo "generic-". Esses arquivos de exemplo devem permitir que se conclua com sucesso a instalação inicial, mas exigirão alguma personalização para obter o funcionamento completa da impressora.

Também é possível definir uma nova configuração do zero. No entanto, isso requer conhecimento técnico sobre a impressora e seus componentes eletrônicos. É recomendado que a maioria dos usuários comece com um arquivo de configuração apropriado. Se estiver criando um novo arquivo de configuração personalizado, comece com o exemplo mais próximo [arquivo de configuração](../../config) e use a [referência de configuração](Config_Reference.md) do Klipper para obter informações.

<a id="interacting-with-klipper"></a>
## Interação com o Klipper

O Klipper é um firmware de impressora 3D, portanto precisa de algum meio para o usuário interagir com ele.

Atualmente, as melhores escolhas são interfaces que obtêm informações por meio da [API web Moonraker](https://moonraker.readthedocs.io/) e há também a opção de usar o [Octoprint](https://octoprint.org/) para controlar o Klipper.

A escolha depende do usuário, mas o Klipper subjacente é o mesmo em todos os casos. Incentivamos os usuários a pesquisar as opções disponíveis e tomar uma decisão informada.

<a id="obtaining-an-os-image-for-sbcs"></a>
## Obtenção de uma imagem de SO para SBCs

Há muitas maneiras de obter uma imagem de SO para uso do Klipper em SBC, a maioria depende de qual interface você deseja usar. Alguns fabricantes dessas placas SBC também fornecem suas próprias imagens focadas no Klipper.

As duas principais interfaces baseadas em Moonraker são [Fluidd](https://docs.fluidd.xyz/) e [Mainsail](https://docs.mainsail.xyz/), a última das quais tem uma imagem de instalação pronta ["MainsailOS"](https://docs-os.mainsail.xyz/), com opção para Raspberry Pi e algumas variantes de OrangePi.

O Fluidd pode ser instalado via KIAUH (Klipper Install And Update Helper), que é explicado abaixo e é um instalador de terceiros para tudo relacionado ao Klipper.

O OctoPrint pode ser instalado via a popular imagem OctoPi ou via KIAUH; este processo é explicado em [OctoPrint.md](OctoPrint.md)

<a id="installing-via-kiauh"></a>
## Instalação via KIAUH

Normalmente você começaria com uma imagem base para seu SBC, RPiOS Lite por exemplo, ou no caso de um dispositivo Linux x86, Ubuntu Server. Observe que variantes Desktop não são recomendadas devido a certos programas auxiliares que podem impedir que algumas funções do Klipper funcionem e até mascarar o acesso a algumas placas de impressora.

O KIAUH pode ser usado para instalar o Klipper e seus programas associados em vários sistemas baseados em Linux que executam uma forma de Debian. Mais informações podem ser encontradas em https://github.com/dw-0/kiauh

<a id="building-and-flashing-the-micro-controller"></a>
## Configurando e atualizando o microcontrolador

Para compilar o código do microcontrolador, comece executando estes comandos no seu dispositivo hospedeiro:

```
cd ~/klipper/
make menuconfig
```

Os comentários no topo de [arquivo de configuração da impressora](#obtain-a-klipper-configuration-file) deve descrever o que deve ser configurado durante "make menuconfig". Abra o arquivo em um navegador/editor e procure por estas instruções perto do topo do arquivo. Depois de ajustar tudo com "menuconfig", pressione "Q" para sair e, em seguida, "Y" para salvar. Em seguida, execute:

```
make
```

Se os comentários no topo do [arquivo de configuração da impressora](#obtain-a-klipper-configuration-file) descreverem etapas personalizadas para fazer "flash" da imagem final na placa de controle da impressora, então siga essas etapas e prossiga para [configurar o OctoPrint](OctoPrint.md#configuring-octoprint-to-use-klipper).

Caso contrário, as etapas a seguir são frequentemente usadas para atualizar (flash) a placa controladora. Primeiro, é necessário determinar a porta serial conectada ao microcontrolador. Execute o seguinte:

```
ls /dev/serial/by-id/*
```

Deve relatar algo semelhante ao exemplo:

```
/dev/serial/by-id/usb-1a86_USB2.0-Serial-if00-port0
```

É comum que cada impressora tenha seu próprio nome exclusivo de porta serial. Este nome exclusivo será usado ao fazer o flash do microcontrolador. É possível que haja várias linhas na saída acima - se houver, escolha a linha correspondente ao microcontrolador. Se muitos itens forem listados e a escolha for ambígua, desconecte a placa e execute o comando novamente, o item ausente será sua placa de impressão (consulte o [FAQ](FAQ.md#wheres-my-serial-port) para mais informações).

Para microcontroladores comuns com chips STM32 ou clones, chips LPC e outros, é usual que estes precisem de um flash inicial do Klipper via cartão SD.

Ao fazer flash com este método, é importante garantir que a placa de impressão não esteja conectada via USB ao hospedeiro, pois algumas placas podem realimentar energia para a placa e impedir que o flash ocorra.

Observe que a maioria das placas de impressão que usam cartões SD para flash implementará algum tipo de proteção contra loop de flash para quando o cartão SD for deixado no lugar. Há dois métodos comuns:

Nome de arquivo com alteração obrigatória (geralmente placas de impressão "originais" (stock)):

Estas placas exigem que o arquivo de firmware tenha um nome diferente a cada flash (por exemplo, firmware1.bin, firmware2.bin, etc.). Se você reutilizar o mesmo nome de arquivo, a placa poderá ignorá-lo e não atualizar.

Renomeação automática de arquivo (geralmente placas de impressão de reposição:

Outras placas permitem usar o mesmo nome de arquivo, comumente firmware.bin, mas após o flash, a placa renomeia o arquivo para firmware.cur. Isso ajuda a indicar que o firmware foi gravado com sucesso e evita que ele seja gravado novamente na próxima inicialização.

Antes de fazer o flash, certifique-se de verificar qual comportamento sua placa segue.

Para microcontroladores comuns que usam chips Atmega, por exemplo o 2560, o código pode ser gravado com algo semelhante a:

```
sudo service klipper stop
make flash FLASH_DEVICE=/dev/serial/by-id/usb-1a86_USB2.0-Serial-if00-port0
sudo service klipper start
```

Certifique-se de atualizar o FLASH_DEVICE com o nome exclusivo da porta serial da impressora.

Para microcontroladores comuns que usam chips RP2040, o código pode ser gravado com algo semelhante a:

```
sudo service klipper stop
make flash FLASH_DEVICE=first
sudo service klipper start
```

É importante observar que chips RP2040 podem precisar ser colocados em modo Boot antes desta operação.

<a id="configuring-klipper"></a>
## Configurando Klipper

O próximo passo é copiar o [arquivo de configuração da impressora](#obtain-a-klipper-configuration-file) para o hospedeiro.

Indiscutivelmente, a maneira mais fácil de definir o arquivo de configuração do Klipper é usando os editores integrados no Mainsail ou Fluidd. Estes permitirão ao usuário abrir os exemplos de configuração e salvá-los como printer.cfg.

Outra opção é usar um editor de desktop que suporte edição de arquivos pelos protocolos "scp" e/ou "sftp". Há ferramentas gratuitas disponíveis que suportam isso (por ex., Notepad++, WinSCP e Cyberduck). Carregue o arquivo de configuração da impressora no editor e salve-o como um arquivo chamado "printer.cfg" no diretório home do usuário pi (ou seja, /home/pi/printer.cfg).

Alternativamente, também se pode copiar e editar o arquivo diretamente no hospedeiro via SSH. Isso pode se parecer com o seguinte (certifique-se de atualizar o comando para usar o nome de arquivo de configuração da impressora apropriado):

```
cp ~/klipper/config/example-cartesian.cfg ~/printer.cfg
nano ~/printer.cfg
```

É comum que cada impressora tenha seu próprio nome exclusivo para o microcontrolador. O nome pode mudar após o flash do Klipper, então execute novamente essas etapas mesmo que elas já tenham sido feitas. Execute:

```
ls /dev/serial/by-id/*
```

Deve relatar algo semelhante ao exemplo:

```
/dev/serial/by-id/usb-1a86_USB2.0-Serial-if00-port0
```

Após, atualize o arquivo de configuração com o nome adequado. Por exemplo, atualize a seção `[mcu]` para algo parecido com:

```
[mcu]
serial: /dev/serial/by-id/usb-1a86_USB2.0-Serial-if00-port0
```

Após criar e editar o arquivo, será necessário emitir um comando "restart" no console de comandos para carregar a configuração. Um comando "status" informará que a impressora está pronta se o arquivo de configuração do Klipper for lido com sucesso e o microcontrolador for encontrado e configurado com sucesso.

Ao personalizar o arquivo de configuração, não é incomum que o Klipper relate um erro. Se ocorrer, faça as correções necessárias no arquivo e execute "restart" até que "status" informe que a impressora está pronta.

O Klipper relata mensagens de erro via console de comandos e pop-ups no Fluidd e Mainsail. O comando "status" pode ser usado para apresentar novamente as mensagens de erro. Um log está disponível e geralmente localizado em `~/printer_data/logs/klippy.log`.

Após o Klipper relatar que a impressora está pronta, prossiga para o [documento de verificação da configuração](Config_checks.md) para executar algumas verificações básicas no arquivo de configuração. Veja [referência da documentação](Overview.md) para outras informações.
