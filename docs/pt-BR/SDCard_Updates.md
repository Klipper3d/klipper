<a id="sdcard-updates"></a>
# Atualizações via cartão SD (SDCard updates)

[English](../SDCard_Updates.md) | **Português brasileiro**

Muitas das placas controladoras populares de hoje vêm com um bootloader capaz de atualizar o firmware via cartão SD. Embora isso seja conveniente em muitas circunstâncias, esses gerenciadores de inicialização geralmente não fornecem outra maneira de atualizar o firmware. Isso pode ser um incômodo se sua placa estiver montada em um local de difícil acesso ou se você precisar atualizar o firmware com frequência. Depois que o Klipper tiver sido inicialmente gravado em um controlador, é possível transferir o novo firmware para o cartão SD e iniciar o procedimento de atualização via ssh.

<a id="typical-upgrade-procedure"></a>
## Procedimento típico de atualização

O procedimento para atualizar o firmware do MCU usando o cartão SD é semelhante ao de outros métodos. Em vez de usar `make flash`, é necessário executar um script auxiliar, `flash-sdcard.sh`. Atualizar uma BigTreeTech SKR 1.3 pode parecer o seguinte:

```
sudo service klipper stop
cd ~/klipper
git pull
make clean
make menuconfig
make
./scripts/flash-sdcard.sh /dev/ttyACM0 btt-skr-v1.3
sudo service klipper start
```

Cabe ao usuário determinar a localização do dispositivo e o nome da placa. Se um usuário precisar programar várias placas, `flash-sdcard.sh` (ou `make flash`, se apropriado) deve ser executado para cada placa antes de reiniciar o serviço do Klipper.

As placas suportadas podem ser listadas com o seguinte comando:

