# Klipper para Ender 3 V3 SE + BMCU

[English](README.md) | **Português brasileiro**

[![Ender CI](https://github.com/bernardokcosta/ender3-v3-se-klipper-bmcu/actions/workflows/ender3-v3-se.yml/badge.svg)](https://github.com/bernardokcosta/ender3-v3-se-klipper-bmcu/actions/workflows/ender3-v3-se.yml)
[![Release](https://img.shields.io/github/v/release/bernardokcosta/ender3-v3-se-klipper-bmcu)](https://github.com/bernardokcosta/ender3-v3-se-klipper-bmcu/releases)
[![GPLv3](https://img.shields.io/badge/license-GPLv3-blue.svg)](COPYING)

Fork não oficial do Klipper para a **Creality Ender 3 V3 SE**, combinando
suporte à tela original e transporte opcional de filamento BMCU de quatro
canais. Uma extrusora física atende aos quatro canais. O BMCU-Klipper
continua sendo uma dependência separada.

> **Mudança incompatível de API na v2.0.0:** comandos, parâmetros e valores de enum
> da integração são somente em inglês. Use artefatos publicados com tag, não uma
> branch sem validação. Refatie
> trabalhos antigos com o [perfil Orca v2](profiles/orca/ender3-v3-se-bmcu/README.pt-BR.md);
> G-code v1 armazenado não é convertido automaticamente.

<a id="version-200"></a>
## Versão 2.0.0

- Tela original TJC3224/DWIN de 3,2 polegadas, em inglês e português brasileiro.
- Integração BMCU 1.0.6 / firmware 1.0.1 por endpoint genérico de extrusora única.
- Chegada por switch, captura iniciada pela extrusora, purga, limpeza e retirada
  assistida.
- Tratamento automático do `T0` do OrcaSlicer 2.4.2 pelo mapa confirmado.
- Klipper upstream integrado até `0cee841e31d701595b3e509a0b502f6e320f52a8`.
- Testes de regressão, compilação reproduzível e checksums correspondentes.
- API de integração somente em inglês com migração protegida de configurações
  e callbacks.
- Rollback integral preparado de host/tela, preservando as evidências atuais
  das rotas.
- Menus bilíngues organizados na tela e documentação pública EN/PT-BR completa.

Veja as [notas da release](docs/pt-BR/releases/v2.0.0.md) para validação e
limitações, e a [procedência e política de tradução](docs/pt-BR/Translations.md)
para cobertura de idiomas. A 2.0.0 identifica este fork; não muda versões do pacote/firmware
BMCU. Calibração e comissionamento físico continuam necessários.

<a id="hardware-scope"></a>
## Escopo de hardware

A MCU suportada é **GD32F303RET6**, na placa CR4NS200320C14 verificada, usando
o alvo compatível STM32F103 do Klipper, bootloader de 28 KiB e USART1.
Confira o chip; a marcação da placa é insuficiente.
**Não grave este binário em uma placa STM32F401.**

CR-Touch usa o suporte upstream. O firmware histórico de Z offset automático
PRTouch/HX711 não está incluído. BMCU e mecanismo de purga são opcionais;
cursos, offsets da sonda, GPIOs e sequências de filamento particulares da
instalação precisam ser medidos na impressora real.

<a id="install-and-upgrade"></a>
## Instalação e atualização

1. Leia o [guia de hardware e gravação](docs/pt-BR/Ender3_V3_SE.md).
2. Faça backup da configuração/calibração antes de mudar uma instalação.
3. Use fontes do host e firmware MCU da mesma tag. Baixe `klipper.bin`,
   `klipper.dict`, `defconfig`, `BUILD-METADATA.txt` e `SHA256SUMS` em
   [Releases](https://github.com/bernardokcosta/ender3-v3-se-klipper-bmcu/releases).
4. Para instalação nova, use
   [`config/printer-creality-ender3-v3-se-gd32f303-2023.cfg`](config/printer-creality-ender3-v3-se-gd32f303-2023.cfg)
   com os módulos de `config/ender3-v3-se/`. Não substitua um `printer.cfg`
   calibrado pelo modelo.
5. Para BMCU, instale primeiro
   [BMCU-Klipper 1.0.6](https://github.com/jarczakpawel/BMCU-Klipper/tree/v1.0.6)
   e seus serviços, depois siga o
   [guia de integração](docs/pt-BR/Ender3_V3_SE_Configuracao_Validada.md).

Em uma impressora BMCU já configurada, mantenha a impressora parada, canais
estacionados e sensor livre. Use uma fonte v2 limpa em checkout separado para
preservar todo o host/display anterior antes de ativar.
**Não execute `git pull` no checkout ativo primeiro.** Após publicar a v2.0.0:

```sh
git clone --branch v2.0.0 --single-branch \
  https://github.com/bernardokcosta/ender3-v3-se-klipper-bmcu.git \
  "$HOME/klipper-v2-source"
sudo python3 "$HOME/klipper-v2-source/scripts/ender3-v3-se/install_multicolor.py" \
  --host-source "$HOME/klipper-v2-source"
```

O instalador preparado preserva checkout antigo, módulos opcionais, calibração
e ajustes da integração. Falha na partida restaura todo o host/display anterior
e configurações afetadas, revertendo callbacks conhecidos no estado BMCU
**atual**, sem substituir evidências das rotas. O host recuperado precisa ficar
pronto; caso contrário, serviços ficam **parados**. Os dois checkouts são
mantidos para diagnóstico. Esse fluxo recusa mudanças em entradas MCU/
compilação ou requisitos Python e nunca grava firmware nem instala dependências.
O modo só de integração sem `--host-source` não restaura um checkout alterado
antes; falhas nesse modo deixam serviços parados. Atualizar somente Git não
substitui módulos opcionais em `klippy/extras/`.

Tela e Mainsail compartilham o backend operacional. **Temperatura** concentra
pré-aquecimentos PLA/PETG/TPU e resfriamento; **Filamento / BMCU** concentra
carga dos quatro canais, retirada automática a 230 °C e reset protegido.
Trocas estacionam o filamento anterior antes da próxima carga/purga. Veja os
[controles diários](docs/pt-BR/Ender3_V3_SE_Configuracao_Validada.md#everyday-filament-controls)
para o fluxo e os ajustes preservados de pré-aquecimento.

O instalador adiciona um
[perfil experimental de formação da ponta e teste de recarga em quatro cores](docs/pt-BR/Ender3_V3_SE_Tip_Forming.md),
preservado nas atualizações. Ele separa e resfria a ponta antes da retirada
existente por sensor/encoder, usando assistência de acompanhamento do buffer
após captura. Inspecione pontas e valide recargas antes de considerar o perfil
calibrado.

A integração também protege contra identificadores de resultados em cache após
reinícios do host e preserva impressões SD virtual quando uma troca falha.
Veja [operações e recuperação da impressão](docs/pt-BR/Ender3_V3_SE_Print_Recovery.md)
para confirmação protegida do caminho e retomada da troca pendente.

Escolha **Configurações → Idioma → English / Português** na tela para aplicar
imediatamente. **Configurações → Salvar ajustes** persiste e reinicia o Klipper
após confirmação quando parado. Preferências manuais `language: en` /
`language: pt_BR` pertencem ao `[e3v3se_display]` principal, não em include.
O idioma da tela não muda a API. Nesta branch v2, comandos, parâmetros e valores
enumerados usam somente inglês. O instalador migra nomes legados conhecidos
e callbacks dos endpoints; não mantém aliases portugueses nem reescreve
trabalhos já fatiados.

O [cortador opcional com servo na Orange Pi](docs/pt-BR/Ender3_V3_SE_Cutter.md)
substitui a formação térmica da ponta após calibração física. Taxas separadas
de avanço, purga e retirada e sobreposição protegida entre movimento do carro
e chegada buscam trocas menores. Vem desabilitado e exige coordenadas medidas
e ligação GPIO confirmada. O comando v2 é `RESET_BMCU CONFIRM=1` para reconciliar
os quatro canais após desobstruir manualmente hotend e caminho compartilhado.

<a id="orcaslicer"></a>
## OrcaSlicer

Use os [trechos de início, fim e troca](profiles/orca/ender3-v3-se-bmcu/README.pt-BR.md).
Troque `YOUR_BMCU_UID` pelo UID da sua BMCU antes de fatiar. Com mapa BMCU
confirmado, `T0` a `T3` selecionam ferramentas lógicas 1 a 4; o mapa resolve
canais físicos. Sem mapa ativo, comandos nativos mantêm seu significado,
incluindo alimentação externa por `T0`. Não exige pós-processamento no PC.

<a id="printable-hardware"></a>
## Peças para impressão

| Peça | Modelo e download |
| --- | --- |
| Suporte BMCU para Creality Ender 3 V3 SE | [MakerWorld](https://makerworld.com/pt/models/3516373-bmcu-mount-for-creality-ender-3-v3-se) |
| Hub PTFE quatro para um com sensor de entrada | [Printables](https://www.printables.com/model/1320106-hub-filament-have-sensor-for-pico-mmu-for-ender-3) |
| Fim de curso X compacto e calha/limpador de purga | [Printables](https://www.printables.com/model/1600776-minimal-x-endstop-and-purge-chutewiper-for-ender-3/files) |
| Balde esquerdo de resíduos de purga | [Printables](https://www.printables.com/model/899282-ender-3-v3-se-left-bin-bucket-but-for-people-with/files) |
| Suporte de filamento para quatro bobinas | [MakerWorld](https://makerworld.com/pt/models/2671446-filament-spool-holder-for-4-spools#profileId-3406834) |
| Cortador opcional Ender 3 V3 SE/KE | [Printables](https://www.printables.com/model/1243521-filament-cutter-for-ender-3-v3-seke) · [Integração e calibração](docs/pt-BR/Ender3_V3_SE_Cutter.md) |

Baixe modelos nas páginas dos autores. Não são duplicados aqui; cada modelo
mantém sua licença e atribuição ao criador.

<a id="backups-and-privacy"></a>
## Backups e privacidade

Snapshots de máquinas, ambientes de serviço, estado, IDs de dispositivo e
exportações pessoais do fatiador não são modelos públicos. Guarde-os de forma
privada. As ferramentas de backup são locais e nunca enviam resultados.
Commits históricos podem conter arquivos removidos da árvore atual; remover
um arquivo não apaga o histórico Git. Troque qualquer credencial publicada.
Copyright e atribuição upstream são preservados intencionalmente.

<a id="development-and-validation"></a>
## Desenvolvimento e validação

Veja o [guia de desenvolvimento local](docs/pt-BR/Development.md) para Linux/WSL
e verificações completas de estilo/compilação sem conexão à impressora.

```sh
python3 -m unittest discover -s test/bmcu_integracao -p 'test_*.py'
python3 -m unittest discover -s test/e3v3se_display -p 'test_*.py'
```

O CI compila firmware GD32F303 e valida a configuração Klippy completa.
Backups e restauração usam dados sintéticos. Testes físicos confirmaram
captura, retirada por switch, purga/limpeza e impressão PETG em duas cores
em uma montagem. Outras placas, materiais, geometrias de purga e impressão
em quatro cores exigem testes físicos próprios.

<a id="documentation"></a>
## Documentação

- [Instalação, gravação e comissionamento](docs/pt-BR/Ender3_V3_SE.md)
- [Integração por switch, atualização e backups privados](docs/pt-BR/Ender3_V3_SE_Configuracao_Validada.md)
- [BMCU: configuração, ligação e calibração](docs/pt-BR/Ender3_V3_SE_BMCU.md)
- [Cortador opcional, servo e reset manual](docs/pt-BR/Ender3_V3_SE_Cutter.md)
- [OrcaSlicer](profiles/orca/ender3-v3-se-bmcu/README.pt-BR.md)
- [Arquitetura da tela](docs/pt-BR/E3V3SE_Display.md)
- [Análise da adaptação e proveniência](docs/pt-BR/E3V3SE_Port_Analysis.md)
- [Documentação oficial do Klipper](https://www.klipper3d.org/)

<a id="credits-and-provenance"></a>
## Créditos e proveniência

Este repositório preserva o histórico Git do Klipper e é distribuído sob a
mesma GNU GPLv3. O trabalho para Ender 3 V3 SE foi possível graças a:

- [Klipper](https://github.com/Klipper3d/klipper), criado por Kevin O'Connor e
  mantido pelos contribuidores Klipper, pelo firmware, arquitetura host,
  sistema de compilação, documentação e código GPLv3.
- [Joao Pedro Curti](https://github.com/jpcurti) e contribuidores do
  [ender3-v3-se-klipper-with-display](https://github.com/jpcurti/ender3-v3-se-klipper-with-display),
  pela integração original da tela, comportamento, pesquisa de protocolo e
  histórico de issues que fundamentaram esta adaptação reescrita.
- [odwdinc/DWIN_T5UIC1_LCD](https://github.com/odwdinc/DWIN_T5UIC1_LCD), pelo
  trabalho DWIN T5L anterior em que a implementação TJC3224 se baseou.
- [E4ST2W3ST](https://github.com/E4ST2W3ST), pela
  [proposta de ponte serial Klipper](https://github.com/Klipper3d/klipper/commit/6469418d73be6743a7130b50fdb5a57d311435ca).
- [Clark Scheff / 0xD34D](https://github.com/0xD34D), pelo
  [fork da máquina](https://github.com/0xD34D/klipper_ender3_v3_se) e
  [referência de configuração](https://github.com/0xD34D/ender3-v3-se-klipper-config)
  usados no estudo de hardware, configurações comunitárias e PRTouch histórico.
- [huhen/Ender3_V3_SE_for_klipper](https://github.com/huhen/Ender3_V3_SE_for_klipper),
  GPLv3, pelo pinout CR4NS200320C13 documentado e configuração usados como base
  elétrica licenciada desta adaptação.
- [bootuz-dinamon](https://github.com/bootuz-dinamon/ender3-v3-se-full-klipper)
  e [FinalX1992](https://github.com/FinalX1992), por descobertas comunitárias de
  hardware que ajudaram a conferir UART TMC e configuração dos parafusos.
- Todos que relataram, diagnosticaram e corrigiram issues nesses projetos.
- [Pawel Jarczak / jarczakpawel](https://github.com/jarczakpawel) e contribuidores
  [BMCU-Klipper](https://github.com/jarczakpawel/BMCU-Klipper), pelo transporte
  externo de filamento e contrato genérico de endpoint.

Revisões e proveniência detalhadas dos commits estão em
[`docs/E3V3SE_Port_Analysis.md`](docs/pt-BR/E3V3SE_Port_Analysis.md).

<a id="license"></a>
## Licença

Klipper, software e configurações distribuídos por este fork são Software Livre
sob a [GNU General Public License, versão 3](COPYING). Fontes mantêm os avisos
aplicáveis de copyright/licença. A fonte correspondente completa de cada release
binária é o estado do repositório na respectiva tag.

Não redistribuímos firmware de tela Creality, pacotes de imagens ou outros
binários proprietários. Creality, Ender, DWIN e TJC podem ser marcas dos seus
titulares. Este projeto não é afiliado nem endossado por Creality, DWIN, TJC
ou pelo projeto Klipper upstream.
