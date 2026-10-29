# Klipper para Ender 3 V3 SE + BMCU

[English](README.md) | **Português brasileiro**

Fork não oficial do Klipper para a **Creality Ender 3 V3 SE**, com suporte à
tela original e alimentação opcional BMCU de quatro canais. Os quatro canais
compartilham uma única extrusora física. O BMCU-Klipper é instalado separadamente.

## Versão 1.0.0

- Tela original TJC3224/DWIN de 3,2 polegadas, em inglês ou português brasileiro.
- Integração com BMCU-Klipper 1.0.6 e firmware BMCU 1.0.1.
- Chegada por switch, captura com a extrusora girando primeiro, purga, limpeza
  e retirada assistida até liberar o sensor.
- Tratamento do `T0` automático do OrcaSlicer 2.4.2 pelo mapa de impressão.
- Atualização do Klipper oficial até `0cee841e31d701595b3e509a0b502f6e320f52a8`.
- Testes automatizados, compilação do firmware e arquivos com checksums.

A versão 1.0.0 identifica este fork, sem alterar a versão do pacote ou do
firmware BMCU. Consulte as [notas da release](docs/releases/v1.0.0.md), em inglês,
para conhecer a validação e as limitações. A instalação exige calibração física.

## Hardware suportado

O alvo deste firmware é o **GD32F303RET6**, na placa CR4NS200320C14 verificada,
usando o alvo compatível com STM32F103, bootloader de 28 KiB e USART1. Confira
a identificação do chip; o código da placa sozinho não basta.
**Não grave esse binário em uma placa STM32F401.**

O CR-Touch usa o suporte do Klipper oficial. A implementação histórica de
Z-offset automático PRTouch/HX711 não faz parte deste fork. Deslocamentos,
GPIOs, limites e distâncias do filamento precisam ser conferidos na montagem.

## Instalação e atualização

1. Leia o [guia de hardware e gravação](docs/Ender3_V3_SE.md), em inglês.
2. Faça um backup privado antes de alterar uma instalação existente.
3. Use o código do host e o firmware da mesma tag. Os arquivos `klipper.bin`,
   `klipper.dict`, `defconfig`, `BUILD-METADATA.txt` e `SHA256SUMS` ficam nas
   [Releases](https://github.com/bernardokcosta/ender3-v3-se-klipper-bmcu/releases).
4. Em uma instalação nova, use o arquivo principal em `config/` e os módulos
   de `config/ender3-v3-se/`. Não substitua um `printer.cfg` já calibrado pelo modelo.
5. Para usar a BMCU, instale primeiro o pacote oficial BMCU-Klipper 1.0.6 e seus
   serviços. Depois siga o [guia da integração](docs/Ender3_V3_SE_Configuracao_Validada.md).

Para atualizar a integração em uma máquina já configurada, com a impressora
ociosa, os canais estacionados e o sensor livre, execute neste checkout:

```sh
sudo python3 scripts/ender3-v3-se/install_multicolor.py
```

O instalador faz backup, preserva os ajustes existentes e restaura os arquivos
anteriores se o Klipper não ficar pronto. Atualizar apenas o Git não substitui
os módulos opcionais já instalados em `klippy/extras/`.

O atalho **Filamento** da tela e os botões do Mainsail compartilham os
pré-aquecimentos PLA/PETG/TPU, a carga dos quatro canais físicos e a retirada
automática a 230 °C. Ao carregar outro canal, o anterior é retirado e estacionado
antes da nova carga e purga. As temperaturas de pré-aquecimento ficam em
`bmcu-controles-ajustes.cfg`, preservado nas próximas instalações. Os ajustes
da sequência calibrada permanecem em `bmcu-multicor-ender.cfg`.

Na seção `[e3v3se_display]`, use `language: pt_BR` para a tela em português.
O código-fonte e a documentação mantida pelo projeto ficam em inglês. Nomes
antigos de comandos, opções de configuração e campos de backup em português
são mantidos para compatibilidade com instalações existentes.

## OrcaSlicer

Use os [G-codes reutilizáveis](profiles/orca/ender3-v3-se-bmcu/README.md).
Antes de fatiar, substitua `YOUR_BMCU_UID` pelo UID da sua BMCU. Com o mapa de
impressão confirmado, `T0` a `T3` selecionam as ferramentas lógicas 1 a 4;
o mapa determina o canal físico. Fora desse contexto, os comandos nativos
mantêm o significado original, incluindo a entrada externa de `T0`.
Não é necessário instalar scripts de pós-processamento no computador.

## Backups e privacidade

Snapshots de máquinas, arquivos de ambiente, estado de execução, identificadores
de dispositivos e exportações pessoais do fatiador devem ficar em armazenamento
privado. Os utilitários de backup não enviam arquivos para serviços externos.
Arquivos removidos da árvore atual ainda podem existir no histórico do Git;
credenciais publicadas precisam ser trocadas. Os créditos e avisos de licença
são preservados.

## Testes e limitações

```sh
python3 -m unittest discover -s test/bmcu_integracao -p 'test_*.py'
python3 -m unittest discover -s test/e3v3se_display -p 'test_*.py'
```

A integração foi testada fisicamente em uma montagem, incluindo uma impressão
PETG de duas cores. Outras revisões de placa, materiais, mecanismos de purga e
impressões de quatro cores exigem validação própria. Os testes de backup usam
dados fictícios; não são uma cópia de uma impressora pessoal.

## Documentação, créditos e licença

A lista completa de guias, autores e projetos de origem está no
[README em inglês](README.md#credits-and-provenance). Este fork preserva o
histórico e a licença GNU GPLv3 do Klipper. Consulte [COPYING](COPYING).
O projeto não tem afiliação com a Creality, DWIN, TJC ou o Klipper oficial.
