# Auditoria do fluxo BMCU e limpeza ampliada do bico

[English](../../../superpowers/specs/2026-11-14-bmcu-workflow-audit.md) | **Português brasileiro**

<a id="approved-goal"></a>
## Objetivo aprovado

Revisar linha por linha o fluxo mantido de troca de filamento, corrigir falhas
verificadas e retirar trabalho comprovadamente desnecessário. Priorizar trocas
mais curtas e menos descarte sem sacrificar qualidade, calibrações ou segurança.

O usuário confirmou seis idas e voltas completas em X entre -6 e -3, seguidas
da passagem final existente para o lado da limpeza. Y permanece em 123.
Trata-se da limpeza do bico, não de uma alteração do curso de corte da lâmina.
O padrão anterior era três idas e voltas entre -6 e -4 e a passagem final.

<a id="constraints"></a>
## Restrições

- Manter a purga atual de 35 mm, temperaturas, taxas de extrusão e calibrações
  físicas, salvo evidência para um ajuste separado e explícito.
- Preservar ordem do homing, recolhimento/desligamento PWM do servo, limites de
  movimento, verificações de sensor/encoder, identidade da sessão BMCU,
  cancelamento e recuperação da impressão pausada.
- Reutilizar helpers e testes existentes. Não criar outro caminho de troca.
- Não agrupar movimentos eliminando sincronizações necessárias à inspeção do
  estado físico. Não substituir retirada por sensor por um recuo curto fixo.
- Respeitar valores particulares da instalação. Documentar a diferença entre
  os novos padrões e ajustes existentes preservados, sem sobrescrever calibrações.
- API e diagnósticos públicos continuam em inglês; documentação operacional EN/PT-BR.
- Esta auditoria não autoriza implantação, movimento, aquecimento ou reset de
  rotas na impressora. Aceitação física e medições de tempo/qualidade são separadas.

<a id="review-boundary-and-evidence"></a>
## Limite da revisão e evidências

Inspecionar integração, cortador, medição, configurações, entradas do fatiador
e caminhos de preservação do instalador mantidos pelo fork e envolvidos neste
fluxo. Registrar os arquivos lidos e a evidência de cada achado. Não afirmar
que a auditoria torna perfeito todo o Klipper upstream ou comprova uma
velocidade máxima universal.

Para cada mudança, criar regressões que falhem antes da implementação,
preservar coordenadas de máquina e identificar estimativas de tempo ou material
como estimativas, não medições. A limpeza mais ampla e frequente aumenta o
percurso dessa fase; não apresentá-la como ganho de velocidade.
