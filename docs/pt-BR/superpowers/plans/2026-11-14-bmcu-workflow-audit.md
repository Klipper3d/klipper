# Plano de implementação da auditoria do fluxo BMCU

[English](../../../superpowers/plans/2026-11-14-bmcu-workflow-audit.md) | **Português brasileiro**

> Executar tarefa por tarefa. O principal edita; a revisão final é somente leitura.

**Objetivo:** melhorar o fluxo mantido de troca e implementar a limpeza confirmada
de seis ciclos X=-6/-3 sem enfraquecer verificações de qualidade ou segurança.

**Arquitetura:** manter a separação entre integração/cortador/medição, callbacks
nativos BMCU e preservação existente do instalador. Usar bancadas unittest e
evidências de tempo atuais, sem criar outro caminho de transporte ou movimento.

**Tecnologias:** Python 3, APIs Klipper de movimento/G-code, Jinja e unittest.

**Especificação:** `../specs/2026-11-14-bmcu-workflow-audit.md`.

<a id="global-constraints"></a>
## Restrições globais

- Limpeza X: -6 a -3, seis idas e voltas completas e passagem final; Y=123.
- Preservar 35 mm de purga e ajustes calibrados de temperatura/extrusão.
- Preservar verificações de sensor/encoder/sessão, homing X primeiro, proteções
  de servo/PWM, cancelamento, recuperação e coordenadas de máquina.
- Não implantar, aquecer, mover nem reconciliar rotas na impressora.
- API pública inglesa; documentos EN/PT-BR; preservar ajustes ao atualizar.

<a id="review-focus"></a>
## Foco da revisão

- Mudanças de sessão, sensor ou temperatura no fim da purga não podem causar
  um falso sucesso da carga.
- Retirar uma espera não pode eliminar uma inspeção física nem permitir UART
  concorrente dentro de uma seção crítica de movimento.
- Limpeza ampliada não pode alterar Y/Z nem ignorar limites independentes dos
  offsets de G-code.
- Coordenadas e quantidade de ciclos personalizados devem sobreviver à instalação.
- Estimativas de tempo/material não podem ser apresentadas como medições reais.

<a id="task-1-read-the-maintained-workflow-and-establish-evidence"></a>
### Tarefa 1: Ler o fluxo mantido e estabelecer evidências

**Arquivos:** `integrations/bmcu/bmcu_ender_integration.py`,
`integrations/bmcu/bmcu_cortador_ender.py`,
`integrations/bmcu/bmcu_measurement.py`,
`config/ender3-v3-se-validada/`, `profiles/orca/ender3-v3-se-bmcu/`,
`scripts/ender3-v3-se/instalar_multicor_bmcu.py` e testes existentes.

- [x] Executar as suítes completas de integração/tela/documentação como base.
- [x] Ler integralmente integração, cortador e medição; seguir callbacks nativos,
  configurações relevantes e os caminhos de preservação do instalador.
- [x] Registrar neste plano trechos inspecionados, achados concretos e atalhos
  inseguros rejeitados. Consultar o Klipper versionado sobre sincronização.
- [x] Para cada falha verificada ou repetição removível, criar uma regressão
  focada que falhe no módulo de teste responsável antes de alterar implementação.

<a id="task-2-confirmed-wipe-defaults-and-preservation"></a>
### Tarefa 2: Padrões confirmados de limpeza e preservação

**Arquivos:** construtor da integração e `wipe()`,
`config/ender3-v3-se-validada/bmcu-multicor-ender.cfg`,
`test/bmcu_integracao/test_cutter_safety.py`,
`test/bmcu_integracao/test_integracao.py`.

- [x] Criar teste que falhe exigindo `wipe_x=-3`, `wipes=6`, mantendo
  `purge_x=-6`, `purge_y=123` e `purge_mm=35`.
- [x] Testar movimentos em coordenadas de máquina: exatamente seis idas e voltas
  e passagem final, Y/Z inalterados e overrides de impressão preservados.
- [x] Testar preservação com coordenadas/ciclos personalizados e confirmar que
  a preparação não os substitui pelos novos padrões.
- [x] Observar as falhas esperadas, alterar somente os padrões relevantes e
  aprovar os testes focados. Não reescrever configurações particulares preservadas.

<a id="task-3-reproduce-and-correct-workflow-findings"></a>
### Tarefa 3: Reproduzir e corrigir achados do fluxo

