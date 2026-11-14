# Formação da ponta do filamento e teste de cargas repetidas

[English](../Ender3_V3_SE_Tip_Forming.md) | **Português brasileiro**

O instalador da integração adiciona `bmcu-formacao-ponta.cfg`. Esse arquivo
editável é preservado nas instalações posteriores. Contém um **perfil inicial
experimental**, não uma calibração fisicamente validada para todo hotend.

As distâncias de captura e avanço ao bico existentes ficam inalteradas.
A purga de exemplo agora é 35 mm, reduzida de 70 mm após um teste observado
com PETG. Após a captura iniciada pela extrusora, o alimentador usa
`MOTION_ON_USE`, o mesmo modo de acompanhamento do buffer usado na impressão,
em vez de entrar repetidamente no modo de empurrar anterior à captura.

<a id="unload-sequence"></a>
## Sequência de retirada

1. Pare o alimentador antes de mover o cabeçote ou aguardar temperatura.
2. Mova à posição de purga configurada e salve o estado de extrusão.
3. Opcionalmente aguarde uma temperatura dedicada de formação da ponta; zero
   preserva a atual. Mantenha o alvo original do aquecedor para a próxima carga.
4. Desabilite temporariamente o pressure advance. Mantenha assistência de
   retirada (`MOTION_BEFORE_PULL_BACK`) ativa nas retrações e nos avanços de
   resfriamento, preservando o mesmo alvo de buffer por toda a formação.
5. Retraia 15 mm a 20 mm/s, depois aproxime a região de resfriamento de 35–45 mm
   em velocidades progressivamente menores.
6. Faça quatro ciclos de resfriamento de 10 mm em velocidades crescentes.
7. Restaure pressure advance e o alvo anterior do aquecedor.
8. Retraia até liberar o sensor de entrada, depois estacione os 55 mm adicionais
   com a verificação existente do encoder.

A busca final pelo switch usa F300 (5 mm/s) por padrão. Na formação e busca,
a extrusora espera entre movimentos concluídos quando a compressão do buffer
excede 70%, mantendo assistência ativa. Retoma em 65% ou menos e aborta se
o buffer não recuperar em três segundos. Essas verificações não sincronizam
passos dos motores nem medem força; conexão frouxa ou restrição ainda pode
causar falha em um movimento. Inspecione conexões PTFE, corte pontas dos tubos
em esquadro e encaixe-as completamente antes de testar novamente.

O limiar de recuperação considera o acompanhamento do buffer BMCU 1.0.1:
sua faixa de repouso pode chegar a 65%, conforme a pressão da carga.
Esperar 55% poderia causar timeout indevido enquanto o alimentador mantém
deliberadamente essa faixa.

A busca final também aborta se o encoder registrar menos de 1 mm de recuo
líquido em três segundos. Timeout ou encoder travado informa distância
comandada, deslocamento do encoder após a formação, porcentagem do buffer,
PWM e modo de movimento. Nenhuma operação com falha é confirmada estacionada.
Essas verificações não detectam todo deslizamento nem provam bico livre.

<a id="encoder-checks-throughout-tip-forming"></a>
### Verificações de encoder durante toda a formação

Antes, a formação verificava pressão do buffer somente entre movimentos de
até 15 ou 21 mm, mantendo o alimentador no modo FOLLOW de impressão.
Uma desconexão de PTFE relatada durante a retirada expôs a limitação:
as verificações da busca final não limitavam o filamento não recolhido
durante a formação.

Agora, todos os movimentos de extrusão da formação e busca final usam
incrementos monitorados de no máximo 1 mm por padrão. Movimentos negativos
e positivos usam o alvo configurado de assistência de retirada. Encoder e
sensor de entrada são conferidos entre incrementos; requisições UART ficam
fora das seções críticas de movimento do Klipper.

A seleção anterior por direção mudava para FOLLOW de impressão nos avanços
de resfriamento. Os controladores de impressão/retirada têm alvos de buffer
diferentes; trocá-los pode encher o buffer durante um avanço curto e exceder
a tolerância acumulada do encoder. Uma falha observada ocorreu em -44 mm
líquidos no primeiro avanço, com divergência de 2,63 mm, modo de impressão 3,
buffer 53% e PWM zero do motor no snapshot informado.

