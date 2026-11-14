# Plano de implementação do homing do cortador com X primeiro

[English](../../../superpowers/plans/2026-11-13-cutter-homing.md) | **Português brasileiro**

> **Para agentes:** execute este plano tarefa por tarefa na sessão principal;
> delegue somente a revisão final externa obrigatória. Etapas usam checkboxes.

**Objetivo:** referenciar X sem saída no servo, recolher e liberar o batente,
depois referenciar os demais eixos solicitados sem confirmação na partida.

**Arquitetura:** dividir chamadas ao handler G28 nativo já registrado no wrapper
existente. Reutilizar validação/recolhimento e opção de liberar PWM; a tela
delega homing ao mesmo G28.

**Tecnologias:** Python, dispatcher G-code Klipper/safe_z_home, unittest, WSL.

**Especificação:** `docs/superpowers/specs/2026-11-13-cutter-homing.md`

<a id="global-constraints"></a>
## Restrições globais

- Nenhum comando de servo antes/durante X, incluindo WIDTH=0.
- Usar o fim de curso configurado (-6 mm no alvo), nunca forjar referência.
- Preservar safe_z_home e proteções de ocupação/corte incerto.
- Preservar opção explícita de manter PWM e arquivos de calibração mecânica.
- Não conectar nem mover a impressora física.

<a id="review-focus"></a>
## Foco da revisão

- Falha de X ou handler retornando sem referência deve impedir servo/Y/Z.
- G28 parcial/repetido não deve contornar a preparação com X primeiro.
- Cortador desabilitado/não configurado deve manter G28 original inalterado.
- Falha de recolhimento/PWM deve manter estado incerto e impedir Y/Z.
- Abertura manual/corte interrompido não deve liberar torque antes da hora.

<a id="task-1-x-first-preparation-and-pwm-release"></a>
### Tarefa 1: Preparação com X primeiro e liberação PWM

**Arquivos:**
- Modificar: `integrations/bmcu/bmcu_integracao_ender.py`
- Modificar: `integrations/bmcu/bmcu_cortador_ender.py`
- Modificar: `klippy/extras/safe_z_home.py`
- Modificar: `config/ender3-v3-se-validada/bmcu-cortador.cfg`
- Testar: `test/bmcu_integracao/test_cortador.py`
- Criar teste: `test/bmcu_integracao/test_cutter_homing.py`

**Interfaces:** `validar_homing_cortador(anterior)` envolve G28 nativo;
`recolher_para_homing(perfil)` verifica referência real X e recolhe;
`posicionar_recolhido(perfil)` espera antes de WIDTH=0 opcional.

- [x] Adicionar testes que falhem para G28 completo/parcial/repetido, falha de X/
  referência ausente, ocupado/incerto/desabilitado, perfil inválido, X inseguro,
  falha de recolhimento/PWM e padrão/opção explícita.
- [x] Rodar arquivos unittest específicos no WSL e confirmar falhas esperadas.
- [x] Implementar G28 nativo só X, recolhimento protegido e somente Y/Z restantes
  solicitados, preservando parâmetros não relacionados aos eixos.
- [x] Padrão PWM liberado em 1, preservar 0 explícito e aposentar bypass sem
  referência.
- [x] Preservar carga/retirada manual pelo mesmo G28 antes de assumir a operação
  quando faltam referência/recolhimento; testar carga com partida a frio.
- [x] Atualizar testes obsoletos e rodar a suíte de integração.
- [x] Tratar P1 da revisão final: reproduzir primeiro elevação Z cumulativa em
  homing completo/parcial com posição física rastreada separadamente, depois
  extrair `SafeZHoming.home_axes(gcmd)` para só o primeiro segmento fazer a
  elevação inicial. Preservar chamadas nativas normais e elevação pós-sondagem.

<a id="task-2-display-and-operator-documentation"></a>
### Tarefa 2: Tela e documentação do operador

**Arquivos:**
- Modificar: `klippy/extras/e3v3se_display/__init__.py`
- Modificar: `klippy/extras/e3v3se_display/ui.py`
- Testar: `test/e3v3se_display/test_display.py`
- Modificar: `docs/Ender3_V3_SE_Cortador.pt-BR.md`
- Modificar: `docs/Ender3_V3_SE_Cutter.md`

**Interfaces:** `home_all()` envia somente G28; `solicitar_homing()` chama
diretamente. `mover_batente(recolher)` mantém confirmação manual.

- [x] Alterar o teste da tela para esperar G28 direto mantendo confirmação
  manual de abrir/fechar; observar falha antes de implementar.
- [x] Remover atalho do servo pré-homing e confirmação da tela.
- [x] Documentar X primeiro, folga física, padrões PWM e ajuste obrigatório
  soltar_servo_recolhido=1 na configuração existente preservada.
- [x] Rodar suítes completas com `/opt/ender3-tests/bin/python -B -m unittest
  discover -s test/<suite> -p 'test_*.py'` no WSL e `git diff --check`.
- [x] Pedir `review_final_gpt56` de todos os arquivos alterados/novos, tratar
  achados concretos, repetir validações afetadas e relatar limites físicos.

<a id="validation-outcome"></a>
## Resultado da validação

- Integração: 189 testes; tela: 57 testes aprovados (246 no total).
- `git diff --check` aprovado. Logs negativos já existiam na referência.
- Revisão externa aprovou os 13 arquivos após corrigir P1; o mesmo revisor
  também confirmou 384 cenários equivalentes ao G28 nativo e oito combinações
  de eixos do dispatcher + safe_z_home.
- Nenhuma implantação ou validação física foi feita na impressora.

Nenhum commit planejado sem pedido explícito do usuário.