```
./scripts/flash-sdcard.sh -l
```
Se você não vir sua placa listada, pode ser necessário adicionar uma nova definição de placa conforme [descrito abaixo](#board-definitions).

<a id="advanced-usage"></a>
## Uso avançado

Os comandos acima pressupõem que seu MCU se conecta na taxa de transmissão (baud rate) padrão de 250000 e que o firmware está localizado em `~/klipper/out/klipper.bin`. O script `flash-sdcard.sh` fornece opções para alterar esses padrões. Todas as opções podem ser vistas pela tela de ajuda:

```
./scripts/flash-sdcard.sh -h
SD Card upload utility for Klipper

usage: flash_sdcard.sh [-h] [-l] [-c] [-s] [-b <baud>] [-f <firmware>]
                       <device> <board>

positional arguments:
  <device>        device serial port
  <board>         board type

optional arguments:
  -h              show this message
  -l              list available boards
  -c              run flash check/verify only (skip upload)
  -s              use fast SPI speed (4MHz)
  -b <baud>       serial baud rate (default is 250000)
  -f <firmware>   path to klipper.bin
```

Se sua placa foi programada com um firmware que se conecta em uma taxa de transmissão personalizada, é possível atualizar especificando a opção `-b`:

```
./scripts/flash-sdcard.sh -b 115200 /dev/ttyAMA0 btt-skr-v1.3
```

Se você desejar programar uma compilação do Klipper localizada em outro lugar que não o local padrão, isso pode ser feito especificando a opção `-f`:

```
./scripts/flash-sdcard.sh -f ~/downloads/klipper.bin /dev/ttyAMA0 btt-skr-v1.3
```

Observe que ao atualizar uma MKS Robin E3 não é necessário executar manualmente `update_mks_robin.py` e fornecer o binário resultante para `flash-sdcard.sh`. Esse procedimento é automatizado durante o processo de upload.

A opção `-c` é usada para executar uma operação somente de verificação (check ou verify-only) para testar se a placa está executando o firmware especificado corretamente. Essa opção destina-se principalmente a casos em que um ciclo de energia manual é necessário para concluir o procedimento de programação, como com bootloaders que usam modo SDIO em vez de SPI para acessar seus cartões SD. (Veja Ressalvas abaixo) Mas ela também pode ser usada a qualquer momento para verificar se o código programado na placa corresponde à versão na sua pasta de compilação em qualquer placa suportada.

<a id="failure-to-initialize"></a>
## Falha ao inicializar

Alguns cartões SD podem falhar ao inicializar na velocidade SPI padrão de 400KHz. Nessa situação, é possível usar `-s` para acionar o periférico SPI a 4MHz. Por exemplo:

```
./scripts/flash-sdcard.sh -s /dev/ttyACM0 btt-skr-v1.3
```

Se o dispositivo ainda falhar ao inicializar, então a causa da falha não está relacionada à velocidade e provavelmente é resultado de uma das seguintes condições:

- O cartão SD está formatado incorretamente. Deve ser `fat` ou `fat32`.
- Tentativa de inicializar um cartão usando a interface SPI que já foi inicializado via SDIO.
- O cartão SD falhou ou está corrompido.

<a id="caveats"></a>
## Ressalvas

- Conforme mencionado na introdução, este método funciona apenas para atualizar o firmware. O procedimento inicial de gravação deve ser feito manualmente conforme as instruções aplicáveis à sua placa controladora.
- Embora seja possível gravar uma compilação que altere o Baud da serial ou a interface de conexão (por exemplo: de USB para UART), a verificação sempre falhará, pois o script não conseguirá se reconectar ao MCU para verificar a versão atual.
- Somente placas que usam SPI para comunicação com o cartão SD são suportadas. Placas que usam SDIO, como a Flymaker Flyboard e a MKS Robin Nano V1/V2, não funcionarão no modo SDIO. No entanto, geralmente é possível gravar essas placas usando o modo SPI por software. Mas se o bootloader da placa usar apenas o modo SDIO para acessar o cartão SD, será necessário desligar e ligar a placa e o cartão SD para que o modo possa alternar de SPI de volta para SDIO para concluir a regravação. Essas placas devem ser definidas com `skip_verify` habilitado para pular a etapa de verificação imediatamente após a gravação. Então, após o ciclo de energia manual, você pode executar novamente o mesmo comando `./scripts/flash-sdcard.sh`, mas adicionar a opção `-c` para concluir a operação de verificação. Consulte [Gravando placas que usam SDIO](#flashing-boards-that-use-sdio) para exemplos.

<a id="board-definitions"></a>
## Definições de placa

A maioria das placas comuns deve estar disponível, no entanto é possível adicionar uma nova definição de placa se necessário. As definições de placa estão localizadas em `~/klipper/scripts/spi_flash/board_defs.py`. As definições são armazenadas em um dicionário, por exemplo:

```python
BOARD_DEFS = {
    'generic-lpc1768': {
        'mcu': "lpc1768",
        'spi_bus': "ssp1",
        "cs_pin": "P0.6"
    },
    ...<further definitions>
}
```

Os seguintes campos podem ser especificados:

- `mcu`: O tipo de mcu. Isso pode ser obtido após configurar a compilação via `make menuconfig` executando `cat .config | grep CONFIG_MCU`. Este campo é obrigatório.
- `spi_bus`: O barramento SPI conectado ao cartão SD. Isso deve ser obtido a partir do esquema elétrico da placa. Este campo é obrigatório.
- `cs_pin`: O pino de seleção de chip (Chip Select) conectado ao cartão SD. Isso deve ser obtido a partir do esquema elétrico da placa. Este campo é obrigatório.
- `firmware_path`: O caminho no cartão SD para onde o firmware deve ser transferido. O padrão é `firmware.bin`.
- `current_firmware_path`: O caminho no cartão SD onde o arquivo de firmware renomeado está localizado após uma gravação bem-sucedida. O padrão é `firmware.cur`.
- `skip_verify`: Define um valor booleano que instrui os scripts a ignorar a etapa de verificação do firmware durante o processo de gravação. O padrão é `False`. Pode ser definido como `True` para placas que exigem ciclo de energia manual para concluir a gravação. Para verificar o firmware depois, execute o script novamente com a opção `-c` para realizar a etapa de verificação. [Veja ressalvas com cartões SDIO](#caveats)

Se SPI por software for necessário, o campo `spi_bus` deve ser definido como `swspi` e o seguinte campo adicional deve ser especificado:

- `spi_pins`: Devem ser 3 pinos separados por vírgula conectados ao cartão SD no formato `miso,mosi,sclk`.

Deve ser extremamente raro que SPI por software seja necessário, normalmente apenas placas com erros de projeto ou placas que normalmente suportam apenas o modo SDIO para o cartão SD exigirão isso. A definição de placa `btt-skr-pro` fornece um exemplo do primeiro caso, e a definição de placa `btt-octopus-f446-v1` fornece um exemplo do segundo.

Antes de criar uma nova definição de placa, deve-se verificar se uma definição de placa existente atende aos critérios necessários para a nova placa. Se for o caso, um `BOARD_ALIAS` pode ser especificado. Por exemplo, o seguinte apelido pode ser adicionado para especificar `my-new-board` como um apelido para `generic-lpc1768`:

```python
BOARD_ALIASES = {
    ...<previous aliases>,
    'my-new-board': BOARD_DEFS['generic-lpc1768'],
}
```

Se você precisar de uma nova definição de placa e não estiver confortável com o procedimento descrito acima, recomenda-se solicitar uma no [Discord do Klipper](Contact.md).

<a id="flashing-boards-that-use-sdio"></a>
## Gravando placas que usam SDIO

[Conforme mencionado nas Ressalvas](#caveats), placas cujo bootloader usa o modo SDIO para acessar o cartão SD exigem um ciclo de energia da placa, e especificamente do próprio cartão SD, para alternar do modo SPI usado durante a escrita do arquivo no cartão SD de volta para o modo SDIO para que o bootloader o grave na placa. Essas definições de placa usarão o sinalizador `skip_verify`, que instrui a ferramenta de gravação a parar após escrever o firmware no cartão SD para que a placa possa ser manualmente desligada e ligada e a etapa de verificação adiada até que isso esteja concluído.

Há dois cenários -- um com o Host RPi funcionando em uma fonte de alimentação separada e outro quando o Host RPi está funcionando na mesma fonte de alimentação da placa principal sendo gravada. A diferença é se é necessário também desligar o RPi e reconectar via `ssh` após a conclusão da gravação para executar a etapa de verificação, ou se a verificação pode ser feita imediatamente. Aqui estão exemplos dos dois cenários:

<a id="sdio-programming-with-rpi-on-separate-power-supply"></a>
### Programação SDIO com RPi em fonte de alimentação separada

Uma sessão típica com o RPi em uma fonte de alimentação separada se parece com o seguinte. Você precisará, é claro, usar o caminho de dispositivo e o nome de placa corretos:

```
sudo service klipper stop
cd ~/klipper
git pull
make clean
make menuconfig
make
./scripts/flash-sdcard.sh /dev/ttyACM0 btt-octopus-f446-v1
# Desligue e ligue manualmente a placa da impressora aqui quando instruído.
./scripts/flash-sdcard.sh -c /dev/ttyACM0 btt-octopus-f446-v1
sudo service klipper start
```

<a id="sdio-programming-with-rpi-on-the-same-power-supply"></a>
### Programação SDIO com RPi na mesma fonte de alimentação

Uma sessão típica com o RPi na mesma fonte de alimentação se parece com o seguinte. Você precisará, é claro, usar o caminho de dispositivo e o nome de placa corretos:

```
sudo service klipper stop
cd ~/klipper
git pull
make clean
make menuconfig
make
./scripts/flash-sdcard.sh /dev/ttyACM0 btt-octopus-f446-v1
sudo shutdown -h now
# Aguarde o RPi desligar, desligue e ligue sua alimentação e reconecte via ssh quando reiniciar.
sudo service klipper stop
cd ~/klipper
./scripts/flash-sdcard.sh -c /dev/ttyACM0 btt-octopus-f446-v1
sudo service klipper start
```

Neste caso, como o Host RPi está sendo reiniciado, o que reiniciará o serviço `klipper`, é necessário parar o `klipper` novamente antes de executar a etapa de verificação e reiniciá-lo após a conclusão da verificação.

<a id="sdio-to-spi-pin-mapping"></a>
### Mapeamento de pinos SDIO para SPI

Se o esquemático da sua placa usa SDIO para o cartão SD, você pode mapear os pinos conforme descrito no quadro abaixo para determinar os pinos SPI por software compatíveis a atribuir no arquivo `board_defs.py`:

| Pino do cartão SD | Pino do cartão Micro SD | Nome do pino SDIO | Nome do pino SPI |
| :-: | :-: | :-: | :-: |
| 9 | 1 | DATA2 | None (PU)* |
| 1 | 2 | CD/DATA3 | CS |
| 2 | 3 | CMD | MOSI |
| 4 | 4 | +3.3V (VDD) | +3.3V (VDD) |
| 5 | 5 | CLK | SCLK |
| 3 | 6 | GND (VSS) | GND (VSS) |
| 7 | 7 | DATA0 | MISO |
| 8 | 8 | DATA1 | None (PU)* |
| N/A | 9 | Detecção de cartão (CD) | Detecção de cartão (CD) |
| 6 | 10 | GND | GND |

\* None (PU) indica um pino não utilizado com resistor de pull-up
