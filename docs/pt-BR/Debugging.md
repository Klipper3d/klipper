<a id="debugging"></a>
# Depuração

[English](../Debugging.md) | **Português brasileiro**

Este documento descreve algumas das ferramentas de depuração do Klipper.

<a id="running-the-regression-tests"></a>
## Execução dos testes de regressão

O repositório principal do Klipper no GitHub usa "github actions" para executar uma série de testes de regressão. Pode ser útil executar alguns desses testes localmente.

A verificação de espaço em branco ("whitespace check") do código-fonte pode ser executada com:

```
./scripts/check_whitespace.sh
```

A suíte de testes de regressão do Klippy requer "dicionários de dados" de muitas plataformas. A maneira mais fácil de obtê-los é [baixá-los do github](https://github.com/Klipper3d/klipper/issues/1438). Uma vez baixados os dicionários de dados, use o seguinte para executar a suíte de regressão:

```
tar xfz klipper-dict-20??????.tar.gz
~/klippy-env/bin/python ~/klipper/scripts/test_klippy.py -d dict/ ~/klipper/test/klippy/*.test
```

<a id="manually-sending-commands-to-the-micro-controller"></a>
## Envio manual de comandos ao microcontrolador

Normalmente, o processo klippy.py do hospedeiro seria usado para traduzir comandos gcode em comandos de microcontrolador do Klipper. No entanto, também é possível enviar manualmente esses comandos de MCU (funções marcadas com a macro DECL_COMMAND() no código-fonte do Klipper). Para isso, execute:

```
~/klippy-env/bin/python ./klippy/console.py /tmp/pseudoserial
```

Consulte o comando "HELP" dentro da ferramenta para mais informações sobre sua funcionalidade.

Algumas opções de linha de comando estão disponíveis. Para mais informações, execute: `~/klippy-env/bin/python ./klippy/console.py --help`

<a id="translating-gcode-files-to-micro-controller-commands"></a>
## Tradução de arquivos gcode em comandos de microcontrolador

O código hospedeiro do Klippy pode ser executado em modo lote para produzir os comandos de microcontrolador de baixo nível associados a um arquivo gcode. Inspecionar esses comandos de baixo nível é útil ao tentar entender as ações do hardware de baixo nível. Também pode ser útil comparar a diferença nos comandos de microcontrolador após uma alteração de código.

Para executar o Klippy neste modo lote, há uma etapa única necessária para gerar o "dicionário de dados" do microcontrolador. Isso é feito compilando o código do microcontrolador para obter o arquivo **out/klipper.dict**:

```
make menuconfig
make
```

Uma vez concluído o acima, é possível executar o Klipper em modo lote (consulte [instalação](Installation.md) para as etapas necessárias para construir o ambiente virtual python e um arquivo printer.cfg):

```
~/klippy-env/bin/python ./klippy/klippy.py ~/printer.cfg -i test.gcode -o test.serial -v -d out/klipper.dict
```

O acima produzirá um arquivo **test.serial** com a saída serial binária. Essa saída pode ser traduzida para texto legível com:

```
~/klippy-env/bin/python ./klippy/parsedump.py out/klipper.dict test.serial > test.txt
```

O arquivo resultante **test.txt** contém uma lista legível por humanos dos comandos de microcontrolador.

O modo lote desabilita certos comandos de resposta / solicitação para funcionar. Como resultado, haverá algumas diferenças entre os comandos reais e a saída acima. Os dados gerados são úteis para teste e inspeção; não são úteis para envio a um microcontrolador real.

<a id="motion-analysis-and-data-logging"></a>
## Análise de movimento e registro de dados

O Klipper suporta registrar seu histórico interno de movimento, que pode ser analisado posteriormente. Para usar este recurso, o Klipper deve ser iniciado com o [Servidor de API](API_Server.md) habilitado.

O registro de dados é habilitado com a ferramenta `data_logger.py`. Por exemplo:

```
~/klipper/scripts/motan/data_logger.py /tmp/klippy_uds mylog -s '*'
```

Este comando conectará ao Servidor de API do Klipper, assinará informações de status e movimento e registrará os resultados. Dois arquivos são gerados - um arquivo de dados compactado e um arquivo de índice (por ex., `mylog.json.gz` e `mylog.index.gz`). Após iniciar o registro, é possível concluir impressões e outras ações - o registro continuará em segundo plano. Ao terminar o registro, pressione `ctrl-c` para sair da ferramenta `data_logger.py`.

Os arquivos resultantes podem ser lidos e plotados em gráficos usando a ferramenta `motan_graph.py`. Para gerar gráficos em um Raspberry Pi, é necessária uma etapa única para instalar o pacote "matplotlib":

```
sudo apt-get update
sudo apt-get install python-matplotlib
```
No entanto, pode ser mais conveniente copiar os arquivos de dados para uma máquina classe desktop junto com o código Python no diretório `scripts/motan/`. Os scripts de análise de movimento devem ser executados em qualquer máquina com versão recente de [Python](https://python.org) e [Matplotlib](https://matplotlib.org/) instalados.

Gráficos podem ser gerados com um comando como o seguinte:

```
~/klipper/scripts/motan/motan_graph.py mylog -o mygraph.png
```

Pode-se usar a opção `-g` para especificar os conjuntos de dados a plotar (ela recebe um literal Python contendo uma lista de listas). Por exemplo:

```
~/klipper/scripts/motan/motan_graph.py mylog -g '[["trapq(toolhead,velocity)"], ["trapq(toolhead,accel)"]]'
```

A lista de conjuntos de dados disponíveis pode ser encontrada usando a opção `-l` - por exemplo:

```
~/klipper/scripts/motan/motan_graph.py -l
```

Também é possível especificar opções de plotagem do matplotlib para cada conjunto de dados:

```
~/klipper/scripts/motan/motan_graph.py mylog -g '[["trapq(toolhead,velocity)?color=red&alpha=0.4"]]'
```
Muitas opções do matplotlib estão disponíveis; alguns exemplos são "color", "label", "alpha" e "linestyle".

A ferramenta `motan_graph.py` suporta várias outras opções de linha de comando - use a opção `--help` para ver uma lista. Também pode ser conveniente ver/modificar o próprio script [motan_graph.py](../../scripts/motan/motan_graph.py).

Os registros de dados brutos produzidos pela ferramenta `data_logger.py` seguem o formato descrito no [Servidor de API](API_Server.md). Pode ser útil inspecionar os dados com um comando Unix como o seguinte: `gunzip < mylog.json.gz | tr '\03' '\n' | less`

<a id="generating-load-graphs"></a>
## Geração de gráficos de carga

O arquivo de registro do Klippy (/tmp/klippy.log) armazena estatísticas sobre largura de banda, carga do microcontrolador e carga do buffer do hospedeiro. Pode ser útil plotar essas estatísticas após uma impressão.

Para gerar um gráfico, é necessária uma etapa única para instalar o pacote "matplotlib":

```
sudo apt-get update
sudo apt-get install python-matplotlib
```

Então gráficos podem ser produzidos com:

```
~/klipper/scripts/graphstats.py /tmp/klippy.log -o loadgraph.png
```

Pode-se então ver o arquivo resultante **loadgraph.png**.

Diferentes gráficos podem ser produzidos. Para mais informações, execute: `~/klipper/scripts/graphstats.py --help`

<a id="extracting-information-from-the-klippylog-file"></a>
## Extração de informações do arquivo klippy.log

O arquivo de registro do Klippy (/tmp/klippy.log) também contém informações de depuração. Há um script logextract.py que pode ser útil ao analisar um desligamento do microcontrolador ou problema semelhante. Ele normalmente é executado com algo como:

```
mkdir work_directory
cd work_directory
cp /tmp/klippy.log .
~/klipper/scripts/logextract.py ./klippy.log
```

O script extrairá o arquivo de configuração da impressora e extrairá informações de desligamento da MCU. Os despejos de informações de um desligamento da MCU (se presentes) serão reordenados por carimbo de data/hora para ajudar a diagnosticar cenários de causa e efeito.

<a id="testing-with-simulavr"></a>
## Teste com simulavr

A ferramenta [simulavr](http://www.nongnu.org/simulavr/) permite simular um microcontrolador Atmel ATmega. Esta seção descreve como executar arquivos gcode de teste via simulavr. Recomenda-se executar isso em uma máquina classe desktop (não um Raspberry Pi), pois exige CPU significativa para ser executado com eficiência.

Para usar o simulavr, baixe o pacote simulavr e compile com suporte a python. Observe que o sistema de construção pode precisar ter alguns pacotes (como swig) instalados para construir o módulo python.

```
git clone git://git.savannah.nongnu.org/simulavr.git
cd simulavr
make python
make build
```
Certifique-se de que um arquivo como **./build/pysimulavr/_pysimulavr.*.so** esteja presente após a compilação acima:

```
ls ./build/pysimulavr/_pysimulavr.*.so
```
Este comando deve informar um arquivo específico (por ex. **./build/pysimulavr/_pysimulavr.cpython-39-x86_64-linux-gnu.so**) e não um erro.

Se você está em um sistema baseado em Debian (Debian, Ubuntu etc.), pode instalar os pacotes a seguir e gerar arquivos *.deb para instalação do simulavr em todo o sistema:

```
sudo apt update
sudo apt install g++ make cmake swig rst2pdf help2man texinfo
make cfgclean python debian
sudo dpkg -i build/debian/python3-simulavr*.deb
```

Para compilar o Klipper para uso no simulavr, execute:

```
cd /path/to/klipper
make menuconfig
```

e compile o software do microcontrolador para um AVR atmega644p e selecione suporte à emulação de software SIMULAVR. Então pode-se compilar o Klipper (execute `make`) e iniciar a simulação com:

```
PYTHONPATH=/path/to/simulavr/build/pysimulavr/ ./scripts/avrsim.py out/klipper.elf
```
Observe que se você instalou python3-simulavr em todo o sistema, não precisa definir `PYTHONPATH`, e pode simplesmente executar o simulador como

```
./scripts/avrsim.py out/klipper.elf
```

Então, com o simulavr em execução em outra janela, pode-se executar o seguinte para ler gcode de um arquivo (por ex., "test.gcode"), processá-lo com o Klippy e enviá-lo ao Klipper em execução no simulavr (consulte [instalação](Installation.md) para as etapas necessárias para construir o ambiente virtual python):

```
~/klippy-env/bin/python ./klippy/klippy.py config/generic-simulavr.cfg -i test.gcode -v
```

<a id="using-simulavr-with-gtkwave"></a>
### Uso do simulavr com gtkwave

Um recurso útil do simulavr é sua capacidade de criar arquivos de geração de ondas de sinal com a temporização exata dos eventos. Para isso, siga as instruções acima, mas execute avrsim.py com uma linha de comando como a seguinte:

```
PYTHONPATH=/path/to/simulavr/src/python/ ./scripts/avrsim.py out/klipper.elf -t PORTA.PORT,PORTC.PORT
```

O acima criaria um arquivo **avrsim.vcd** com informações sobre cada alteração nos GPIOs em PORTA e PORTB. Isso poderia então ser visto usando gtkwave com:

```
gtkwave avrsim.vcd
```