**Arquivos:** funções existentes de integração/cortador/medição apontadas na
Tarefa 1 e os módulos de regressão correspondentes em `test/bmcu_integracao/`.

- [x] Simular falha de sessão/sensor no último trecho da purga e na limpeza;
  exigir que não haja movimento posterior nem sucesso após detectar a falha.
- [x] Testar cada redução proposta de sincronização contra contagens de comandos,
  ordem de movimentos, distância máxima de extrusão e verificações existentes de
  seção crítica UART; rejeitar propostas que alterem inspeções/cancelamento.
- [x] Implementar somente correções reproduzidas ou trabalho comprovadamente
  redundante. Manter taxas físicas, volume de purga e atrasos calibrados.
- [x] Reexecutar a integração completa após cada grupo de correções.
- [x] Relatar reduções reais de comandos/sincronizações separadas das estimativas
  de tempo mecânico; não afirmar desempenho físico sem medição.

<a id="task-4-documentation-full-validation-and-final-review"></a>
### Tarefa 4: Documentação, validação completa e revisão final

**Arquivos:** guias operacionais afetados e pares PT-BR, este plano/especificação
e seus pares. Não colocar relatórios de processo nos READMEs do projeto/perfil.

- [x] Documentar limpeza ampliada, percurso/tempo adicional e preservação de
  ajustes existentes, com checklist manual de comissionamento no guia operacional.
- [x] Executar todos os testes de integração/tela/documentação, checker estrito,
  diff Git nativo e verificador original completo de estilo em export público LF.
- [x] Compilar o alvo e validar configuração se as mudanças afetarem suas entradas;
  isso não substitui regressões nem aceitação física.
- [x] Obter revisão final externa GPT de todo o escopo, corrigir achados concretos
  e repetir validações afetadas antes da entrega.

<a id="audit-evidence"></a>
## Evidências da auditoria

<a id="inspected-boundary"></a>
### Escopo inspecionado

- Lidos integralmente integração (linhas originais 1–1815), cortador (1–400) e
  medição (1–736), incluindo temporização de captura, diagnósticos assistidos,
  contextos dos callbacks nativos, formação de ponta, retirada com encoder/buffer,
  estacionamento, cancelamento manual, recuperação pausada e reset. Medição e
  cortador não precisaram de alterações para os achados reproduzidos.
- Lidos os oito arquivos de configuração validada e snippets de início/troca/fim
  e ajustes do Orca. O teste existente do projeto de recarga inspeciona os ajustes
  embutidos e todas as 16 peças do 3MF; geometria e perfis permanecem inalterados.
- Seguidos preparação do instalador, patches nativos de captura/retirada/contrato,
  includes ativos, preservação da sequência/ponta/cortador/painel, ativação e
  rollback. Lido `integrations/bmcu/ender_transport.py`; proteções de transporte
  permanecem inalteradas.
