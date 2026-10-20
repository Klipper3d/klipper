# Configuração validada e recuperação

Esta é a configuração da Ender 3 V3 SE do proprietário, com hotend cerâmico
Creality, BMCU, switch no hub acima da extrusora e calha de purga no eixo X.
O proprietário confirmou a troca entre os canais físicos 2 e 4 e a impressão
da plaquinha Falcon 9 em duas cores. A validação física não cobre outras
montagens, outros materiais nem uma reinstalação completa.

## O que já está versionado

| Arquivo | Conteúdo |
| --- | --- |
| `klippy/extras/bmcu_integracao_ender.py` | Carga, retirada, troca durante a impressão e adaptação do Orca |
| `klippy/extras/bmcu_medicao.py` | Medição até o switch e testes assistidos usados na calibração |
| `config/ender3-v3-se-validada/bmcu-multicor-ender.cfg` | Parâmetros de troca e macros de início/fim multicolor |
| `config/ender3-v3-se-validada/bmcu-switch.cfg` | Switch e MCU Linux da Orange Pi |
| `config/ender3-v3-se-validada/impressao.cfg` | Últimas macros conhecidas de início/fim, usadas pela integração |
| `config/ender3-v3-se-validada/klipper-mcu-switch.conf` | Override do serviço host MCU sem `-r` |
| `profiles/orca/ender3-v3-se-bmcu/` | Configuração extraída do projeto que foi impresso e G-codes reutilizáveis |
| `scripts/ender3-v3-se/instalar_multicor_bmcu.py` | Aplicação dos três ajustes compatíveis com BMCU 1.0.6, backup e rollback |

