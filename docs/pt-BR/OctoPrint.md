<a id="octoprint-for-klipper"></a>
# OctoPrint para Klipper

[English](../OctoPrint.md) | **Português brasileiro**

O Klipper tem algumas opções de interfaces (front ends), o Octoprint foi a primeira interface original para o Klipper. Este documento dará uma breve visão geral da instalação com esta opção.

<a id="install-with-octopi"></a>
## Instalação com OctoPi

Comece instalando o [OctoPi](https://github.com/guysoft/OctoPi) no computador Raspberry Pi. Use o OctoPi v0.17.0 ou posterior - consulte as [versões do OctoPi](https://github.com/guysoft/OctoPi/releases) para informações sobre versões.

Deve-se verificar se o OctoPi inicializa e se o servidor web do OctoPrint funciona. Após conectar-se à página web do OctoPrint, siga as instruções para atualizar o OctoPrint, se necessário.

Após instalar o OctoPi e atualizar o OctoPrint, será necessário conectar-se via ssh à máquina de destino para executar alguns comandos de sistema.

Comece executando estes comandos no seu dispositivo hospedeiro (host):

**Se você não tiver o git instalado, instale-o com:**

```
sudo apt install git
```
em seguida prossiga:

```
cd ~
git clone https://github.com/Klipper3d/klipper
./klipper/scripts/install-octopi.sh
```

O acima fará o download do Klipper, instalará as dependências de sistema necessárias, configurará o Klipper para executar na inicialização do sistema e iniciará o software hospedeiro (host) do Klipper. Será necessária uma conexão com a internet e a conclusão pode levar alguns minutos.

<a id="installing-with-kiauh"></a>
## Instalação com KIAUH

O KIAUH pode ser usado para instalar o OctoPrint em vários sistemas baseados em Linux que executam uma variante do Debian. Mais informações podem ser encontradas em https://github.com/dw-0/kiauh

<a id="configuring-octoprint-to-use-klipper"></a>
## Configuração do OctoPrint para usar o Klipper

O servidor web do OctoPrint precisa ser configurado para se comunicar com o software hospedeiro (host) do Klipper. Usando um navegador web, faça login na página web do OctoPrint e configure os seguintes itens:

Navegue até a aba Settings (o ícone de chave inglesa no topo da página). Em "Serial Connection", em "Additional serial ports", adicione:

```
~/printer_data/comms/klippy.serial
```
Em seguida, clique em "Save".

*Em algumas configurações mais antigas, este endereço pode ser `/tmp/printer`*

Abra novamente a aba Settings e, em "Serial Connection", altere a configuração "Serial Port" para a adicionada acima.

Na aba Settings, navegue até a subaba "Behavior" e selecione a opção "Cancel any ongoing prints but stay connected to the printer". Clique em "Save".

Na página principal, na seção "Connection" (no canto superior esquerdo da página), certifique-se de que a "Serial Port" esteja definida para a nova porta adicional adicionada e clique em "Connect". (Se ela não estiver na seleção disponível, tente recarregar a página.)

Uma vez conectado, navegue até a aba "Terminal" e digite "status" (sem as aspas) na caixa de entrada de comando e clique em "Send". A janela do terminal provavelmente informará que há um erro ao abrir o arquivo de configuração - isso significa que o OctoPrint está se comunicando com sucesso com o Klipper.

Prossiga para [Installation.md](Installation.md) e a seção *Compilação e gravação do microcontrolador*