No BMCU-Klipper v1.0.6, o controlador de retirada é bidirecional: fornece
filamento abaixo do alvo de buffer e retrai acima. Manter
`MOTION_BEFORE_PULL_BACK` durante o resfriamento evita o reabastecimento pelo
alvo de impressão. A tolerância de 2 mm fica inalterada e ainda interrompe
movimento inesperado excessivo do alimentador nos dois sentidos de extrusão.
Isso remove o problema identificado de transição de modo; qualidade física
da ponta e recargas repetidas ainda precisam de verificação. Captura, purga
e impressão normais continuam com os modos existentes.

Uma referência de encoder é mantida por toda a retirada, incluindo os ciclos
de avanço/recuo. Antes de cada retração, a integração compara extrusão comandada
acumulada com deslocamento com sinal do encoder do alimentador. Com tolerância
padrão de 2 mm, não comanda outra retração que excederia essa diferença.
A extrusora espera com a assistência ativa; não acompanhar dentro de
`unload_progress_timeout` aborta e para o alimentador, sem confirmar
estacionamento. Mudança inesperada do modo de assistência também aborta.

Os padrões `unload_step_mm: 1` e `unload_slack_mm: 2` são configuráveis em
`[bmcu_ender_integration]`. Configurações existentes usam esses padrões sem
substituir distâncias ou ajustes personalizados da ponta. Captura, avanço ao
bico, purga de 35 mm, estacionamento de 55 mm, distâncias de resfriamento,
velocidades solicitadas e temperaturas ficam inalterados. O tempo real de
formação pode aumentar, pois cada incremento espera conclusão e feedback.
A ponta resultante ainda exige inspeção física.

O encoder mede no alimentador; não detecta todo deslizamento posterior,
obstrução ou conexão danificada. São verificações por feedback, não passos
sincronizados nem medição de força. A nova retirada exige teste físico de
retirada/recarga antes da impressão multicolor.

<a id="withdrawal-timing-and-status-traffic"></a>
### Tempos da retirada e tráfego de status

Cada incremento concluído fornece um snapshot novo para as verificações
seguintes de buffer, sessão, modo de assistência e encoder. Verificações
consecutivas o reutilizam somente até outro movimento ou espera explícita.
Esperas de recuperação do buffer e acompanhamento do encoder continuam
pedindo status novo após ceder a execução. Permanecem os incrementos de 1 mm,
tolerância de encoder de 2 mm, limiares de buffer e velocidades solicitadas.

Na fixture sintética bem-sucedida de retirada de 66 mm, requisições de status
caíram de 578 para 181 nos mesmos 146 movimentos de formação/resfriamento/
retirada. Isso mede tráfego, não porcentagem de melhoria do tempo físico de
impressão. Cada incremento ainda espera conclusão. Sobrepor mais movimento
e comunicação exige validar separadamente a fila limitada de movimentos.

Após estacionamento confirmado pelo encoder, o console informa tempos de
posicionamento, formação da ponta, liberação do sensor, estacionamento do
alimentador e total. Os mesmos valores são expostos em
`bmcu_ender_integration.unload_timing`, com resultado `awaiting_parking`,
`complete`, `failed` ou `parking_failed`. O total começa no hook de liberação
da extrusora; exclui pré-aquecimento manual anterior e preparação nativa de
carga/retirada. O tempo de estacionamento inclui a verificação final do encoder.
As medições identificam o próximo gargalo sem mudar temperatura, ciclos ou
percurso com base em um tempo total de botão não medido.

