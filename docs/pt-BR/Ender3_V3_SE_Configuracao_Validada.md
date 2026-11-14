# Integração BMCU por switch e recuperação privada

[English](../Ender3_V3_SE_Configuracao_Validada.md) | **Português brasileiro**

Este guia cobre a integração opcional da Ender 3 V3 SE para BMCU-Klipper 1.1.2
com firmware 1.0.4. Quatro canais alimentadores compartilham uma única
extrusora física por meio de um switch de entrada acima das engrenagens.
Uma calha de purga com limpador é usada nas trocas de ferramenta. O pacote
externo controla transporte, estado das rotas, mapas de impressão e serviços.

<a id="installation-scope"></a>
## Escopo da instalação

A sequência fornecida foi testada fisicamente em uma Ender 3 V3 SE
GD32F303RET6, com hotend de cerâmica, Orange Pi Zero3 e PETG. Uma impressão
em duas cores funcionou. Essas observações não validam outros hardwares,
materiais ou impressão em quatro cores. Distâncias e coordenadas de exemplo
são pontos de partida a verificar, não valores universais de calibração.

Instale o pacote oficial BMCU e seus serviços antes desta integração. Mantenha
uma única seção `[bmcu]` e uma única extrusora física. O instalador atualmente
suporta um alimentador chamado `bmcu0` e o endpoint `extruder`; não vincula
a instalação a um UID de dispositivo específico.

<a id="files"></a>
## Arquivos

| Caminho | Finalidade |
| --- | --- |
| `integrations/bmcu/bmcu_ender_integration.py` | Captura, retirada, limpeza e adaptação das ferramentas do Orca |
| `integrations/bmcu/bmcu_measurement.py` | Comandos de medição de chegada e calibração assistida |
| `config/ender3-v3-se-validada/bmcu-multicor-ender.cfg` | Sequência de exemplo e macros de impressão |
| `config/ender3-v3-se-validada/bmcu-switch.cfg` | Exemplo de switch de entrada na Orange Pi |
| `profiles/orca/ender3-v3-se-bmcu/` | Trechos reutilizáveis do fatiador |
| `scripts/ender3-v3-se/install_multicolor.py` | Backup, verificações de compatibilidade, instalação e rollback |

Comandos, parâmetros, valores enumerados, opções de configuração e campos
públicos de status usam a API v2 em inglês nos dois idiomas da tela.
O instalador migra nomes conhecidos da configuração v1 e callbacks persistidos
dos endpoints; rejeita colisões ambíguas entre nomes antigos/novos sem ativar
arquivos. Não há aliases públicos em português. **Refatie os trabalhos de
impressão existentes com o perfil v2 do Orca**; o G-code armazenado não é
convertido automaticamente. Helpers privados de movimento e fixtures legadas
mantêm sua implementação testada, independentemente dos nomes públicos.

Expressões Jinja personalizadas que combinam status da integração com literais
legados de diagnóstico são rejeitadas para revisão manual antes de ativar.
Atualize essas condições para os valores públicos em inglês; o instalador não
reescreve rótulos privados indiscriminadamente nem presume que renomear um campo
preserva o significado de uma condição. Acessos por ponto ao status da integração
e aliases desse status copiados, derivados ou reatribuídos também exigem revisão
manual, em vez de serem ativados com semântica incerta. Use acessos diretos por
colchetes e confira as expressões personalizadas conforme o esquema de status
em inglês. Templates arbitrários ainda exigem revisão separada.

<a id="wiring-and-configuration"></a>
## Ligação e configuração

Conecte TX do host ao RX do alimentador, RX do host ao TX do alimentador e
compartilhe o GND. Identifique o caminho UART real e garanta que nenhum console
serial o esteja usando. O exemplo da Orange Pi usa `/dev/ttyS1`; outra imagem
pode expor um caminho diferente.

Para o switch de entrada normalmente aberto, conecte COM ao GND e NO à entrada
GPIO verificada. No arranjo documentado da Orange Pi Zero3, PC15 é o pino físico
16 e GND é o pino 6. `gpiochip1/gpio79` depende da imagem: confirme com
`gpiodetect` e `gpioinfo`. Reutilize um `[mcu host]` existente; não o defina
duas vezes. Teste os dois estados físicos com
`QUERY_FILAMENT_SENSOR SENSOR=bmcu_entry`.

