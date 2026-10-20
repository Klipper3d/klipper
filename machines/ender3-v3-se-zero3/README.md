# Ender 3 V3 SE / Orange Pi Zero3 — recuperação da configuração real

O diretório `2026-10-20/` contém a cópia revisada dos arquivos ativos da máquina,
obtida do backup privado `backup-restauracao-20261020-034530.tar.gz`. A captura
passou na verificação de 371 arquivos. O SHA256 do arquivo completo é
`11f2fe7cd9d13fd46e1f82663b05271378108f4a5b4a7a41bafe05ae4e56728a`.

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

## Pendências identificadas

- A primeira captura não trouxe `printer_data/systemd/klipper.env`,
  `moonraker.env` e `crowsnest.env`, referenciados pelos serviços.
- O pacote completo de presets pessoais do Orca ainda precisa ser exportado
  pelo próprio Orca no computador. Os ajustes do projeto testado já estão em
  `profiles/orca/ender3-v3-se-bmcu/`.
- O artefato de firmware capturado tem dicionário `v0.13.0-782-geb5fb15e3`,
  anterior ao checkout do host. Não há comprovação de que seja o binário gravado
  na placa, portanto não é publicado nem aplicado como firmware recomendado.

A ausência de `/boot/overlay-user` não exige criar a pasta: o `armbianEnv.txt`
capturado não declara `user_overlays`. Na imagem nova, preserve seu `rootdev`;
o UUID do cartão anterior não deve substituir o da nova instalação. Confira
somente os overlays e as ligações de GPIO correspondentes à montagem.

## Completar a captura dos serviços

Na Orange Pi, execute a versão atual do coletor deste checkout:

```sh
sudo python3 scripts/ender3-v3-se/capturar_configuracao.py --complemento
```

O complemento é pequeno, fica em `~/printer_data/config/` e pode ser baixado
pelo Mainsail. Ele lê os arquivos de ambiente e consulta as versões das MCUs pelo Moonraker;
não envia G-code, não interrompe nem movimenta a impressora. O conteúdo é privado até ser revisado.

## Restaurar os arquivos da impressora

Esta ferramenta se destina a uma reinstalação futura. A máquina que já está
funcionando não precisa receber estes arquivos novamente.

1. Prepare a Orange Pi com Klipper/Moonraker/Mainsail e o mesmo usuário `pi`.
   Use o commit Klipper indicado acima, recrie o ambiente Python com o inventário
   em `requirements-klippy.txt` e instale BMCU-Klipper 1.0.6. Os módulos nativos e
   seu runtime precisam existir antes de preparar a restauração pública.
2. Recupere os serviços e seus arquivos `.env` a partir da captura privada e do
   complemento. O serviço BMCU é inicializado pelo `ExecStartPre` de Klipper;
   não havia um serviço separado chamado `bmcu-transport` nesta instalação.
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
o token local, copia a configuração e aplica os três ajustes compatíveis da
integração BMCU. Cria um backup privado dos arquivos substituídos antes de parar
Klipper; se uma escrita falhar, restaura os arquivos anteriores. Mantém o serviço
parado ao final para conferência. Não instala o sistema, não substitui serviços,
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
os testes automatizados cobrem o preparo, integridade, credenciais e rollback.
