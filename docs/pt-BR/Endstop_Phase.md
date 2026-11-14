<a id="endstop-phase"></a>
# Estágio da chave de fim de curso

[English](../Endstop_Phase.md) | **Português brasileiro**

Este documento descreve o sistema de fim de curso do Klipper ajustado pela fase do motor de passo. Esta funcionalidade pode melhorar a precisão das chaves de fim de curso tradicionais. É mais útil ao usar um driver de motor de passo Trinamic que possua configuração em tempo de execução.

Uma chave de fim de curso comum tem uma precisão de cerca de 100 microns e cada vez que um eixo é enviado para a posição inicial, a chave pode ser acionada um pouco mais cedo ou um pouco mais tarde. Embora seja um erro relativamente pequeno, pode resultar em artefatos indesejados. Em particular, esse desvio de posição pode ser perceptível ao imprimir a primeira camada de um objeto. Em contraste, os motores de passo comuns podem obter uma precisão significativamente maior.

O mecanismo de fim de curso ajustado pela fase do motor de passo pode usar a precisão dos motores de passo para melhorar a precisão das chaves de fim de curso. Um motor de passo se move passando por uma série de fases até que complete quatro "passos completos". Assim, um motor de passo com 16 micro-passos teria 64 fases e, ao se mover em uma direção positiva, passaria pelas fases: 0, 1, 2, ... 61, 62, 63, 0, 1, 2, etc. Importante, quando o motor de passo posicionar um trilho linear em um ponto específico, a fase do motor deverá ser sempre o mesma. Assim, quando um carrinho acionar a chave de fim de curso, o motor que controla esse carrinho sempre deverá estar na mesma fase. O sistema de fase de fim de curso (endstop phase) do Klipper combina a fase do motor de passo com o acionamento da chave para melhorar a precisão do fim de curso.

Para usar essa funcionalidade, é necessário ser capaz de identificar a fase do motor de passo. Se estiver usando drivers Trinamic TMC2130, TMC2208, TMC2224 ou TMC2660 em modo de configuração durante a execução (ou seja, não em modo autônomo), o Klipper pode consultar a fase do motor de passo a partir do driver. (Também é possível usar esse sistema em drivers de motores de passo tradicionais se for possível reinicializar os drivers de forma confiável - veja abaixo mais detalhes.)

<a id="calibrating-endstop-phases"></a>
## Calibrando fases de fim e curso (endstop phases)

Se estiver usando drivers da Trinamic com configuração em tempo de execução, então você pode calibrar as fases de fim de curso usando o comando ENDSTOP_PHASE_CALIBRATE. Comece adicionando o seguinte ao arquivo de configuração:

```
[endstop_phase]
```

Então REINICIE a impressora e execute um comando `G28` seguido do comando `ENDSTOP_PHASE_CALIBRATE`. Em seguida, mova a cabeça de impressão para um novo local e execute `G28` novamente. Tente mover a cabeça de impressão para várias posições diferentes executando o comando `G28` em seguida. Execute pelo menos cinco vezes o comando `G28`.

Após realizar o procedimento acima, o comando `ENDSTOP_PHASE_CALIBRATE` frequentemente informará a mesma fase (ou quase a mesma) para o motor de passo. Essa fase pode ser salva no arquivo de configuração para que todos os futuros comandos G28 usem essa fase. (Assim, em futuras operações de referenciamento, o Klipper obterá a mesma posição mesmo que o fim de curso dispare um pouco antes ou um pouco depois.)

Para salvar a fase de fim de curso para um motor de passo específico, execute algo como em seguida:

```
ENDSTOP_PHASE_CALIBRATE STEPPER=stepper_z
```

Execute o comando acima para todos os motores de passo que você deseja salvar. Normalmente, você usaria stepper_z em impressoras cartesianas e corexy, e stepper_a, stepper_b e stepper_c em impressoras delta. Finalmente, execute o seguinte comando para atualizar o arquivo de configuração com os dados:

```
SAVE_CONFIG
```

<a id="additional-notes"></a>
### Notas adicionais

* Este recurso é mais útil em impressoras delta e no fim de curso Z de impressoras cartesianas/corexy. É possível usar este recurso nos fins de curso X/Y de impressoras cartesianas, mas isso não é particularmente útil, pois um pequeno erro na posição do fim de curso X/Y é pouco provável que afete a qualidade da impressão. Não é válido usar este recurso nos fins de curso X/Y de impressoras corexy (já que a posição XY não é determinada por um único motor de passo na cinemática corexy). Não é válido usar este recurso em uma impressora que usa um fim de curso Z "probe:z_virtual_endstop" (pois a fase do motor de passo só é estável se o fim de curso estiver em um local estático em um trilho).
* Após calibrar a fase do fim de curso, se o fim de curso for posteriormente movido ou ajustado, será necessário recalibrar o fim de curso. Remova os dados de calibração do arquivo de configuração e repita as etapas acima.
* Para usar este sistema, o fim de curso deve ser preciso o suficiente para identificar a posição do motor de passo dentro de dois "passos completos". Então, por exemplo, se um motor de passo estiver usando 16 micro-passos com uma distância de passo de 0,005mm, então o fim de curso deve ter uma precisão de pelo menos 0,160mm. Se você receber mensagens de erro do tipo "Endstop stepper_z incorrect phase" ("Fim de curso stepper_z fase incorreta") pode ser devido a um fim de curso que não é suficientemente preciso. Se a recalibração não ajudar, então desative os ajustes de fase do fim de curso removendo-os do arquivo de configuração.
* Se estiver usando um eixo Z tradicional controlado por motor de passo (como em uma impressora cartesiana ou corexy) junto com parafusos tradicionais de nivelamento da mesa, também é possível usar este sistema para fazer com que cada camada impressa seja executada em um limite de "passo completo". Para ativar esse recurso, certifique-se de que o fatiador de G-Code esteja configurado com uma altura de camada que seja múltipla de um "passo completo", ative manualmente a opção endstop_align_zero na seção de configuração endstop_phase (consulte [referência de configuração](Config_Reference.md#endstop_phase) para mais detalhes) e nivele novamente os parafusos da mesa.
* É possível usar este sistema com drivers de motor de passo tradicionais (não-Trinamic). No entanto, fazer isso necessário que se garanta que os drivers do motor de passo sejam reiniciados sempre que o microcontrolador é reiniciado. (Se os dois sempre forem reiniciados juntos, então o Klipper poderá determinar a fase do motor de passo rastreando o número total de passos que ele ordenou que o motor de passo se mova.) Atualmente, a única maneira de fazer isso de forma confiável é se tanto o microcontrolador quanto os drivers do motor de passo são alimentados apenas por USB e que essa energia USB seja fornecida por um host rodando em um Raspberry Pi. Nesta situação, você pode especificar uma configuração de mcu com "restart_method: rpi_usb" - essa opção irá fazer com que o microcontrolador sempre seja reiniciado por meio de uma reinicialização de energia USB, que faria que tanto o microcontrolador quanto os drivers do motor de passo sejam reiniciados juntos. Se estiver usando este mecanismo, você precisará configurar manualmente as seções de configuração "trigger_phase" (consulte a [referência de configuração](Config_Reference.md#endstop_phase) para mais detalhes).
