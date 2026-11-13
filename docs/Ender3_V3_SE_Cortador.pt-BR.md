# Cortador opcional com servo na Orange Pi

[English: guia completo e referências](Ender3_V3_SE_Cutter.md)

Usamos o [cortador de matarsaed para Ender 3 V3 SE/KE](https://www.printables.com/model/1243521-filament-cutter-for-ender-3-v3-seke), com os [comentários de montagem](https://www.printables.com/model/1243521-filament-cutter-for-ender-3-v3-seke/comments), e uma adaptação do [suporte de servo de jmillerfo](https://www.printables.com/model/1097275-voron-24r2-filametrix-filament-cutter-gantry-servo). Os arquivos CAD e suas licenças continuam nas páginas dos autores.

O servo posiciona o batente; quem pressiona a alavanca é o movimento X do cabeçote. Este perfil exige corte **abaixo das engrenagens da extrusora**. A sobra dentro do hotend será empurrada e purgada pelo próximo filamento. Um cortador acima das engrenagens, deixando uma sobra solta sem tração, exige outra sequência.

## Ligação proposta na Orange Pi Zero 3

| Fio do SG90 | Ligação |
| --- | --- |
| Sinal, normalmente laranja/amarelo | Pino físico 15, PC8, após conferir a disponibilidade |
| GND, normalmente marrom/preto | GND da fonte do servo e GND da Orange Pi, por exemplo pino físico 6 |
| Alimentação, normalmente vermelho | Fonte regulada externa de 5 V, com capacidade para os picos do servo |

Use SG90 posicional, não de rotação contínua. O GPIO fornece sinal de 3,3 V, não alimentação para o motor. Recomenda-se fonte regulada com capacidade de pelo menos 2 A para um SG90. Una os GNDs, mantenha a alimentação da Orange Pi e não injete a fonte externa nos pinos de 5 V da placa. Não ligue o servo em 24 V. O encaixe do SG90 não comprova a durabilidade de suas engrenagens no mecanismo originalmente projetado para MG90S.

O sensor de entrada usa PC15/pino 16 na configuração validada. A UART5 da BMCU deve permanecer intacta. O servo proposto usa PWM por software da MCU Linux; não habilite overlays de PWM nos pinos da UART5. Confira estabilidade do servo com a câmera e a carga normal da Orange Pi antes de confiar em trocas automáticas.

Confira o endereço e se o GPIO está livre:

```sh
sudo gpiodetect
sudo gpioinfo
```

O endereço proposto para PC8 no layout observado é `host:gpiochip1/gpio72`, mas o índice do gpiochip depende do kernel. Pino físico 15 não significa GPIO15. Consulte o pinout e a orientação de GPIO no guia em inglês.

O PC9 (pino físico 7) está ligado à interrupção do controlador de alimentação AXP313A no [device tree da Zero 3 no Linux](https://github.com/torvalds/linux/blob/v6.18/arch/arm64/boot/dts/allwinner/sun50i-h618-orangepi-zero3.dts). Não o use como saída do servo nem desative a interrupção do PMIC para liberá-lo. Ausência de consumidor no `gpioinfo` não comprova disponibilidade como saída: também existem reservas de interrupção e multiplexação. O instalador rejeita o offset 73 quando identifica a Orange Pi Zero 3. A indicação anterior do PC9 estava incorreta.

Se a instalação falhou no PC9, mantenha a configuração restaurada. Desligue as alimentações antes de mover somente o sinal do servo para PC8, mantendo os 5 V e o GND comum. Após ligar e o Klipper ficar pronto, repita a instalação com o endereço conferido do PC8. Se a MCU Linux continuar em shutdown após a restauração, execute `FIRMWARE_RESTART` com a configuração restaurada antes de tentar novamente. Teste o servo somente após a instalação concluir.

## Atualização e calibração

Cancele a impressão, retire manualmente qualquer filamento preso e confirme os canais estacionados/vazios antes da atualização. Faça backup. Após conferir o GPIO:

```sh
cd "$HOME/klipper" &&
git pull --ff-only origin master &&
sudo python3 scripts/ender3-v3-se/install_multicolor.py \
  --cutter-servo-pin host:gpiochip1/gpio72
```

Substitua o endereço se o mapeamento for diferente. A alteração é no host; não exige novo `.bin` da impressora. O instalador faz backup, preserva calibrações e restaura os arquivos anteriores se a inicialização falhar. Sem o argumento do servo, instala o perfil opcional e o reset sem reservar GPIO.

O arquivo `bmcu-cortador.cfg` começa com `habilitado=0` e `calibrado=0`. As coordenadas inválidas de propósito devem ser substituídas por medições. O servo não recebe um ângulo automaticamente ao iniciar.

1. Com o cabeçote fisicamente afastado do batente, use `CORTADOR_SERVO ANGULO=<angulo_seguro> CONFIRMAR=1` para encontrar as posições recolhida e acionada. Se não souber a direção, comece com o braço sem carga mecânica.
2. Edite os ângulos, `variable_x_aproximacao`, `variable_x_corte`, `variable_y_corte` e a folga Z em `bmcu-cortador.cfg`. Meça o curso completo na sua montagem. As coordenadas de purga não são coordenadas de corte. Confira interferências com câmera, ventoinha, hub e mesa sem aumentar limites para esconder colisões.
3. Reinicie o Klipper, comande o ângulo recolhido e faça homing. Com hotend/caminho compartilhado livres e canais estacionados, execute `TESTAR_CORTADOR CONFIRMAR=1`. O teste é mecânico, sem aquecer ou extrudar. Confira corte completo, retorno da mola e recolhimento.
4. Depois do teste, configure `variable_calibrado: 1` e `variable_habilitado: 1` e reinicie.
5. Faça carga e retirada a quente, confira a ponta cortada e o fluxo no bico, depois repita um teste curto de quatro cores. Não há sensor de posição da lâmina, confirmação de corte ou captura depois das engrenagens: comandos concluídos não comprovam sucesso físico.

Na tela original, **Mover → Abrir batente / Fechar batente** comanda só o servo, após a confirmação de folga. **Preparar → Referenciar eixos** envia somente `G28`, sem recolher antecipadamente nem pedir confirmação na partida normal. O console e a tela usam a mesma sequência abaixo. O comando manual `CORTADOR_SERVO POSICAO=RECOLHIDO CONFIRMAR=1` continua disponível para calibração e recuperação, com o cabeçote livre.

Com o cortador habilitado e calibrado, todo `G28` primeiro referencia X pelo fim de curso real (−6 mm no perfil desta máquina), **sem enviar nenhum comando ao servo, nem `WIDTH=0`, antes ou durante essa referência**. Somente após X concluir e confirmar uma posição livre, recolhe o batente, espera `espera_servo_ms` e libera o PWM conforme o ajuste abaixo. O `G28` completo então referencia Y (mesa) e Z (vertical). O `safe_z_home` continua responsável por uma única elevação inicial de proteção, que pode ocorrer antes de X, pelo ponto de medição de Z e pela elevação após a sondagem.

**Todo o percurso até o fim de curso X precisa estar livre com o batente na posição em que ficou**, inclusive após falta de energia; não use essa sequência em uma montagem que exija mover o servo para liberar o próprio homing de X. Não se usa movimento X sem referência nem posição fictícia. `G28 X` termina após recolher; `G28 Y` faz X → recolher → Y; `G28 Z` faz X → recolher → Z e mantém a exigência nativa de Y já referenciado. A opção antiga `recolher_sem_referencia` é obsoleta e não dispensa a referência de X. Uma falha de X, servo ou validação impede as etapas seguintes. Se um corte deixou o batente incerto na sessão atual, a recuperação manual continua obrigatória. Carga/retirada manual usa esse mesmo `G28` antes de iniciar a operação, quando precisa referenciar ou recolher.

Se estiver habilitado sem calibração, sem servo, fora dos limites ou com posição incerta, o sistema interrompe a operação. Ele não volta silenciosamente à formação de ponta. Desabilitado, mantém o fluxo anterior.

`variable_soltar_servo_recolhido: 1` agora é o padrão de `bmcu-cortador.cfg` e também quando a variável está ausente. Após comandar o ângulo recolhido e esperar o braço parar, a integração envia `SET_SERVO SERVO=bmcu_cortador WIDTH=0`, evitando que o servo fique corrigindo o ângulo e tremendo. **O instalador preserva perfis existentes:** se o seu arquivo ainda tiver `variable_soltar_servo_recolhido: 0`, altere para `1` na seção `[gcode_macro _BMCU_ENDER_CORTADOR_AJUSTES]` e reinicie o Klipper para adotar esse comportamento.

Confirme no teste mecânico que o batente recolhido continua fora de todo o percurso sem torque, inclusive com vibração. Se sua montagem exige força para mantê-lo recolhido, use explicitamente `0` para manter o PWM. Quando acionado, o servo continua firme durante todo o avanço da lâmina e o retorno do cabeçote; só libera depois de recolher. Um corte interrompido não desliga o servo acionado automaticamente. Desligar o PWM não corta a alimentação de 5 V.

Quando houver trecho livre entre o acionamento e a alavanca, registre o primeiro toque em `variable_x_contato`. Nesse caso, `variable_curso_maximo_mm` limita a distância do contato ao corte completo. Com `None` ou sem essa variável, o limite continua valendo desde a aproximação. O avanço e o recuo com o batente acionado usam `variable_corte_f` em todo o trecho.

Use `variable_manter_y_corte: 1` somente quando houver folga para o corte com a mesa em qualquer posição de troca. O corte ignora `variable_y_corte` nesse modo. A purga continua usando a posição já configurada para a calha.

### Recuo antes de cortar

`variable_recuo_antes_corte_mm` vem em 0. Aceita até 5 mm, mas não pode exceder `folga_recuo_mm`; o perfil é rejeitado antes de mover o cabeçote se exceder essa folga. Na aproximação, a extrusora recua em passos monitorados com a lâmina livre e a BMCU parada. O buffer absorve a diferença. Se o sensor liberar, o buffer atingir o limite, a sessão mudar ou qualquer motor da BMCU continuar ativo, a operação para antes da lâmina. O teste a seco não movimenta filamento.

Se o filamento prende na entrada do dissipador, confira se a base é a versão `Cutter_Base with guide v1` indicada pelo autor do modelo. Antes de ativar, confira o alinhamento do guia com o hotend e verifique que o recuo total não leva material amolecido até a lâmina. O recuo ainda ativo do fatiador soma-se ao da integração; não há compensação automática. O recuo pré-corte usa `recuo_f`; distâncias de purga e posições da lâmina não mudam.

O instalador também impede que o núcleo BMCU ligue a assistência por 200 ms antes do callback `BMCU_LIBERAR_ENDER`. O canal e a sessão são passados explicitamente porque parar os motores pode limpar `now_channel`. A assistência começa somente na retirada após o curso da lâmina e o recolhimento do batente. Reinstale pelo instalador completo, não apenas copiando os dois módulos opcionais: o ajuste do gerenciador nativo faz parte da mesma atualização. Outros callbacks preservam o comportamento nativo.

## Velocidade e segurança do caminho

O perfil do cortador elimina a formação térmica da ponta e começa com recuo F600, avanço ao bico F240 e purga F180, em mm/min. A captura e as distâncias existentes são preservadas. A BMCU precisa acompanhar o recuo; a velocidade de recolhimento validada é 10 mm/s. Reduza as velocidades se houver patinação ou fluxo insuficiente, sem aumentar a tolerância de erro do encoder para esconder o problema.

Com `variable_posicionamento_paralelo: 1`, o cabeçote pode ir à purga durante a busca nativa até o switch somente com todos os caminhos livres e sensor de entrada desacionado. A extrusora espera o posicionamento terminar antes da captura. Não se antecipa um segundo filamento em um caminho ocupado ou incerto.

Os tempos podem ser comparados em `unload_timing` e `ultimo_corte` no estado de `bmcu_integracao_ender`. Uma interrupção registra `ultimo_corte.etapa`, inclusive se ocorrer na validação ou no recolhimento inicial; não reutiliza o sucesso de um corte anterior. `sincronismo_pre_corte` e `sincronismo_pos_corte` registram recuo comandado, encoder, buffer, PWM, modo e tempo antes/depois dos passos. O tempo dos movimentos inclui a espera por conclusão, não apenas o giro do motor. Os testes verificam a sequência; confiabilidade e tempos reais exigem medição na impressora.

Purga e limpeza usam coordenadas físicas da máquina, como o cortador, independentemente de `G92`, offsets do G-code ou `M220`. Os limites da calha, do limpador e a altura disponível para o alívio Z são conferidos antes de posicionar; sem a folga Z completa, não inicia o deslocamento XY. Distâncias, velocidades, coordenadas e tempos de configuração devem ser finitos; `nan` e infinito são rejeitados. Ao final do estacionamento, a confirmação exige encoder, sensor livre, mesma sessão e motores parados; uma resposta antiga de fim de movimento não basta.

O sensor de entrada acionado mantém o recuo da extrusora com assistência da BMCU, sujeito aos limites de tempo, buffer e progresso do encoder. O motor da BMCU estar momentaneamente parado no alvo do buffer não comprova que a ponta saiu das engrenagens. A tentativa introduzida no PR #22, que substituía o recuo da extrusora aos 30 mm por uma redução do alvo do buffer, foi removida após falhar no teste físico. O recuo pré-corte de 2 mm continua disponível. Essa reversão corrige a interrupção antecipada, mas não explica por si só os timeouts anteriores aos 62 mm.

## Reset após retirada manual

Depois de cancelar a impressão e deixar hotend e caminho compartilhado fisicamente livres:

```gcode
RESET_BMCU CONFIRMAR=1
```

O reset para os motores da BMCU e confirma os quatro canais pelo comando nativo: `PARKED` onde a entrada ainda detecta filamento, `EMPTY` onde não detecta. Exige sensor de entrada livre e nenhuma impressão ativa/pausada ou operação da BMCU em andamento. Somente após confirmar todos os canais encerra a sessão antiga e limpa a troca pendente. Uma falha parcial não é anunciada como sucesso.

Não retira filamento fisicamente, não aquece, não desentope, não apaga calibrações e não declara que o servo foi recolhido. Para preservar uma impressão pausada, siga a [recuperação de troca](Ender3_V3_SE_Print_Recovery.md). Se o corte deixou o batente incerto, libere-o fisicamente; `CORTADOR_SERVO` permite recolhê-lo nessa recuperação pausada com confirmação e sem operação da BMCU ativa.
