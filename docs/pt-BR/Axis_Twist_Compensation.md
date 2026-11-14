<a id="axis-twist-compensation"></a>
# Compensação de torção do eixo

[English](../Axis_Twist_Compensation.md) | **Português brasileiro**

Este documento descreve o módulo `[axis_twist_compensation]`.

Algumas impressoras podem apresentar uma pequena torção no trilho X, o que pode distorcer os resultados de uma sonda fixada ao carro X. Isso é comum em impressoras com projetos como Prusa MK3, Sovol SV06 etc. e é descrito em detalhes em [viés da localização da sonda](Probe_Calibrate.md#location-bias-check). Isso pode fazer com que operações de sondagem como [Malha da mesa](Bed_Mesh.md), [Ajuste da inclinação dos parafusos](G-Codes.md#screws_tilt_adjust), [Ajuste da inclinação Z](G-Codes.md#z_tilt_adjust) etc. retornem representações imprecisas da mesa.

Este módulo usa medições manuais feitas pelo usuário para corrigir os resultados da sonda. Observe que, se o seu eixo estiver significativamente torcido, é fortemente recomendado corrigi-lo primeiro por meios mecânicos antes de aplicar correções por software.

**Aviso**: Este módulo ainda não é compatível com sondas acopláveis e tentará sondar a mesa sem acoplar a sonda se você o usar.

<a id="overview-of-compensation-usage"></a>
## Visão geral do uso da compensação

> **Dica:** Certifique-se de que os [deslocamentos X e Y da sonda](Config_Reference.md#probe) estejam corretamente definidos, pois influenciam muito a calibração.

<a id="basic-usage-x-axis-calibration"></a>
### Uso básico: Calibração do eixo X

1. Após configurar o módulo `[axis_twist_compensation]`, execute:

```
AXIS_TWIST_COMPENSATION_CALIBRATE
```
Este comando calibrará o eixo X por padrão.

- O assistente de calibração solicitará que você meça o deslocamento Z da sonda em vários pontos ao longo da mesa.
- Por padrão, a calibração usa 3 pontos, mas você pode especificar um número diferente com a opção: `SAMPLE_COUNT=<value>`

1. **Ajuste seu deslocamento Z:** Após concluir a calibração, certifique-se de [ajustar seu deslocamento Z](Probe_Calibrate.md#calibrating-probe-z-offset).
1. **Execute operações de nivelamento da mesa:** Use operações baseadas em sonda conforme necessário, como:

- [Ajuste de inclinação dos parafusos](G-Codes.md#screws_tilt_adjust)
- [Ajuste de inclinação Z](G-Codes.md#z_tilt_adjust)

1. **Finalize a configuração:**

- Faça o homing de todos os eixos e execute uma [malha da mesa](Bed_Mesh.md) se necessário.
- Execute uma impressão de teste, seguida de qualquer [ajuste fino](Axis_Twist_Compensation.md#fine-tuning) se necessário.

<a id="for-y-axis-calibration"></a>
### Para calibração do eixo Y

O processo de calibração para o eixo Y é semelhante ao do eixo X. Para calibrar o eixo Y, use:

```
AXIS_TWIST_COMPENSATION_CALIBRATE AXIS=Y
```
Isto o guiará pelo mesmo processo de medição usado para o eixo X.

> **Dica:** A temperatura da mesa e a temperatura e o tamanho do bico não parecem influenciar o processo de calibração.

<a id="fine-tuning"></a>

<a id="axis_twist_compensation-setup-and-commands"></a>
## Configuração e comandos de [axis_twist_compensation]

As opções de configuração de `[axis_twist_compensation]` podem ser encontradas na [Referência de configuração](Config_Reference.md#axis_twist_compensation).

Os comandos de `[axis_twist_compensation]` podem ser encontrados na [Referência de G-Codes](G-Codes.md#axis_twist_compensation)
