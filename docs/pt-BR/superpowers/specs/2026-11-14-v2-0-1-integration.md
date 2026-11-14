# Release 2.0.1: BMCU 1.1.2 e cartão nativo no Mainsail

[English](../../../superpowers/specs/2026-11-14-v2-0-1-integration.md) | **Português brasileiro**

<a id="approved-scope"></a>
## Escopo aprovado

O usuário escolheu uma única release, 2.0.1, com a auditoria de fluxo concluída,
compatibilidade BMCU-Klipper 1.1.2, cartão nativo de quatro rolos no dashboard do
Mainsail e reposição manual confirmada após fim de filamento. Recuperação por
queda de energia fica explicitamente adiada. Não implementar aquecimento,
homing ou impressão automáticos após reinício. Gravar o firmware físico da BMCU
é uma operação separada para depois.

<a id="integration-and-replacement"></a>
## Integração e reposição

- Fixar BMCU-Klipper 1.1.2 e exigir seu firmware 1.0.4. Dispositivo incompatível
  ou indisponível não pode ganhar permissão de movimento por renomear uma versão.
- Inspecionar os métodos upstream reais e versionados e usar fim de filamento,
  ponta destacada, reconciliação de rotas e proteção de IDs nativos sem duplicá-los.
  Manter apenas adaptações específicas da Ender ainda necessárias.
- No fim do filamento, preservar arquivo pausado, mapa, posição e estado de
  extrusão. O operador insere filamento no canal indicado e usa Retomar normal
  na tela original ou no Mainsail. Concluir carga nativa e captura/avanço/purga/
  limpeza da Ender antes de voltar à impressão. Reposição incompleta ou com
  falha mantém a pausa; caminhos incertos exigem revisão física.
- Manter purga de 35 mm, seis ciclos X=-6/-3 e passagem final, Y=123,
  taxas/temperaturas calibradas, verificações de encoder/buffer, homing X primeiro
  do cortador, servo/PWM e recuperação protegida existente da troca de ferramenta.
- Preservar ajustes particulares e backup completo de host/display. Não afirmar
  que rollback do host desfaz atualização separada do pacote externo ou firmware.

<a id="native-mainsail-card"></a>
## Cartão nativo no Mainsail

Criar uma sobreposição pequena e versionada para Mainsail 2.19.0, não outro
aplicativo web nem objeto Happy Hare falso. Reutilizar Panel, ícone de rolo,
diálogo, assinatura Vuex/Moonraker e infraestrutura de comandos. Não adicionar
servidor, consultas UART diretas, injeção HTML ou implementação nova de transporte.

O cartão mostra canais físicos 1–4, rolos coloridos, material e estados carregado/
estacionado/vazio/incerto/offline. Metadados não comprovam carga física. A edição
valida material/cor e envia o comando nativo existente, nunca G-code arbitrário
do usuário. Desabilitar edição com pacote/dispositivo incompatível, desconectado,
ocupado ou impressão ativa/pausada. Manter visualização somente leitura, teclado,
estados identificados além da cor, layout responsivo, tema Mainsail e textos EN/PT-BR.

Fornecer compilação reproduzível, procedência e instalador separado do painel,
invocado explicitamente, com validação, backup e rollback. Não sobrescrever uma
instalação Mainsail alheia silenciosamente nem atualizar firmware automaticamente.

<a id="release-boundary"></a>
## Limite da release

Comandos ingleses e guias EN/PT-BR. Preservar READMEs objetivos, créditos,
licenças e os seis links de modelos. Alterar VERSION para 2.0.1 e preparar
notas, artefatos de compilação validados e verificações CI. Não há autorização
para implantação, movimento, aquecimento, gravação de firmware ou reset de
rotas. Separar validação de software dos testes físicos pendentes.