- Consultados movimentos/esperas em `klippy/toolhead.py` e a visão geral oficial
  do código Klipper. Context7 indisponível por cota. A
  [implementação versionada do dispositivo BMCU](https://github.com/jarczakpawel/BMCU-Klipper/blob/v1.0.6/klippy/extras/bmcu_core/device.py)
  confirma que cada reenvio de modo é uma solicitação com confirmação, não uma
  operação local sem efeito. Trechos dos callbacks nativos e testes existentes
  de recuperação/homing orientaram o rastreamento do fluxo mantido; isso não
  certifica todo o upstream.

<a id="reproduced-findings-and-corrections"></a>
### Achados reproduzidos e correções

- As verificações antigas só ocorriam antes dos incrementos de purga. Injetar
  reinício, ausência no canal, switch de entrada livre ou extrusora fria no último
  incremento ou na limpeza produzia falso sucesso. Regressões agora verificam as
  13 fronteiras da limpeza, inclusive a passagem final, e a pausa de acomodação.
  O cancelamento manual pela tela nessa pausa também bloqueia novos movimentos.
- Reenviar assistência a cada trecho podia reiniciar um alimentador parado
  inesperadamente. A carga agora entra no acompanhamento uma vez, verifica cada
  leitura atualizada e a fronteira final do avanço ao bico. A retirada inicia a
  assistência na fase responsável; o helper existente continua verificando modo,
  sessão, sensor, encoder e buffer. A parada/reinício periódico intencional da
  retirada permanece intacta.
- Novos padrões do construtor/template correspondem às seis idas e voltas e
  passagem final solicitadas. Testes com o estado real de G-code do Klipper
  cobrem offsets, overrides de velocidade/fluxo e Y/Z estacionários; calibrações
  explícitas e coordenadas/ciclos preservados pelo instalador não mudam.
- Cada comportamento alterado passou primeiro por regressões falhando. A base
  era 253 testes de integração + 71 de tela + 14 documentais. A integração
  passou a 267 testes aprovados, incluindo injeções de falha parametrizadas e
  verificações antes de armar a assistência ao bico após a captura e após a resposta.

<a id="optimization-accounting-and-rejected-shortcuts"></a>
### Contabilidade da otimização e atalhos rejeitados

- Com avanço ao bico de 80 mm em trechos de 10 mm, solicitações de modo caem
  de nove para duas no avanço e purga: sete solicitações redundantes removidas.
- Na retirada sintética com cortador e sensor liberando aos 66 mm, saem 33
  solicitações de modo redundantes. A bancada cai de 37 para quatro, contando
  sua solicitação inicial de preparação; permanecem início de fase e dois
  reinícios intencionais. O percurso continua em 66 incrementos monitorados de 1 mm.
- Na bancada sem cortador, formação/retirada cai de 25 para três solicitações,
  também contando a preparação inicial: 22 solicitações redundantes removidas.
  Os 146 incrementos de extrusão e recuo líquido de 66 mm não mudam. São contagens
  sintéticas de testes, não tempos medidos na impressora nem sugestão de calibração.
- Nenhuma espera de conclusão, pausa de um segundo da purga, acomodação do servo,
  transição de sensor, janela de captura ou inspeção do encoder foi retirada.
  Agrupar submovimentos de extrusão alteraria a temporização das verificações
  térmicas do Klipper; ampliar os trechos de retirada enfraqueceria as fronteiras
  de folga/progresso. Ambas as propostas foram rejeitadas.
- O volume de purga permanece em 35 mm. Ainda não há economia comprovada de
  material; evitar novas tentativas ou purga presa continua sendo meta física.
- A limpeza cresce de 14 para 39 mm: o percurso ideal a F600 passa de 1,4 para
  3,9 s. As leituras adicionais de segurança também levam tempo. Nem as
  solicitações removidas nem essas estimativas comprovam troca total mais rápida,
  desprendimento da purga ou qualidade de impressão.

<a id="validation-results"></a>
### Resultados da validação

- 267 testes de integração + 71 de tela + 14 documentais aprovados: 352 no total.
- Checker documental estrito: 77 páginas inglesas, nenhum par PT-BR ausente e
  nenhum erro estrutural. Verificações do diff Git nativo aprovadas.
- Verificador original completo de estilo aprovado em um novo export público LF.
  A primeira execução apontou uma linha nova de teste longa, já corrigida.
- Alvo GD32F303 compilado; ponte da tela, tamanho dos trechos e comandos/respostas
  do dicionário conferidos. O teste de configuração Klippy da tela original passou
  com esse dicionário. A primeira tentativa na pasta de trabalho encontrou a
  limitação de espaços no caminho da compilação do C-helper upstream; executar
  no export público sem espaços passou. Nenhum contorno upstream foi adicionado.
- Validação de importação do host em Python 3 aprovada. Fontes MCU, dependências
  e configuração-alvo não mudaram; a matriz CI completa de múltiplos alvos em
  Python 2/3 não foi reexecutada localmente. Não houve acesso à impressora,
  implantação ou teste físico.
- A revisão final externa cobriu os 12 arquivos alterados/novos e reproduziu
  mais um achado P2: a perda inesperada de assistência nas fronteiras de reinício
  periódico podia ser ocultada pelo próprio reinício. As regressões falharam
  primeiro nas fronteiras sintéticas de 26 mm e 52 mm. O helper agora verifica
  o modo atualizado após cada incremento concluído enquanto o switch de entrada
  continua ocupado, antes de qualquer reinício periódico. As contagens saudáveis
  de reinício e todos os 352 testes continuam aprovados. O agente principal
  conferiu essa correção pontual sem nova chamada de revisão; ela não acrescenta
  movimento, API ou escopo. A aceitação física continua pendente.