O `klipper-mcu-switch.conf` remove o escalonamento em tempo real (`-r`) do
serviço da MCU Linux para uma imagem que o rejeitou. Aplique-o somente quando
esse erro estiver presente. O endpoint da MCU do host é `/tmp/klipper_host_mcu`.

<a id="commissioning-example"></a>
## Exemplo de comissionamento

| Fase | Exemplo usado no conjunto testado |
| --- | --- |
| Chegada | Avanço até o sensor de entrada; percurso máximo de 1500 mm |
| Captura | 20 mm a F180; a extrusora inicia antes da assistência do alimentador |
| Avanço até o bico | Mais 80 mm a F120 |
| Purga | 35 mm a F120; reduzido em relação ao teste anterior de 70 mm |
| Retirada | Extrusora a F300 até o sensor de entrada liberar; verificações de buffer e encoder |
| Estacionamento | Mais 55 mm pela BMCU, verificados pelo encoder |
| Purga/limpeza | X=-6 / X=-4, Y=123; elevação Z relativa de 2 mm |
| PETG | Bico a 230 °C; mesa a 80 °C na primeira camada, depois 75 °C |

Confira o curso e as folgas antes de usar as coordenadas de purga. Meça captura,
avanço ao bico, purga e estacionamento no caminho montado. Não substitua a
transição do switch por uma retração fixa de 60 ou 66 mm. O sensor de entrada
determina quando a extrusora para; o encoder verifica então o estacionamento.
Os ajustes nativos de retração do canal são restaurados após a operação.

Não suponha que uma transição do sensor de entrada prove que o filamento já
está entre as engrenagens. A captura iniciada pela extrusora cobre essa lacuna.
Mantenha o hotend na temperatura apropriada do material para captura e retirada.

<a id="nozzle-wiping-defaults"></a>
## Padrões de limpeza do bico

Os padrões da fonte atual, **não da tag v2.0.0**, usam seis idas e voltas
completas em X entre -6 e -3, seguidas da passagem final até -3. Y fica em 123
e Z permanece na posição já elevada durante a limpeza. Isso altera a limpeza
do bico, não o curso da lâmina. A tabela de comissionamento acima registra o
conjunto testado anteriormente; a limpeza ampliada ainda exige aceitação física.

O instalador preserva um `bmcu-multicor-ender.cfg` existente, inclusive
coordenadas e quantidade de ciclos. Atualizar somente o código não substitui
valores antigos explícitos de `wipe_x: -4` ou `wipes: 3`. Após conferir as
folgas, edite estas opções na seção `[bmcu_ender_integration]` **existente**,
sem duplicá-la, e reinicie o Klipper parado para adotar o novo padrão:

```ini
purge_x: -6
wipe_x: -3
purge_y: 123
wipes: 6
```

Mantenha `purge_mm: 35`, temperaturas do material e taxas de extrusão calibradas.
A limpeza continua em F600 com a pausa existente de um segundo para acomodação.
Treze travessias de 3 mm percorrem 39 mm, em vez das sete travessias anteriores
de 2 mm (14 mm). A F600, somente o percurso ideal em velocidade constante leva
3,9 s em vez de 1,4 s; são estimativas sem aceleração, esperas de conclusão e
verificações do controlador, não medições do ciclo. Não apresente a limpeza
ampliada como mais rápida.

Sessão, presença no canal, leitura do switch de entrada, temperatura mínima de
extrusão e assistência ativa do alimentador são verificados até a conclusão da
purga, após a pausa e depois de cada travessia concluída, inclusive a final.
Uma falha interrompe a carga em vez de relatar sucesso. Durante uma operação
manual, essas verificações também respeitam o cancelamento pela tela. PWM zero
do motor no alvo do buffer não significa modo de assistência parado. Avanço ao
bico e retirada entram em assistência nas fronteiras de fase, sem reenviar o
mesmo modo a cada trecho; verificações de sensor/encoder e reinícios intencionais
da retirada permanecem.

Antes de aceitar a mudança no equipamento:

1. Termine ou cancele a impressão, estacione os canais e confirme o batente do
   cortador recolhido. Verifique se todo o trajeto X=-6 a X=-3 fica livre em
   Y=123 e no Z elevado configurado, sem aquecer ou extrudar nessa conferência.