Os módulos nativos de BMCU-Klipper continuam sendo fornecidos pelo projeto
[BMCU-Klipper v1.0.6](https://github.com/jarczakpawel/BMCU-Klipper/tree/v1.0.6).
O instalador modifica somente os métodos de contrato genérico, pré-captura
e leitura do sensor gerenciado necessários para esta montagem. Ele verifica
a estrutura antes de alterar, aceita uma instalação repetida e preserva os
demais caminhos nativos. Os trechos de teste em `test/bmcu_integracao/fixtures/`
vieram de `manager.py`, `endpoints.py` e `protocol.py` daquela versão, sob GPL.

## Parâmetros confirmados

| Etapa | Ajuste |
| --- | --- |
| Chegada | BMCU avança até o switch físico; limite de busca 1500 mm |
| Captura | 20 mm, F180 = 3 mm/s; extrusora começa antes da BMCU |
| Avanço após captura | 80 mm, F120 = 2 mm/s |
| Purga | 70 mm, F120 = 2 mm/s |
| Retirada | Extrusora a F600 até o switch liberar, com assistência da BMCU |
| Estacionamento | Mais 55 mm pela BMCU, conferidos pelo encoder antes de liberar o outro canal |
| Calha | X=-6 para purga; X=-4 no fim da limpeza; Y=123 |
| Durante a troca | Sobe 2 mm relativos ao Z atual e retorna à posição de impressão |
| PETG | Bico 230 °C; mesa 80 °C na primeira camada e 75 °C nas seguintes |
| Orca | PA 0,04; fluxo 1,00; limite volumétrico 20 mm³/s do perfil testado |

Não fixe a retirada até o switch em 60 ou 66 mm: esses foram resultados
observados, não limites permanentes. A leitura do switch determina a parada.
O valor nativo `unload_retract_mm=200` permanece salvo. A integração aplica
55 mm temporariamente depois de liberar o switch e restaura a política nativa
ao concluir a operação. Não duplique a retirada no G-code do Orca.

## Ligações e serviços desta Orange Pi

- BMCU `bmcu0`, UID `CDAB42A1D0BEDF0B39E339E3`, UART `/dev/ttyS1`, 115200 baud.
- BMCU-Klipper 1.0.6; firmware BMCU 1.0.1; transporte sidecar habilitado.
- Ender: `/dev/serial/by-id/usb-1a86_USB_Serial-if00-port0`.
- Switch NO: sinal PC15, pino físico 16; COM no GND, pino físico 6.
- Kernel observado: `6.18.44-current-sunxi64`; linha `gpiochip1/gpio79`.
- MCU Linux: `/tmp/klipper_host_mcu`.
- Idioma da tela: `language: pt_BR` em `[e3v3se_display]`.

O kernel desta imagem não permitiu `sched_setscheduler` com a opção `-r`.
O override conhecido remove somente essa opção; o serviço host MCU passa a
executar `/usr/local/bin/klipper_mcu -I /tmp/klipper_host_mcu`. Não reutilize
esses números GPIO em outra imagem sem conferir `gpiodetect` e `gpioinfo`.
PC9 não é o pino do switch desta montagem.

## Capturar o estado atual antes de reinstalar

**Pendente:** ainda não há no repositório um snapshot completo da Orange Pi.
O Z-offset e a malha atuais precisam vir da máquina; valores antigos da conversa
não substituem essa captura. O coletor inclui os `.cfg`, `SAVE_CONFIG`, JSON de
estado da BMCU, módulos Klippy instalados, fonte Git do commit, alterações locais,
artefatos de compilação disponíveis, unidades systemd, overlays e inventário de
versões. Ele não altera configurações, não movimenta motores e não reinicia serviços.

Com a impressora ociosa, execute no SSH, a partir deste checkout:

```sh
sudo python3 scripts/ender3-v3-se/capturar_configuracao.py
```

Baixe o `backup-restauracao-*.tar.gz` pelo Mainsail, em Máquina → arquivos de
configuração, e guarde outra cópia fora do cartão SD. O backup é privado e pode
conter senhas e tokens; não publique o arquivo inteiro no repositório público.
Após revisar a captura, os arquivos da impressora podem ser incorporados ao Git
sem credenciais. Avisos `Pendente:` identificam arquivos ausentes ou não lidos.
Resolva-os antes de considerar a captura completa.

O coletor verifica se os arquivos mudaram durante a operação e interrompe caso
isso aconteça. Não exige descarregar o filamento. A posição carregada fica
registrada como diagnóstico, não como uma garantia sobre a montagem futura.

Para conferir a integridade e extrair para um diretório novo, sem instalar:

```sh
python3 scripts/ender3-v3-se/verificar_backup.py backup-restauracao-AAAA.tar.gz --extrair recuperacao
```

O manifesto confere SHA256 de cada arquivo antes de extrair. O hash externo
impresso pelo coletor também deve ser guardado para conferir o transporte.

## Restauração após a captura

1. Instale a imagem Linux correspondente e confira os overlays capturados em
   `sistema/boot/armbianEnv.txt`. Os caminhos originais e versões ficam no manifesto.
2. Recupere o checkout Klipper do commit registrado. `fontes/klipper-commit.tar`
   contém o fonte rastreado; `klipper-alteracoes.patch` contém alterações locais;
   `klippy-instalado/` contém também os módulos adicionais em uso. Recrie o ambiente
   Python usando o inventário capturado e as dependências do Klipper.
3. Reinstale os serviços do BMCU-Klipper 1.0.6 e aplique as adaptações da montagem.
   As unidades e seus overrides ficam em `sistema/`; compare seus `ExecStart` para
   identificar eventuais diretórios de transporte externos ao checkout Klipper
   que ainda precisem ser copiados. O coletor não faz imagem completa do SD.
4. Com os serviços parados, restaure a pasta `config/` completa, incluindo includes
   e `SAVE_CONFIG`, com proprietário `pi`. Recupere também o estado BMCU externo,
   se o manifesto indicar um `state_file` fora dessa pasta. Não substitua esses
   arquivos pelos modelos genéricos de `config/ender3-v3-se/`.
5. Recompile o MCU Linux do mesmo fonte e recoloque o override sem `-r`. Recarregue
   systemd e confira o serviço e `/tmp/klipper_host_mcu` antes de iniciar Klipper.
6. Reconcile o filamento físico com o estado da BMCU antes de qualquer carga.
   Uma rota `LOADED` salva não prova que o filamento continua no bico depois da
   remontagem. Confira sensores, canais e homing antes de repetir a troca testada.

O firmware da placa Ender não precisa ser regravado para recuperar os `.cfg`.
Se for necessário gravá-lo, Klippy e firmware precisam corresponder ao mesmo
fonte/release. Os binários capturados servem como evidência do build disponível;
não comprovam por si só qual firmware está gravado na placa. Calibrações internas
da BMCU também devem ser conferidas se houver troca ou regravação da controladora.

Esta etapa ainda depende da captura para fechar caminhos de instalação, PID,
Z-offset, malha e painel reais. Não existe um comando de restauração automática
validado em uma instalação nova neste momento.

## Instalar apenas a integração em uma máquina já configurada

O instalador abaixo corresponde ao procedimento já usado na máquina. Ele exige
BMCU 1.0.6 pronto, switch livre, canais estacionados, impressora ociosa e arquivos
nativos compatíveis. Não execute para fazer o backup da instalação que já funciona.

```sh
sudo python3 scripts/ender3-v3-se/instalar_multicor_bmcu.py
```

Ele usa os módulos e configurações deste checkout, aplica os ajustes compatíveis,
preserva o bloco `SAVE_CONFIG`, cria backup e reinicia o processo Klipper inteiro.
Se o Klipper não ficar pronto, restaura os arquivos anteriores. O caminho do
restaurador do backup é informado no terminal. Uma instalação nova ainda precisa
dos includes do switch, impressão e dos módulos/serviços BMCU, sem duplicar seções.
`BMCU_CONFIGURAR_MULTICOR` é configuração manual: não deve rodar a cada boot nem
no início de cada impressão, pois redefine materiais dos canais 2 e 4.

## Testes

```sh
python3 -m unittest discover -s test/bmcu_integracao -p 'test_*.py'
python3 -m unittest discover -s test/e3v3se_display -p 'test_*.py'
```

Os testes de integração usam simulação; não substituem o teste físico de carga,
retirada e impressão. Os testes do backup verificam integridade, preservação de
calibração, arquivos privados, mudanças durante a coleta e extração sem escrever
fora do diretório escolhido.
