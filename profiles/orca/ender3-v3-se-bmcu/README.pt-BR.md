# OrcaSlicer 2.4.2 — Ender 3 V3 SE com BMCU

[English](README.md) | **Português brasileiro**

<a id="v2-profile-re-slice-existing-jobs"></a>
<a id="setup"></a>
## Configuração

Use estes trechos com a [integração por switch](../../../docs/pt-BR/Ender3_V3_SE_Configuracao_Validada.md).
Copie-os para um perfil de impressora do usuário e substitua `YOUR_BMCU_UID`
em `start.gcode` pelo UID informado pela sua BMCU.

| Ajuste da impressora | Valor |
| --- | --- |
| G-code inicial | Conteúdo de `start.gcode` |
| G-code final | Conteúdo de `end.gcode` |
| G-code de troca de filamento | Conteúdo de `change.gcode` |
| Pausa | `PAUSE` |
| Firmware | Klipper |
| Extrusoras físicas | Uma, com multimaterial de extrusora única habilitado |
| Trocas manuais e torre de purga | Desabilitadas na montagem testada com calha e limpador |
| Retração de troca, tubo de resfriamento e carga adicional | Zero; a integração controla essas fases |
| Ramming e movimentos de resfriamento | Desabilitados |

Salve esses ajustes como um perfil de impressora do usuário. Não acrescente
outro homing, sequência de temperatura ou linha de purga depois de
`ENDER_START_MULTICOLOR`. Não use `M600` nas trocas multicoloridas da BMCU.

O macro inicial usa `BED_TEMP`, `NOZZLE_TEMP` e `TOOL`; o macro final é
`ENDER_END_MULTICOLOR`.

| Slot Orca | Comando explícito | Canal físico padrão | Índice da API BMCU |
| --- | --- | --- | --- |
| 1 | `T0 B1` | 1 | 0 |
| 2 | `T1 B2` | 2 | 1 |
| 3 | `T2 B3` | 3 | 2 |
| 4 | `T3 B4` | 4 | 3 |

Com o mapa de impressão confirmado, a integração também aceita os comandos
automáticos `T0` a `T3`, sem `B`, como ferramentas lógicas 1 a 4. A BMCU
determina suas rotas físicas. Fora desse contexto, as ferramentas nativas
mantêm o significado original; `T0` seleciona a alimentação externa. Uma
ferramenta ausente em um mapa ativo causa erro, em vez de selecionar outra
origem. Selecionar novamente uma ferramenta carregada não gera outra purga.

Mantenha `T{next_extruder} B{next_extruder+1}` no campo personalizado de troca.

`example-settings.json` fornece um ponto de partida para PETG: 230 °C,
mesa a 80/75 °C, PA 0.04, fluxo 1.00 e limite volumétrico de 20 mm³/s no
fatiador. Calibre material e fluxo para seu próprio hotend; input shaping e
ajuste da velocidade máxima são tarefas separadas.

<a id="four-channel-reload-project"></a>
## Projeto de recarga dos quatro canais

[`bmcu_four_channel_reload_test.3mf`](bmcu_four_channel_reload_test.3mf) contém
quatro ciclos de duas camadas por canal: 16 cargas, 15 trocas e três recargas
por canal. Abra-o como projeto, revise todos os ajustes, substitua o UID de
exemplo e fatie com este perfil. Ele não contém G-code pronto para impressão.

Para atualizar perfis, veja as [notas da release v2.0.0](../../../docs/pt-BR/releases/v2.0.0.md#breaking-change).
