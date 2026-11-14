# Identificadores de operação de filamento e recuperação da impressão

[English](../Ender3_V3_SE_Print_Recovery.md) | **Português brasileiro**

A integração opcional exige BMCU-Klipper 1.1.2 e firmware 1.0.4.
A manutenção de pacote/firmware antigo é separada; veja os
[limites da atualização 2.0.1](releases/v2.0.1.md#upgrade-and-rollback).
O instalador Ender altera apenas arquivos do host, sem gravar MCUs.

<a id="cached-operation-results-after-a-host-restart"></a>
## Resultados de operações em cache após reiniciar o host

O daemon de transporte originalmente reiniciava seu contador de comandos no
protocolo em 1. O controlador retém quatro resultados de operações concluídas
entre handshakes de sessão do host. Reutilizar um identificador retido pode
reproduzir um resultado antigo em vez de iniciar a operação solicitada,
incluindo um sucesso ou aborto antigo.

O `EnderTransportGuard` instalado primeiro solicita o estado recente do
controlador e exige uma operação ociosa na mesma sessão. Em seguida, consulta
o histórico do controlador para um identificador candidato. Um identificador
retido é ignorado sem enviar comando de movimento. Somente a resposta explícita
do protocolo indicando resultado indisponível autoriza iniciar a nova operação.
Até quatro resultados retidos podem ser ignorados; respostas malformadas,
controlador ocupado, mudança de sessão, cancelamento ou verificação expirada
interrompem a solicitação.

Isso se aplica às operações nativas de alimentação, retração, teste de encoder,
carga automática e calibração automática. O controle normal de acompanhamento
do buffer durante a impressão permanece inalterado. A guarda não repete
cegamente uma carga ou retirada física.

A BMCU 1.1.2 aleatoriza o contador inicial no protocolo. A verificação explícita
do histórico permanece: aleatoriedade não comprova que um candidato está livre.

<a id="manual-replacement-after-runout"></a>
## Reposição manual após fim de filamento

Com reposição automática desabilitada, a BMCU nativa registra a reposição
pendente no mesmo canal e pausa a impressão. Tela original e
[cartão Mainsail](Ender3_V3_SE_Mainsail.md) mostram o canal físico a repor.
Insira a reposição e use **Retomar** normal ou `RESUME`; não limpe o mapa,
resete rotas ou faça carga manual separada nessa pausa. O wrapper nativo
verifica identidade, endpoint, pausa, rotas reconciliadas, presença da reposição
e contexto da ponta destacada antes de carregar. O callback Ender conclui
captura, avanço ao bico, purga e limpeza antes de a pausa/retomada nativa voltar
à peça. Falha mantém solicitação pendente e arquivo pausado. A chegada física
e a extrusão ainda precisam de teste supervisionado.

<a id="failed-tool-changes-during-printing"></a>
## Falhas de troca de ferramenta durante a impressão

Quando `BMCU_CHANGE_TOOL_ENDER` falha durante a execução de um comando de
impressão por SD virtual, a integração para o alimentador e pausa o arquivo
sem propagar o erro de filamento para `virtual_sdcard.on_error_gcode`.
O arquivo permanece selecionado, a próxima linha de G-code não é executada
e o mapeamento lógico de ferramentas é retido. A posição original de impressão,
o estado de coordenadas/extrusão e o alvo do bico são salvos separadamente
da posição de purga.

Após verificar o caminho do filamento, use o botão normal **Retomar impressão**
ou `RESUME`. A troca pendente é concluída na posição de purga antes da retomada
normal. Um alvo já carregado passa novamente por purga/limpeza. A temperatura
original e o estado salvo da impressão são restaurados antes de retornar à peça.
Se a tentativa de recuperação falhar, o arquivo permanece pausado com a mesma
ferramenta pendente.

O objeto de status `bmcu_ender_integration` expõe `recovery_pending`,
`recovery_tool`, `recovery_channel` e `recovery_error`, além de `manual_busy`,
`unload_timing`, `cutter_uncertain` e `last_cut`. A tela usa esse estado existente
em vez de reconstruir ou limpar uma sessão de impressão em um fluxo separado.

Uma impressão pausada com troca pendente oferece **Recuperar filamento**.
O menu de recuperação mostra o canal físico da falha e **Detalhes da falha**.
**Confirmar caminho livre** exige confirmação explícita antes de chamar o
handler protegido para aquele canal. **Retomar impressão** e **Cancelar
impressão** mantêm as proteções de recuperação e cancelamento existentes.
Esse menu não oferece um **Reset BMCU** global.

Se o cortador estiver incerto, **Fechar batente** exige a confirmação existente
de folga do cabeçote e envia somente:

```gcode
CUTTER_SERVO POSITION=RETRACTED CONFIRM=1
```

O fluxo retorna à recuperação sem homing, movimentos manuais dos eixos ou
abertura do batente. Os dois idiomas da tela usam os mesmos comandos em inglês.
Altere o idioma em **Configurações → Idioma**; salvar é um reinício separado
da configuração, protegido por confirmação.

A recuperação é bloqueada se o arquivo ou sua posição lógica mudou, uma rota
está incerta, sensor e estado de rota discordam, outra operação está ativa,
o controlador está indisponível, as coordenadas foram alteradas, os motores
foram desativados ou a máquina foi referenciada novamente. Uma parada de
firmware ou perda de posição mecânica não pode ser reparada por este mecanismo
de pausa. O comportamento existente de tempo ocioso é preservado; não deixe
uma recuperação pendente sem supervisão até que os motores sejam desativados.
O sensor de entrada, sozinho, não confirma extrusão física no bico.

<a id="confirming-a-manually-cleared-path-during-a-paused-recovery"></a>
## Confirmar um caminho liberado manualmente durante a recuperação pausada

Para uma rota incerta, remova fisicamente qualquer obstrução do caminho
compartilhado de PTFE/extrusora e estacione o filamento desse canal atrás do
sensor de entrada, livre da junção compartilhada. O sensor deve indicar livre.
O comando abaixo não move filamento; registra a verificação física explícita
do operador:

```gcode
BMCU_CONFIRM_CLEAR_PATH CHANNEL=3 CONFIRM=1
```

Substitua `3` pelo canal físico registrado na falha pausada. Este comando é
restrito a esse canal, à mesma impressão pausada e a um alimentador ocioso.
Rejeita sensor de entrada ativo, outra rota ocupada ou uma cauda destacada.
A releitura da rota pelo firmware é obrigatória antes de liberar o estado
de recuperação do host. O mapa de impressão é preservado. Em seguida, use
`RESUME` para concluir a troca pendente.

Não confirme um caminho vazio apenas para contornar um erro. O switch de entrada
não detecta filamento deixado em outro ponto do caminho compartilhado.
A confirmação normal de rota nativa permanece inalterada fora deste comando
protegido de recuperação de impressão.

Use `RESET_BMCU CONFIRM=1` somente depois de cancelar a impressão e desobstruir
manualmente o hotend/caminho compartilhado. Ele reconcilia o estado das rotas
sem apagar calibração; não é reset de fábrica nem substitui a recuperação de
impressão pausada. A opção da tela fica em **Filamento / BMCU**, fora de uma
impressão ativa/pausada.

`CANCEL_PRINT` e `SDCARD_RESET_FILE` descartam a recuperação pendente. Erros de
carga manual continuam sendo informados normalmente. Outros erros de G-code
mantêm a política configurada de erro de SD virtual. Reiniciar o Klipper descarta
a recuperação de impressão em memória; isto não é recuperação entre reinícios
do controlador ou após falta de energia.

<a id="installation-and-verification"></a>
## Instalação e verificação

Termine ou cancele qualquer impressão, estacione fisicamente os canais e confira
se o sensor de entrada está livre. Use uma fonte separada e limpa para que
o atualizador preserve o checkout anterior inteiro do host/display; não execute
`git pull` na árvore ativa antes. Conclua a manutenção separada de pacote/firmware.
Depois que a v2.0.1 for publicada:

```sh
git clone --branch v2.0.1 --single-branch \
  https://github.com/bernardokcosta/ender3-v3-se-klipper-bmcu.git \
  "$HOME/klipper-v2-source"
sudo python3 "$HOME/klipper-v2-source/scripts/ender3-v3-se/install_multicolor.py" \
  --host-source "$HOME/klipper-v2-source"
```

O instalador preparado preserva todo o host/display antigo, as configurações
afetadas e a calibração existente. Uma falha de inicialização restaura o
checkout e as configurações anteriores, desfazendo somente nomes de callbacks
no estado BMCU atual para não perder novas evidências das rotas. A recuperação
deve validar que o host anterior ficou pronto; caso contrário, os serviços
permanecem parados. Entradas da MCU/compilação e requisitos Python devem
coincidir para esta atualização somente no host; outras mudanças exigem revisão
separada de firmware/ambiente. O instalador não grava MCUs. Consulte o
[guia de instalação](Ender3_V3_SE_Configuracao_Validada.md) para detalhes do rollback.

Ajustes de formação de ponta, PID, Z offset e mesh são preservados. Omita
`--purge-mm` e `--pullback-speed` para manter os valores calibrados atuais;
esses argumentos são substituições explícitas, não etapas obrigatórias da
migração. Verifique uma carga/descarga de canal único e uma impressão multicolor
curta antes de uma impressão longa. Testes automatizados exercitam o dispatcher
real do SD virtual do Klipper, pausa/retomada e restauração do estado G-code,
além de métodos nativos de transporte com histórico simulado do controlador.
Esses testes não substituem a verificação física.

Referências nativas de protocolo e transporte:

- [Daemon de transporte BMCU v1.1.2](https://github.com/jarczakpawel/BMCU-Klipper/blob/v1.1.2/scripts/bmcu_transportd.py)
- [Protocolo BMCU e resultados retidos de operações](https://github.com/jarczakpawel/BMCU-Klipper/blob/v1.1.2/firmware/src/bmcu_protocol.cpp)
