# Ender 3 V3 SE / Orange Pi Zero3 — recuperação da configuração real

O diretório `2026-10-20/` contém a cópia revisada dos arquivos ativos da máquina,
obtida do backup privado `backup-restauracao-20261020-034530.tar.gz`. A captura
passou na verificação de 371 arquivos. O SHA256 do arquivo completo é
`11f2fe7cd9d13fd46e1f82663b05271378108f4a5b4a7a41bafe05ae4e56728a`.

O complemento `complemento-restauracao-20261020-045613.tar.gz` também foi
verificado: seis arquivos íntegros, com SHA256
`063f5fb108de24820b0ecdf0d4ddc6ad2ae78e01811f9837ffb19bf6271e48f5`.
Ele fecha os arquivos de ambiente dos serviços e as versões informadas pelas MCUs.

O backup privado continua sendo necessário para recuperar o runtime BMCU exato,
o fonte instalado, binários e credenciais originais. A cópia pública não é uma
imagem inicializável do cartão SD. Não armazena senha SSH, banco Moonraker nem
presets pessoais exportados do Orca no computador.

## O que foi preservado

| Item | Valor capturado |
| --- | --- |
| Sistema | Armbian 26.8.3 trixie / Debian 13 / aarch64 |
| Kernel | 6.18.44-current-sunxi64 |
| Klippy | v0.13.0-810-geff16d2a5 |
| Commit Klipper | eff16d2a5b08503e089e7ebaabb3462771b09191 |
| MCU Ender, informada pela placa | v0.13.0-782-geb5fb15e3; stm32f103xe; bridge da tela ativo |
| MCU Linux, informada pelo serviço | v0.13.0-794-gceb442eb6 |
| Extrusora | rotation_distance 7,589; PID 20,800 / 2,433 / 44,461 |
| Mesa | PID 64,987 / 0,715 / 1476,821 |
| Z-offset | 1,940 mm |
| Malha default | 9×9, com os 81 pontos reais preservados |
| BMCU | Pacote 1.0.6; firmware informado 1.0.1 |
| Velocidades persistentes | Carga 10, retirada 10, fim da retirada 4 mm/s |
| Troca | Captura 20, avanço 80, purga 70, retirada até switch e mais 55 mm |
| Tela | pt_BR |

`config/printer.cfg` e seu bloco `SAVE_CONFIG` são idênticos ao arquivo capturado.
As regras Git deste diretório preservam finais de linha e espaços dos dados
capturados, inclusive ao clonar no Windows. Os demais arquivos de configuração
também são preservados, exceto o token em
`bmcu/bmcu_panel.cfg`, substituído por `__GERAR_TOKEN_LOCAL__`. O restaurador
conserva o token da instalação de destino ou gera outro e atualiza seu hash nos
metadados locais BMCU. Não copie esse marcador diretamente para um painel ativo.

`bmcu_state.json` contém materiais, cores, rotas lógicas, LEDs e política de
velocidades. O valor de carga 80 em `bmcu/bmcu.cfg` é o fallback do pacote;
o estado persistente capturado aplica **10 mm/s**. Restaurar só os `.cfg`, sem
esse JSON, não reproduz o comportamento da máquina.

`calibracao-bmcu.json` registra os valores informados pelos sensores na captura,
incluindo a polaridade diferente entre os canais. É uma referência: o restaurador
não grava esses valores na flash da BMCU. Se a controladora for regravada ou
trocada, confira a calibração dos quatro canais no próprio firmware.

O painel ainda contém atalhos históricos para malha 7×7 e avanço de 60 mm; as
sequências multicolor ativas usam 80 mm e a malha salva é 9×9. As macros antigas
`LOAD_FILAMENT`/`UNLOAD_FILAMENT` continuam sem calibração própria; a sequência
validada usa `BMCU_TROCAR_ENDER`. Esses arquivos foram preservados para não
introduzir mudanças no comportamento já observado durante a recuperação.

## Serviços e firmware identificados

`ambiente-servicos/` contém `klipper.env`, `moonraker.env`, `crowsnest.env` e
`sonar.env`, revisados e sem credenciais. Os quatro mantêm os bytes capturados.
O restaurador os prepara em `/home/pi/printer_data/systemd/`, junto da
configuração. As unidades e overrides originais estão em `sistema/etc/systemd/`;
a instalação dessas unidades continua sendo uma etapa separada, conforme os
programas e ambientes Python disponíveis na imagem nova.

