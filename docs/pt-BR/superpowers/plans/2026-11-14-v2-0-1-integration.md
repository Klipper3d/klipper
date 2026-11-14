# Plano de implementação da release 2.0.1

[English](../../../superpowers/plans/2026-11-14-v2-0-1-integration.md) | **Português brasileiro**

> Executar tarefa por tarefa. O principal implementa; revisão final somente leitura.

**Objetivo:** entregar o escopo aprovado da 2.0.1 sem implantação ou recuperação de energia.

**Arquitetura:** fixar e testar contratos reais da fonte BMCU 1.1.2, reutilizar
reposição e proteções nativas de transporte e compilar sobreposição mínima do
Mainsail 2.19.0 com a conexão Moonraker/Vuex existente. Instalador do painel separado.

**Tecnologias:** Python 3, Klipper, unittest, Vue 2.7, TypeScript, Vuetify 2,
Mainsail 2.19.0, Vitest e verificação no navegador.

**Especificação:** `../specs/2026-11-14-v2-0-1-integration.md`.

<a id="global-constraints"></a>
## Restrições globais

- Release 2.0.1; BMCU-Klipper 1.1.2; firmware 1.0.4; Mainsail 2.19.0.
- Preservar a auditoria concluída de limpeza/segurança e calibrações particulares.
- Usar fim de filamento e Retomar nativos; não simular estados físicos das rotas.
- Sem acesso/implantação/aquecimento/movimento/flash; sem recuperação de energia.
- API inglesa e documentação/textos de interface completos EN/PT-BR.

<a id="review-focus"></a>
## Foco da revisão

- Firmware antigo, métodos ausentes e contratos alterados falham de forma segura.
- Reposição não pode pular carga, voltar cedo à peça ou perder arquivo/mapa.
- Entrada de metadados não pode injetar G-code ou disputar uma operação ativa silenciosamente.
- Dados offline/antigos/incertos não podem aparecer como carga confirmada.
- Falha de instalação/rollback do painel preserva arquivos alheios e ajustes particulares.

<a id="task-1-bmcu-112-and-confirmed-runout-replacement"></a>
### Tarefa 1: BMCU 1.1.2 e reposição confirmada

**Arquivos:** módulos de integração/medição, adaptador da tela, instalador,
macros validadas, testes responsáveis e fixtures nativas versionadas em
`test/bmcu_integracao/fixtures/`.

- [x] Comparar contratos reais da 1.1.2 e proteção de transporte com os patches instalados.
- [x] Criar testes falhando para versões/firmware e métodos nativos reais ajustados.
- [x] Adaptar callbacks Ender necessários. Manter a verificação explícita do histórico:
  contador aleatório não é verificação equivalente; reiniciar pelos helpers nativos.
- [x] Exercitar reposição nativa por pausa/retomada reais e callback Ender; verificar
  arquivo/mapa/posição preservados, limpeza concluída antes da retomada e falhas pausadas.
- [x] Ampliar leitura de disponibilidade/reposição na tela sem UART na renderização.
- [x] Rodar todas as suítes Python e preservar regressões de rollback/calibração.

<a id="task-2-native-dashboard-card-and-controlled-installation"></a>
### Tarefa 2: Cartão nativo e instalação controlada

**Arquivos:** sobreposição em `integrations/mainsail/`, scripts de aplicação/
compilação em `scripts/ender3-v3-se/`, testes de unidade/navegador e guia EN/PT-BR.

- [x] Inspecionar layouts, assinaturas, diálogos e rolos do Mainsail versionado.
- [x] Criar testes falhando para normalização, comandos seguros, proteções offline/
  ocupado, registro no layout e validação/rollback do instalador.
- [x] Implementar cartão de quatro rolos, layout integrado ao tema, diálogo e textos EN/PT-BR.
- [x] Compilar de forma reproduzível a partir da revisão upstream fixa, mantendo licenças.
- [x] Implementar instalador separado com destino explícito, backup, validação do
  artefato e rollback; sem implantação remota automática.
- [x] Rodar testes/lint/tipos/build e inspecionar estados no navegador desktop/mobile.

<a id="task-3-release-assembly-and-final-validation"></a>
### Tarefa 3: Montagem da release e validação final

**Arquivos:** `VERSION`, par de notas, guias/READMEs/documentação do perfil
atuais, workflow CI e os pares de plano/especificação da release.

- [x] Atualizar versões suportadas sem reescrever notas históricas de releases.
- [x] Documentar manutenção separada de pacote/firmware e preservação da limpeza.
- [x] Preparar notas 2.0.1, artefato/hashes/procedência do painel e verificações CI.
- [x] Rodar suítes Python/UI completas, checker documental estrito, estilo LF completo,
  build/configuração-alvo e revisar o diff Git nativo inteiro.
- [x] Obter revisão final externa GPT de todas as mudanças anteriores/novas; corrigir
  achados concretos e repetir verificações antes de qualquer publicação da release.

<a id="validation-checkpoint"></a>
## Checkpoint de validação

- Python: passaram 299 testes de integração, 74 da tela e 14 de documentação.
- Passaram comparação da fixture de reposição upstream, compilação/idempotência
  dos patches na fonte completa e parada nativa de processo verificado, sem UART.
- Mainsail: passaram 61 testes, lint, formatação e build de produção/TypeScript/PWA.
- Navegador local: passaram 20 casos de idioma/tema/estado, edição pelo teclado,
  gravação, dados antigos/falha e largura mobile, somente com estado simulado.
- Documentação estrita: 81 páginas inglesas, sem pares PT-BR faltantes ou erros.
- Passaram estilo LF público completo, build GD32F303/dicionário da ponte e
  configuração Klippy. O firmware local é build de validação, não binário de release.
- Cartão candidato: `ci_build/ender-201-verified/mainsail-bmcu.zip`; manifesto,
  licenças e fingerprints canônicos conferidos; entrada do simulador excluída.
- Nesse checkpoint, as mudanças estavam locais e sem commits; jobs/publicação GitHub
  ainda não tinham rodado. Manutenção de pacote/firmware e comissionamento físico
  permanecem separados. Recuperação de energia adiada.
- A revisão externa foi refeita com sucesso. Seu achado P2 na transição avanço/purga
  foi reproduzido com regressão nativa de reposição falhando: o alimentador parado
  podia ser rearmado antes da purga. A primeira leitura da purga agora exige
  assistência já ativa; a regressão confirma ausência de extrusão/limpeza adicional
  e preserva arquivo pausado, mapa e pedido de reposição. A suíte de 299 testes passou.
  O mesmo revisor aprovou o escopo de software após confirmar a correção; CI remota
  e aceitação física permanecem limites de validação separados.
