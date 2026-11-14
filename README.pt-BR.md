# Klipper para Ender 3 V3 SE + BMCU

[English](README.md) | **Português brasileiro**

[![Ender CI](https://github.com/bernardokcosta/ender3-v3-se-klipper-bmcu/actions/workflows/ender3-v3-se.yml/badge.svg)](https://github.com/bernardokcosta/ender3-v3-se-klipper-bmcu/actions/workflows/ender3-v3-se.yml)
[![Release](https://img.shields.io/github/v/release/bernardokcosta/ender3-v3-se-klipper-bmcu)](https://github.com/bernardokcosta/ender3-v3-se-klipper-bmcu/releases)
[![GPLv3](https://img.shields.io/badge/license-GPLv3-blue.svg)](COPYING)

Fork não oficial do Klipper para a **Creality Ender 3 V3 SE**, combinando
suporte à tela original e transporte opcional de filamento BMCU de quatro
canais. Uma extrusora física atende aos quatro canais. O BMCU-Klipper
continua sendo uma dependência separada.

<a id="version-200"></a>
<a id="features"></a>
## Recursos

- Tela original TJC3224/DWIN de 3,2 polegadas, em inglês e português brasileiro.
- Integração BMCU 1.0.6 / firmware 1.0.1 por endpoint genérico de extrusora única.
- Chegada por switch, captura iniciada pela extrusora, purga, limpeza e retirada
  assistida.
- Perfil OrcaSlicer 2.4.2 para impressão multimaterial com uma extrusora.
- Comandos, parâmetros e campos de status da integração somente em inglês.

Veja as [notas da release](docs/pt-BR/releases/v2.0.0.md) para as alterações
de cada versão.

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
<a id="installation"></a>
## Instalação

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

Para instalações existentes, siga as
[instruções de atualização da release](docs/pt-BR/releases/v2.0.0.md#upgrade-and-rollback).

<a id="operation"></a>
## Operação

Controle filamento e temperatura pela tela original ou pelo Mainsail. Veja os
[controles diários de filamento](docs/pt-BR/Ender3_V3_SE_Configuracao_Validada.md#everyday-filament-controls)
para carga de canais, retirada, pré-aquecimento e reset protegido, e a
[recuperação da impressão](docs/pt-BR/Ender3_V3_SE_Print_Recovery.md) para falhas
na troca de filamento.

Escolha **Configurações → Idioma → English / Português** na tela para aplicar
imediatamente. **Configurações → Salvar ajustes** persiste e reinicia o Klipper
após confirmação quando parado. O idioma da tela não altera a API de comandos
em inglês.

O [perfil de formação da ponta e teste de recarga em quatro cores](docs/pt-BR/Ender3_V3_SE_Tip_Forming.md)
é experimental e exige calibração física. O
[cortador opcional com servo na Orange Pi](docs/pt-BR/Ender3_V3_SE_Cutter.md)
vem desabilitado e exige coordenadas de corte medidas e ligação GPIO verificada.

<a id="orcaslicer"></a>
## OrcaSlicer

Use os [trechos de início, fim e troca](profiles/orca/ender3-v3-se-bmcu/README.pt-BR.md).
Troque `YOUR_BMCU_UID` pelo UID da sua BMCU antes de fatiar. O guia do perfil
lista os ajustes necessários da impressora e o mapeamento dos canais.

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

<a id="development-and-validation"></a>
## Desenvolvimento e validação

Veja o [guia de desenvolvimento local](docs/pt-BR/Development.md) para configurar
o ambiente Linux/WSL, executar testes e compilar o projeto.

```sh
python3 -m unittest discover -s test/bmcu_integracao -p 'test_*.py'
python3 -m unittest discover -s test/e3v3se_display -p 'test_*.py'
python3 -m unittest discover -s test/public_docs -p 'test_*.py'
python3 scripts/check_public_docs.py
```

O CI executa esses testes, compila o firmware GD32F303 e valida a configuração
do Klippy.

<a id="documentation"></a>
## Documentação

- [Instalação, gravação e comissionamento](docs/pt-BR/Ender3_V3_SE.md)
- [Integração BMCU por switch e calibração](docs/pt-BR/Ender3_V3_SE_Configuracao_Validada.md)
- [BMCU: configuração, ligação e calibração](docs/pt-BR/Ender3_V3_SE_BMCU.md)
- [Cortador opcional, servo e reset manual](docs/pt-BR/Ender3_V3_SE_Cutter.md)
- [OrcaSlicer](profiles/orca/ender3-v3-se-bmcu/README.pt-BR.md)
- [Arquitetura da tela](docs/pt-BR/E3V3SE_Display.md)
- [Análise da adaptação e proveniência](docs/pt-BR/E3V3SE_Port_Analysis.md)
- [Documentação oficial do Klipper](https://www.klipper3d.org/)

<a id="credits-and-provenance"></a>
<a id="credits"></a>
## Créditos

Este fork se baseia no trabalho de:

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

Referências adicionais: [análise da adaptação](docs/pt-BR/E3V3SE_Port_Analysis.md)
e [créditos das traduções](docs/pt-BR/Translations.md).

<a id="license"></a>
## Licença

Distribuído sob a [GNU General Public License, versão 3](COPYING).
Os arquivos-fonte mantêm seus avisos aplicáveis de copyright e licença.