`versoes-mcus.json` preserva versões, compiladores, constantes e configuração
informados por `mcu` e `mcu host` via Moonraker. Contadores transitórios da conexão
foram omitidos. A Ender informa a mesma versão do artefato `klipper.bin` do backup
privado, mas essa consulta não verifica o hash da flash. O arquivo não é publicado
nem aplicado automaticamente como firmware recomendado.

A combinação capturada está funcionando com **Klippy v810, Ender v782 e MCU Linux
v794**. Preservar essa evidência não significa recomendar misturar versões em uma
instalação nova. Recuperar os `.cfg` não exige regravar a Ender; se for atualizar
firmware, compile e valide host e MCUs do mesmo fonte/release como uma operação
separada. Não atualize a máquina funcional só para igualar esses números.

O pacote completo de presets pessoais do Orca ainda precisa ser exportado no
computador. Os ajustes do projeto testado já estão em
`profiles/orca/ender3-v3-se-bmcu/`. A restauração após formatar um cartão ainda
precisa de validação física; a configuração ativa e os arquivos de ambiente
solicitados já foram recebidos, revisados e incorporados.

A ausência de `/boot/overlay-user` não exige criar a pasta: o `armbianEnv.txt`
capturado não declara `user_overlays`. Na imagem nova, preserve seu `rootdev`;
o UUID do cartão anterior não deve substituir o da nova instalação. Confira
somente os overlays e as ligações de GPIO correspondentes à montagem.

## Restaurar os arquivos da impressora

Esta ferramenta se destina a uma reinstalação futura. A máquina que já está
funcionando não precisa receber estes arquivos novamente.

1. Prepare a Orange Pi com Klipper/Moonraker/Mainsail e o mesmo usuário `pi`.
   Use o commit Klipper indicado acima, recrie o ambiente Python com o inventário
   em `requirements-klippy.txt` e instale BMCU-Klipper 1.0.6. Os módulos nativos e
   seu runtime precisam existir antes de preparar a restauração pública.
2. Prepare os serviços usando as unidades e overrides em `sistema/etc/systemd/`.
   Seus quatro arquivos `.env` já estão em `ambiente-servicos/` e serão copiados
   pelo restaurador para `printer_data/systemd/`. O transporte BMCU é inicializado
   pelo `ExecStartPre` de Klipper; não havia um serviço separado chamado `bmcu-transport` nesta instalação.
3. Recompile o MCU Linux do fonte correspondente e recupere o override
   `klipper-mcu.service.d/switch.conf`, que remove `-r`. Confira a existência de
   `/tmp/klipper_host_mcu`, a UART `/dev/ttyS1` e o GPIO do switch.
4. A partir deste repositório de recuperação, simule a preparação:

```sh
python3 scripts/ender3-v3-se/restaurar_snapshot.py
```

Depois de preparar os serviços e conferir o destino, aplique:

```sh
sudo python3 scripts/ender3-v3-se/restaurar_snapshot.py --aplicar
```

O script verifica os hashes do snapshot e o commit instalado, preserva ou gera
o token local, copia a configuração e os quatro arquivos de ambiente e aplica
os ajustes compatíveis da integração BMCU. Cria um backup privado dos arquivos substituídos antes de parar
Klipper; se uma escrita falhar, restaura os arquivos anteriores. Mantém o serviço
parado ao final para conferência. Outros serviços só leem os arquivos `.env`
restaurados no próximo início; eles não são reiniciados pela ferramenta.
Não instala o sistema, não substitui unidades systemd,
não aplica `armbianEnv.txt`, não recompila nem grava firmware e não movimenta eixos.

O token, materiais e state file de origem só devem ser restaurados de uma cópia
privada quando esse for o objetivo; a ferramenta pública gera uma credencial
válida para a instalação atual. Não execute `BMCU_CONFIGURAR_MULTICOR` a cada
inicialização: a configuração do endpoint já está no state file capturado.

Confira o filamento físico e depois inicie:

```sh
sudo systemctl start klipper
```

Verifique temperaturas, switch, sonda e homing, depois a troca entre os canais
2 e 4. Uma rota carregada na captura não comprova a posição física depois da
remontagem. A restauração completa ainda precisa ser validada nessa situação;
os testes automatizados cobrem o preparo, arquivos de ambiente, integridade,
credenciais e rollback.
