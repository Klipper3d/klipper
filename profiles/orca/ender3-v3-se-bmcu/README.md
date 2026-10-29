# OrcaSlicer 2.4.2 — Ender 3 V3 SE com BMCU

Estes arquivos foram extraídos do projeto da plaquinha Falcon 9 em PETG preto
no canal físico 2 e branco no 4. O proprietário confirmou que a impressão e a
troca de cores funcionaram. Eles dependem da integração descrita em
[Configuração validada](../../../docs/Ender3_V3_SE_Configuracao_Validada.md).

`projeto-validado.json` conserva as configurações globais extraídas do 3MF;
`ajustes-validados.json` destaca os ajustes de comunicação, troca e filamento.
São referências de configuração, não arquivos nativos de preset importáveis
diretamente no Orca. Não incluem geometrias nem configurações por objeto do 3MF.
Para recuperar todos os presets pessoais e suas heranças no computador, exporte
também o pacote de configurações do próprio Orca. Esse pacote ainda está pendente.

No perfil da impressora, use:

| Campo | Arquivo/valor |
| --- | --- |
| G-code inicial | Conteúdo de `inicio.gcode` |
| G-code final | Conteúdo de `fim.gcode` |
| Troca de filamento | Conteúdo de `troca.gcode` |
| Pausa | `PAUSE` |
| Firmware | Klipper |
| Extrusoras físicas | Uma, com troca multimaterial |
| Troca manual e torre de purga | Desativadas neste perfil testado |
| Retração de troca, cooling tube e carga extra do fatiador | Zero; integração controla a troca |
| Ramming e cooling moves | Desativados |

Salve como um perfil de usuário para reutilizar em outras peças. Não mantenha
o início antigo com outro `G28`, temperaturas fixas ou segunda linha de purga
depois de `ENDER_INICIAR_MULTICOR`. Não use `M600` como troca multicolor.

| Slot no Orca | G-code | Canal físico BMCU | Índice da API BMCU |
| --- | --- | --- | --- |
| 1 | `T0 B1` | 1 | 0 |
| 2 | `T1 B2` | 2 | 1 |
| 3 | `T2 B3` | 3 | 2 |
| 4 | `T3 B4` | 4 | 3 |

O parâmetro `B` seleciona explicitamente o adaptador desta integração. Mantenha
`T{next_extruder} B{next_extruder+1}` no campo de troca de filamento.

O Orca 2.4.2 também pode acrescentar um `T0` automático quando a impressão usa
somente o primeiro slot, sem passar pelo G-code personalizado de troca. Com
`BMCU_PRINT_COMMIT` concluído e o mapa de impressão ativo, a integração interpreta
`T0` a `T3` sem `B` como ferramentas lógicas 1 a 4 desse mapa. A BMCU continua
resolvendo o canal físico, inclusive quando ele foi remapeado; um slot ausente
do mapa produz erro antes de selecionar outra entrada. Repetir a ferramenta
já carregada não solicita uma purga adicional.

Sem mapa de impressão ativo, os comandos sem `B` mantêm o comportamento nativo:
`T0` seleciona a entrada externa reservada. Para selecionar explicitamente essa
entrada durante um mapa ativo, use `BMCU_TOOL_CHANGE TOOL=0`. Não é necessário
instalar scripts de pós-processamento no PC nem no notebook. Remova o script
`corrigir_canais_bmcu.ps1` do perfil de processo caso tenha sido configurado.

O perfil testado usa PETG a 230 °C, mesa 80/75 °C, PA 0,04, fluxo 1,00 e limite
volumétrico de 20 mm³/s. O limite não demonstra a capacidade máxima do hotend.
Input shaping e testes adicionais de velocidade continuam pendentes.

O G-code efetivamente impresso tinha SHA256
`2fe7061adc4d512dc041cec0fc8b6b51bd35244f6dcf15266c5e03cd999c965f`.