Limite do buffer, limiar de retomada e timeouts são editáveis em
`[bmcu_ender_integration]` como `unload_buffer_limit`, `unload_buffer_resume`,
`unload_buffer_timeout` e `unload_progress_timeout`. Ajustes personalizados
são preservados. Siga a
[instalação preparada](Ender3_V3_SE_Configuracao_Validada.md#install-or-update-an-existing-integration)
para preservar todo o host/display anterior. Não execute `git pull` no checkout
ativo antes. Após preparar a fonte limpa, somente se quiser explicitamente
a purga menor e a busca final mais lenta, use:

```sh
sudo python3 "$HOME/klipper-v2-source/scripts/ender3-v3-se/install_multicolor.py" \
  --host-source "$HOME/klipper-v2-source" \
  --purge-mm 35 --pullback-speed 300
```

Omita os dois argumentos de substituição para manter os valores calibrados
atuais. O atualizador somente do host rejeita mudanças nas entradas MCU/
compilação ou requisitos Python; não grava firmware nem instala dependências.
Comandos e parâmetros usam inglês nos dois idiomas da tela. Refatie trabalhos
antigos com o perfil v2 do Orca; G-code armazenado não é convertido em silêncio.

Instale somente com impressora parada, canais fisicamente estacionados e caminho
compartilhado vazio. Não declare uma rota incerta livre se ainda houver
filamento no caminho compartilhado. Temperaturas, distâncias e ciclos da
formação não mudam nesta atualização. Reduzir a purga separada à metade não
reduz à metade todo o plástico extrudado na carga: permanecem captura de 20 mm
e avanço de 80 mm ao bico.

Todas as distâncias descrevem deslocamento comandado da extrusora; o sensor
de entrada não prova captura nem saída de plástico pelo bico. A região de
resfriamento precisa ficar abaixo das engrenagens. Inspecione a ponta retirada
e teste recaptura antes de imprimir por longo tempo. Controlador reiniciado,
filamento ausente, sensor liberado ou falha de movimento abortam a operação.

Ajustes `*_f` usam **mm/min**, enquanto perfis de referência Happy Hare usam
mm/s. Velocidades iniciais são deliberadamente menores que as do perfil KE.
Continuam valendo limites de distância/velocidade de extrusão do Klipper.
Temperaturas escolhidas precisam superar `min_extrude_temp`; não é uma rotina
automática de cold-pull.

<a id="calibration"></a>
## Calibração

Use `UNLOAD_ACTIVE_CHANNEL` para testar a retirada completa de uma rota
carregada conhecida. Não confirme estacionamento antes de o caminho físico
compartilhado estar livre.

Altere uma variável por vez, sem reiniciar:

```gcode
SET_GCODE_VARIABLE MACRO=_BMCU_ENDER_TIP_SETTINGS VARIABLE=separation_f VALUE=1500
```

Para um experimento separado a 220 °C:

```gcode
SET_GCODE_VARIABLE MACRO=_BMCU_ENDER_TIP_SETTINGS VARIABLE=temperature VALUE=220
```

Para restaurar a sequência antiga de retirada:

```gcode
SET_GCODE_VARIABLE MACRO=_BMCU_ENDER_TIP_SETTINGS VARIABLE=enabled VALUE=0
```

Mudanças em execução se perdem no reinício. Persista valores aceitos em
`bmcu-formacao-ponta.cfg`. Essas mudanças Python/configuração não exigem
gravação de firmware. Comece pela referência PETG de 230 °C, inspecione a ponta
e observe extrusora/alimentador; cliques repetidos, desgaste por atrito ou
filamento comprimido são testes com falha, mesmo se o software concluir.

<a id="four-color-test"></a>
## Teste em quatro cores

O bloco mede 24 × 16 × 6,4 mm, com 32 camadas de 0,2 mm. Cada cor ocupa duas
camadas. Canais 1, 2, 3 e 4 repetem quatro vezes: 16 cargas e 15 trocas entre
cores, dando a cada canal três recargas após o primeiro uso. Fatie por camada,
sem camadas adaptativas nem torre de purga; a integração purga no balde.
Os quatro índices mapeiam explicitamente para canais físicos 0–3 em `bmcu0`.

Corte pontas danificadas, estacione todos os canais e confirme caminho vazio
antes de instalar. Confira PETG nos quatro canais e se as rotas do seletor
correspondem às conexões físicas. Observe o primeiro par retirada/recarga.
Três recargas por canal são uma verificação funcional inicial, não prova de
confiabilidade em impressão longa. O canal final fica carregado para inspecionar
sua última ponta pelo controle normal de retirada depois de imprimir.

<a id="references-and-attribution"></a>
## Referências e atribuição

- [Formação de ponta Happy Hare](https://github.com/moggieuk/Happy-Hare/blob/main/config/macros/mmu_form_tip.cfg),
  Paul Morgan e contribuidores, GPLv3: separação escalonada no bico,
  resfriamento e remoção temporária de pressure advance.
- [Perfil comunitário Ender 3 V3 KE](https://github.com/3DCoded/MMU-Tip-Configs/blob/main/Ender3-V3-KE.cfg),
  matthew73210: resfriamento em 35 mm, curso de 10 mm, quatro ciclos e nenhuma
  mudança de temperatura. Hardware original e material não especificado não
  comprovam compatibilidade com hotend cerâmico atualizado da SE.
- [Experimento de formação rápida 3MS](https://github.com/3DCoded/3MS/issues/25):
  sequência escalonada alternativa; tempos históricos não garantem desempenho
  nesta integração.
- [Contrato de endpoint genérico BMCU](https://github.com/jarczakpawel/BMCU-Klipper/blob/v1.0.6/printers/Generic/README.md):
  formação da ponta e liberação da extrusora precedem a retirada longa do
  alimentador.
