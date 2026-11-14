# Homing do cortador com X primeiro

[English](../../../superpowers/specs/2026-11-13-cutter-homing.md) | **Português brasileiro**

<a id="approved-behavior"></a>
## Comportamento aprovado

O operador confirmou que todo o caminho de homing X está livre com o batente
na posição existente. Nesta máquina, X referencia no fim de curso configurado,
-6 mm. Não envie comando de servo antes/durante X. Após X concluir, recolha
o batente, espere o servo e desabilite PWM. G28 completo segue com Y (mesa)
e Z (vertical). Mantenha torque durante acionamento, curso da lâmina e recuo
do carro; libere PWM somente depois de recolher. Isso não desliga os 5 V.

<a id="implementation-decisions"></a>
## Decisões de implementação

- Reutilizar wrapper G28, handler nativo e rotina de recolhimento existentes.
  Nunca usar G1 sem referência ou posição X forjada.
- Repetir preparação com X primeiro em todo G28 com cortador calibrado
  habilitado. Pedidos por eixo preservam os demais solicitados: G28 X termina
  após recolher, G28 Y adiciona X, G28 Z adiciona X mas mantém exigência nativa
  de Y já referenciado.
- Preservar safe_z_home nativo, com uma elevação inicial Z, ponto de sondagem
  e elevação pós-sondagem. A elevação inicial pode preceder X; nenhum comando
  de servo precede X. Reutilizar continuação home_axes extraída para Y/Z, sem
  duplicar elevação inicial física ao dividir um G28 lógico.
- Abortar etapas seguintes em falha de X, referência ausente, perfil inválido,
  BMCU ocupada ou posição insegura de recolhimento. Manter recuperação manual
  para corte interrompido com posição incerta.
- Homing da tela envia só G28. Abertura/fechamento manuais ainda exigem
  confirmação; nenhum atalho recolhe antes de X.
- Preservar homing automático de carga/retirada manual, preparando G28 do
  cortador antes de assumir a operação, sem contornar o bloqueio.
- Reutilizar soltar_servo_recolhido, padrão 1 em ajustes novos/ausentes;
  preservar 0 explícito para mecanismos que exigem torque recolhido. Perfis
  existentes preservados em 0 precisam mudar para 1 na instalação.
- recolher_sem_referencia não contorna mais o homing X e está obsoleto.

<a id="verification-and-limits"></a>
## Verificação e limites

Testes cobrem homing completo/parcial, chamadas repetidas, nenhuma saída de
servo durante X, isolamento de falhas, tela, tempos PWM e opção de manter PWM.
Rode as duas suítes unittest em Linux/WSL e revisão final externa. Testes não
provam folga física, retenção do batente sem torque ou sucesso do corte.
Nenhum serviço da impressora, configuração remota ou firmware muda nesta tarefa.