2. Após confirmar a folga, teste uma carga aquecida supervisionada com os ajustes
   calibrados do material. Conte seis idas e voltas completas e a passagem
   final; Y e Z precisam permanecer parados durante a limpeza.
3. Confirme que a purga cai na calha em vez de ficar presa no bico, com fluxo
   adequado e sem patinação ou colisão. Inspecione as transições de cor antes
   de reduzir o volume de purga ou aumentar taxas.
4. Repita nos quatro canais e registre o tempo completo das trocas e as falhas
   nas mesmas condições. Testes de software sozinhos não comprovam desprendimento
   da purga nem economia total de tempo ou material.

<a id="install-or-update-an-existing-integration"></a>
## Instalar ou atualizar uma integração existente

Faça backup primeiro. Mantenha a impressora parada, todos os canais estacionados
e o sensor de entrada livre. Para preservar um ponto de rollback completo do
host/display, execute o novo instalador de um checkout limpo e separado da
release v2 publicada. Não atualize o checkout ativo `~/klipper` com `git pull`
antes disso. Conclua primeiro a
[manutenção separada de pacote/firmware](releases/v2.0.1.md#upgrade-and-rollback);
os comandos exigem BMCU 1.1.2 / 1.0.4 já pronta. Depois que a v2.0.1 for publicada:

```sh
git clone --branch v2.0.1 --single-branch \
  https://github.com/bernardokcosta/ender3-v3-se-klipper-bmcu.git \
  "$HOME/klipper-v2-source"
sudo python3 "$HOME/klipper-v2-source/scripts/ender3-v3-se/install_multicolor.py" \
  --host-source "$HOME/klipper-v2-source"
```

O instalador verifica a estrutura 1.1.2 instalada, ajusta o contrato genérico
e as fases de pré-captura, instala módulos opcionais e preserva `SAVE_CONFIG`
e os ajustes existentes da integração. Um único bloqueio serializa instalação
e rollback. Os serviços relevantes são parados, e o estado é relido antes do
backup de referência. O checkout anterior inteiro do host, incluindo código da
tela, histórico Git e arquivos particulares não rastreados, é preservado.
Módulos opcionais instalados acompanham o novo checkout; outros arquivos não
rastreados ficam na árvore preservada e são listados no manifesto privado.

Esse fluxo aceita atualizações somente no host: entradas da MCU/compilação e
requisitos Python precisam coincidir. Mudanças de firmware ou ambiente são
recusadas para revisão separada; o instalador nunca grava MCUs nem instala
dependências Python. Arquivos rastreados modificados no host/alvo, layouts Git
sem suporte, conflitos de módulos opcionais ou mudanças nos arquivos preparados
são rejeitados em vez de descartar o trabalho do usuário. Os arquivos preparados
são mantidos para diagnóstico, não apagados automaticamente.

O helper nativo de processos para o transporte após o Klipper sair e inicia
o daemon alterado antes do Klipper; não se presume serviço systemd separado.
Se a inicialização
falhar, o host/display anterior e as configurações afetadas são restaurados.
Somente renomeações conhecidas de callbacks são desfeitas no estado BMCU
**atual**; rotas antigas e calibrações não substituem as evidências atuais.
O host restaurado precisa ficar pronto antes de a recuperação ser informada
como bem-sucedida; caso contrário, os serviços ficam parados. O checkout que
falhou permanece disponível para inspeção. Backups e um restaurador independente
são gravados de forma privada no diretório pessoal da instalação.

Executar `install_multicolor.py` sem `--host-source` ainda atualiza somente
a integração. Esse modo não consegue reconstruir um checkout do host alterado
antes e, por isso, deixa os serviços parados após falhar. Atualizações apenas
pelo Git não substituem os módulos opcionais instalados.

O instalador também atualiza a sequência suportada de aquecimento no início da
impressão em `bmcu-multicor-ender.cfg` e em um `impressao.cfg` existente. Após
o homing e o carregamento da mesh, o cabeçote chega à calha de purga configurada
antes de aguardar o bico subir da temperatura de espera (até 150 °C) para a de
impressão. O posicionamento não extruda nem habilita extrusão a frio. Layouts
de início personalizados que não podem ser migrados são rejeitados para revisão
manual; distâncias calibradas, coordenadas e `SAVE_CONFIG` são mantidos.

Na configuração inicial do endpoint, revise os exemplos de macros antes de
executar `BMCU_SETUP_MULTICOLOR`. A configuração não substitui as atribuições
de material/cor dos canais. Não execute a configuração do endpoint a cada
inicialização ou início de impressão.

<a id="everyday-filament-controls"></a>
## Controles diários de filamento

A tela original agrupa cada operação em um só lugar:

O [cartão nativo Mainsail](Ender3_V3_SE_Mainsail.md) acrescenta visualização
atual e edição protegida de material/cor. No fim do filamento, insira a reposição
no canal indicado e confirme por Retomar normal; não resete a BMCU nem o mapa.

- **Filamento / BMCU:** carregar um dos quatro canais físicos, retirar o canal
  ativo ou confirmar **Reset BMCU** após a remoção manual do filamento.
  Uma rota incerta desabilita a carga, mas não oculta o reset protegido.
- **Temperatura:** pré-aquecimento PLA/PETG/TPU, resfriamento, alvos do bico/mesa
  e ventoinha.
- **Mover:** homing, ajustes dos eixos/extrusora, liberação dos motores e
  batente opcional do cortador, com a confirmação de folga existente.
- **Calibrar:** Z offset do CR-Touch, mesh da mesa e ajuste dos parafusos
  quando configurado.
- **Configurações:** selecionar **English / Português** imediatamente e salvar
  explicitamente a configuração quando parada para persistir. Salvar exige
  confirmar um reinício do Klipper.
- Durante a impressão, pausa/ajuste/cancelamento permanecem disponíveis.
  Uma troca pausada com falha oferece **Recuperar filamento**, não um reset
  BMCU global. A confirmação guiada preserva o mapa da impressão pendente
  e usa o canal da falha.

Sem BMCU, o menu de filamento oferece os controles comuns de extrusora única.
Pré-aquecimento/resfriamento e carga/retirada não ficam mais duplicados no menu
Preparar. Com esta integração, a tela reutiliza o mesmo backend do Mainsail:

| Botão do Mainsail | Ação |
| --- | --- |
| `PREHEAT_PLA` | Bico/mesa padrão: 200/60 °C |
| `PREHEAT_PETG` | Bico/mesa padrão: 230/80 °C |
| `PREHEAT_TPU` | Bico/mesa padrão: 230/50 °C |
| `UNLOAD_ACTIVE_CHANNEL` | Retirar o canal BMCU ativo confirmado |
| `LOAD_CHANNEL_1` até `LOAD_CHANNEL_4` | Carregar o canal físico |
| `RESET_BMCU CONFIRM=1` | Reconciliar o estado das rotas após remoção manual, fora de uma impressão |

Altere o pré-aquecimento em `bmcu-controles-ajustes.cfg`; o instalador preserva
esse arquivo em atualizações posteriores. As definições compartilhadas de
`bmcu-controles.cfg` são gerenciadas pelo instalador. O pré-aquecimento da tela
usa essas macros quando disponíveis; sem elas, vale a configuração `preheat_*`
da tela. Confira as temperaturas de exemplo para o filamento real.

Carga/retirada manual sempre define o alvo do bico para 230 °C e aguarda a
temperatura suficiente. Referencia eixos ainda sem referência e move para a
posição de purga configurada antes de alimentar. Carregar outro canal retira
o existente, verifica que o switch de entrada liberou e a rota estacionou,
depois usa a sequência calibrada de captura, avanço ao bico, purga e limpeza.
Selecionar o canal já carregado não faz nada. Retirar de uma rota vazia não
aquece nem move. Filamento-alvo ausente, rota incerta ou divergência sensor/rota
interrompem a ação antes do aquecimento. Impressões ativas/pausadas e contextos
de impressão BMCU abertos precisam ser encerrados antes desses controles.

O alvo de 230 °C da troca manual é independente dos perfis de pré-aquecimento.
O perfil TPU não comprova que um filamento flexível específico possa atravessar
a BMCU; carregue somente materiais verificados no alimentador e caminho reais.

Use o fluxo de atualização preparada acima para instalar esses controles.
Valores calibrados da sequência e o bloco `SAVE_CONFIG` são preservados.
Preferências de idioma em um include da tela são movidas para o `printer.cfg`
principal, para que o `SAVE_CONFIG` nativo não conflite com uma opção incluída.
Somente os atalhos padrão exatos de carga/retirada da tela são removidos, pois
o menu nativo já os oferece; rótulos/scripts personalizados são mantidos.

<a id="macro-purpose-and-caller-audit"></a>
## Finalidade das macros e auditoria dos chamadores

Macros curtas de encaminhamento não são necessariamente inúteis. A auditoria
não encontrou macro G-code removível com segurança nestes fluxos operacionais:

| Grupo de macros | Chamador ou finalidade necessária |
| --- | --- |
| `PAUSE`, `RESUME`, `CANCEL_PRINT` | Ciclo da impressão, recuperação protegida e cancelamento seguro |
| `LOAD_FILAMENT`, `UNLOAD_FILAMENT` | Controles comuns da extrusora e callbacks genéricos do modelo |
| `PREHEAT_*`, `LOAD_CHANNEL_*`, `UNLOAD_ACTIVE_CHANNEL` | Botões do Mainsail compartilhando o backend da tela |
| `BMCU_PREPARE_ENDER`, `BMCU_RELEASE_ENDER` | Callbacks nativos de captura/retirada do endpoint; não são botões de carga manual |
| `BMCU_SETUP_MULTICOLOR` | Comissionamento único e explícito do endpoint, não automação na partida |
| `ENDER_START_MULTICOLOR`, `ENDER_END_MULTICOLOR` | Entradas de início/fim do Orca |
| `ENDER_START_PRINT`, `ENDER_END_PRINT` | Início monocor e comportamento compartilhado de fim de impressão |
| `CUTTER_SERVO`, `TEST_CUTTER`, `RESET_BMCU` | Calibração manual protegida, teste mecânico e recuperação |
| Macros `_...` de ajustes/validação/corpo | Helpers ocultos de implementação/calibração, ainda chamados internamente |
| `BMCU_SETUP_ENDER`, `BMCU_TOOLHEAD_PREPARE`, `BMCU_BEFORE_PULLBACK` | Somente modelo antigo não comissionado; não execute sua configuração sobre o endpoint validado |

As seções de atalho padrão removidas da tela **não** eram macros G-code.
Excluir callbacks ou macros necessários ao fatiador quebraria uma operação,
em vez de apenas organizar sua apresentação. Mantenha atalhos personalizados
somente para ações que os menus nativos ainda não oferecem.

<a id="restoring-the-prepared-update"></a>
## Restaurar a atualização preparada

Mantenha os dois diretórios privados de backup informados/criados na mesma
atualização:

| Local no diretório pessoal da instalação | Conteúdo |
| --- | --- |
| `backup-multicor-<timestamp>/` | `restaurar.py`, helper salvo, arquivo de configuração, originais por arquivo e `host.json` |
| `backup-host-update-<timestamp>/` | `host-record.json` e árvores de checkout preservadas/preparadas |
| `host-before/` no backup do host | Checkout anterior inteiro antes do rollback |
| `host-failed/` no backup do host | Checkout v2 substituído, mantido durante o rollback |
| `host-staged/` no backup do host | Checkout preparado antes da ativação ou evidências da preparação |

Os timestamps não precisam coincidir. Leia `host.json` no backup de configuração
para localizar os caminhos exatos `active`, `previous` e `failed`; não adivinhe
quais diretórios correspondem entre si. Manifestos e arquivos podem conter
caminhos, configurações ou estado privados. Não os publique.

Para rollback manual, termine/cancele a impressão, desobstrua fisicamente o
caminho compartilhado e estacione os canais primeiro. Preserve alterações de
configuração feitas após a atualização em outro backup privado: o restaurador
recupera os arquivos daquela atualização, não calibrações posteriores.
Substitua pelo diretório exato:

```sh
BACKUP="$HOME/backup-multicor-<timestamp>"
sudo python3 "$BACKUP/restaurar.py"
```

Esse comando **aplica** o rollback; não é prévia. Compartilha o bloqueio de
atualização com o instalador. Se o Klipper está rodando, confere primeiro as
condições de impressora parada/canais estacionados. Para os serviços relevantes,
restaura o checkout antigo inteiro e os arquivos afetados, depois desfaz
callbacks reconhecidos no estado atual. Inicia transporte antes do Klipper
e valida a seção antiga da integração. Investigue falhas de restauração/
validação com os serviços parados; não force a partida nem declare rotas livres
manualmente apenas para contornar um erro.

Uma falta de energia entre renomeações da ativação pode deixar `~/klipper`
ausente, com `host-before/` e `host-staged/` preservados. O restaurador gerado
consegue recolocar o checkout antigo. Também trata interrupção após mover o
novo checkout para `host-failed/`, antes de devolver o antigo ao caminho ativo.
Snapshots existentes não são sobrescritos. Se falharem verificações de revisão/
caminho, preserve todas as árvores e manifestos para diagnóstico manual;
não use reset Git, apague evidências ou renomeie outro checkout sobre o ativo.

Sem `--host-source`, o backup não tem `host.json`: seu restaurador recupera
somente arquivos da integração e deixa os serviços parados. Não pode recuperar
uma versão antiga de tela/host que nunca foi preservada.

<a id="orca-selection"></a>
## Seleção no Orca

Mantenha `T{next_extruder} B{next_extruder+1}` no campo de troca de ferramenta.
O parâmetro explícito `B` seleciona a integração a qualquer momento. Quando
`BMCU_PRINT_COMMIT` ativa um mapa, `T0` a `T3` automáticos selecionam as
ferramentas lógicas 1 a 4 por esse mapa, incluindo um canal físico remapeado.
Ferramentas lógicas ausentes falham antes de selecionar uma origem sem relação.
Selecionar novamente uma ferramenta carregada não pede purga adicional.

Sem mapa ativo, comandos sem `B` mantêm o comportamento nativo. O `T0` nativo
seleciona alimentação externa; use `BMCU_TOOL_CHANGE TOOL=0` para solicitá-la
explicitamente com um mapa ativo. Não é necessário pós-processamento do fatiador.

<a id="private-backup-and-recovery"></a>
## Backup privado e recuperação

Snapshots pessoais e exportações do fatiador são intencionalmente excluídos das
releases públicas. Guarde configurações calibradas, estado, ambientes de serviço
e tokens em armazenamento privado. Remover arquivos da árvore atual não apaga
suas versões históricas no Git. Troque quaisquer credenciais expostas.

Crie um arquivo privado com a impressora parada:

```sh
sudo python3 scripts/ender3-v3-se/capture_configuration.py
```

O coletor registra configuração, calibração, estado BMCU, módulos instalados,
revisão das fontes, patches locais, artefatos de firmware disponíveis e metadados
dos serviços. Não envia arquivos, move motores ou reinicia serviços. Rejeita
arquivos que mudem durante a coleta. Resolva os avisos de arquivos ausentes
antes de confiar no backup para recuperação.

Verifique e extraia para um diretório novo de revisão:

```sh
python3 scripts/ender3-v3-se/verify_backup.py backup.tar.gz --extract recovery
```

O formato do arquivo mantém os nomes de campos e aliases CLI originais para
compatibilidade. Restaure pacotes do sistema, revisões das fontes e serviços
antes de copiar configurações revisadas para uma instalação existente.
Não use PIDs de execução salvos como prova de processo ativo, nem estado
`LOADED` salvo como prova de filamento ainda no bico.

O helper avançado `restore_snapshot.py` exige um diretório privado `--snapshot`
explicitamente fornecido, no formato de manifesto com checksums. Nenhum snapshot
pessoal é incluído ou selecionado por padrão. Primeiro prepara um plano e só
aplica com `--apply`; não é um instalador de imagem SD. Arquivos das fontes e
versões informadas pela MCU não provam, sozinhos, qual binário está gravado.

<a id="validation"></a>
## Validação

```sh
python3 -m unittest discover -s test/bmcu_integracao -p 'test_*.py'
python3 -m unittest discover -s test/e3v3se_display -p 'test_*.py'
```

Os testes cobrem seleção de rota, contratos de captura/retirada, recuperação
de falhas, leitura de configuração, integridade de backups e restauração com
dados sintéticos. O comissionamento físico completo continua necessário em
uma instalação nova.
