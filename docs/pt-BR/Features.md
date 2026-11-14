<a id="features"></a>
# Recursos

[English](../Features.md) | **Português brasileiro**

O Klipper tem vários recursos atraentes:

* Movimento de motor de passo de alta precisão. O Klipper utiliza um processador de aplicação (como um Raspberry Pi de baixo custo) ao calcular os movimentos da impressora. O processador de aplicação determina quando acionar cada motor de passo, comprime esses eventos, transmite-os ao microcontrolador, e então o microcontrolador executa cada evento no momento solicitado. Cada evento de motor de passo é agendado com precisão de 25 microssegundos ou melhor. O software não usa estimativas cinemáticas (como o algoritmo de Bresenham) - em vez disso, calcula tempos de passo precisos com base na física da aceleração e na física da cinemática da máquina. Um movimento de motor de passo mais preciso proporciona operação da impressora mais silenciosa e estável.
* Desempenho de primeira classe. O Klipper é capaz de atingir altas taxas de passos tanto em microcontroladores novos quanto antigos. Mesmo microcontroladores antigos de 8 bits podem obter taxas acima de 175K passos por segundo. Em microcontroladores mais recentes, são possíveis vários milhões de passos por segundo. Taxas mais altas de motor de passo permitem velocidades de impressão mais altas. A temporização dos eventos de motor de passo permanece precisa mesmo em altas velocidades, o que melhora a estabilidade geral.
* O Klipper suporta impressoras com vários microcontroladores. Por exemplo, um microcontrolador poderia ser usado para controlar uma extrusora, enquanto outro controla os aquecedores da impressora, enquanto um terceiro controla o restante da impressora. O software hospedeiro do Klipper implementa sincronização de relógio para compensar a deriva de relógio entre microcontroladores. Nenhum código especial é necessário para ativar vários microcontroladores - bastam algumas linhas extras no arquivo de configuração.
* Configuração por meio de arquivo de configuração simples. Não há necessidade de regravar o microcontrolador para alterar uma configuração. Toda a configuração do Klipper é armazenada em um arquivo de configuração padrão que pode ser facilmente editado. Isso facilita configurar e manter o hardware.
* O Klipper suporta "Smooth Pressure Advance" - um mecanismo para compensar os efeitos da pressão dentro de uma extrusora. Isso reduz a "baba" da extrusora e melhora a qualidade dos cantos impressos. A implementação do Klipper não introduz mudanças instantâneas de velocidade da extrusora, o que melhora a estabilidade e a robustez gerais.
* O Klipper suporta "Input Shaping" para reduzir o impacto das vibrações na qualidade de impressão. Isso pode reduzir ou eliminar "ringing" (também conhecido como "ghosting", "echoing" ou "rippling") nas impressões. Também pode permitir obter velocidades de impressão mais rápidas mantendo alta qualidade de impressão.
* O Klipper usa um "solucionador iterativo" para calcular tempos de passo precisos a partir de equações cinemáticas simples. Isso facilita portar o Klipper para novos tipos de robôs e mantém a temporização precisa mesmo com cinemáticas complexas (não é necessária "segmentação de linha").
* O Klipper é agnóstico a hardware. Deve-se obter a mesma temporização precisa independentemente do hardware eletrônico de baixo nível. O código do microcontrolador do Klipper foi projetado para seguir fielmente o cronograma fornecido pelo software hospedeiro do Klipper (ou alertar o usuário de forma proeminente se não conseguir). Isso facilita usar o hardware disponível, atualizar para hardware novo e ter confiança no hardware.
* Código portátil. O Klipper funciona em microcontroladores ARM, AVR, PRU e outros. Impressoras existentes estilo "reprap" podem rodar o Klipper sem modificação de hardware - basta adicionar um Raspberry Pi. O leiaute interno do código do Klipper facilita suportar outras arquiteturas de microcontrolador também.
* Código mais simples. O Klipper usa uma linguagem de altíssimo nível (Python) para a maior parte do código. Os algoritmos de cinemática, a análise de G-code, os algoritmos de aquecimento e de termistor, etc., são todos escritos em Python. Isso facilita desenvolver novas funcionalidades.
* Macros programáveis personalizadas. Novos comandos de G-Code podem ser definidos no arquivo de configuração da impressora (não são necessárias alterações de código). Esses comandos são programáveis - permitindo produzir ações diferentes dependendo do estado da impressora.
* Servidor de API integrado. Além da interface padrão de G-Code, o Klipper suporta uma rica interface de aplicação baseada em JSON. Isso permite que programadores criem aplicações externas com controle detalhado da impressora.

<a id="additional-features"></a>
## Recursos adicionais

O Klipper suporta muitos recursos padrão de impressoras 3d:

* Várias interfaces web disponíveis. Funciona com Mainsail, Fluidd, OctoPrint e outras. Isso permite controlar a impressora usando um navegador web comum. O mesmo Raspberry Pi que executa o Klipper também pode executar a interface web.
* Suporte padrão a G-Code. Comandos comuns de g-code produzidos por "fatiadores" típicos (SuperSlicer, Cura, PrusaSlicer, etc.) são suportados.
* Suporte a várias extrusoras. Extrusoras com aquecedores compartilhados e extrusoras em carros independentes (IDEX) também são suportadas.
* Suporte a impressoras estilo cartesiana, delta, corexy, corexz, hybrid-corexy, hybrid-corexz, deltesian, delta rotativa, polar e guincho de cabos.
* Suporte a nivelamento automático da mesa. O Klipper pode ser configurado para detecção básica de inclinação da mesa ou nivelamento completo por malha da mesa. A malha da mesa pode ser personalizada para o tamanho da impressão (malha adaptativa da mesa). Se a mesa usar vários motores de passo Z, o Klipper também pode nivelar manipulando os motores de passo Z de forma independente. A maioria das sondas de altura Z é suportada, incluindo sondas BL-Touch e sondas acionadas por servo. As sondas podem ser calibradas para compensação de torção do eixo. Se estiver usando uma "sonda de corrente parasita", pode-se utilizar varredura rápida da malha da mesa,
* Suporte a calibração automática de delta. A ferramenta de calibração pode realizar calibração básica de altura, bem como uma calibração aprimorada das dimensões X e Y. A calibração pode ser feita com uma sonda de altura Z ou por sondagem manual.
* Suporte a "exclude object" em tempo de execução. Quando configurado, este módulo pode facilitar o cancelamento de apenas um objeto em uma impressão com várias peças.
* Suporte a sensores comuns de temperatura (por exemplo, termistores comuns, AD595, AD597, AD849x, PT100, PT1000, MAX6675, MAX31855, MAX31856, MAX31865, BME280, HTU21D, DS18B20, AHT1X, AHT2X, AHT3X, SHT3x e LM75). Termistores personalizados e sensores analógicos personalizados de temperatura também podem ser configurados. Pode-se monitorar o sensor interno de temperatura do microcontrolador e o sensor interno de temperatura de um Raspberry Pi.
* Proteção térmica básica do aquecedor ativada por padrão.
* Suporte a ventiladores padrão, ventiladores do bico e ventiladores controlados por temperatura. Não é preciso manter os ventiladores funcionando quando a impressora está ociosa. A velocidade do ventilador pode ser monitorada em ventiladores com tacômetro. Pode-se atribuir uma "fórmula matemática" a um ventilador para atualização automática da velocidade do ventilador.
* Suporte a configuração em tempo de execução de drivers de motor de passo TMC2130, TMC2208/TMC2224, TMC2209, TMC2240, TMC2660, TMC5160 e TMC5262. Há também suporte a controle de corrente de drivers tradicionais de motor de passo via AD5206, DAC084S085, MCP4451, MCP4728, MCP4018 e pinos PWM.
* Suporte a telas LCD comuns conectadas diretamente à impressora. Um menu padrão também está disponível. O conteúdo da tela e do menu pode ser totalmente personalizado via arquivo de configuração.
* Aceleração constante e suporte a "look-ahead". Todos os movimentos da impressora acelerarão gradualmente da parada até a velocidade de cruzeiro e depois desacelerarão de volta à parada. O fluxo recebido de comandos de movimento em G-Code é enfileirado e analisado - a aceleração entre movimentos em direção semelhante será otimizada para reduzir paradas da impressão e melhorar o tempo total de impressão.
* O Klipper implementa um algoritmo de "fim de curso por fase do motor de passo" que pode melhorar a precisão de chaves típicas de fim de curso. Quando devidamente ajustado, pode melhorar a adesão da primeira camada da impressão à mesa.
* Suporte a sensores de presença de filamento, sensores de movimento de filamento e sensores de largura de filamento.
* Suporte a medição e registro de aceleração usando acelerômetros adxl345, mpu9250, mpu6050, lis2dw12, lis3dh e icm20948.
* Suporte a limitar a velocidade máxima de movimentos curtos em "zigue-zague" para reduzir vibração e ruído da impressora. Consulte o documento [cinemática](Kinematics.md) para mais informações.
* Arquivos de configuração de exemplo estão disponíveis para muitas impressoras comuns. Verifique o [diretório config](../../config) para uma lista.

Para começar a usar o Klipper, leia o guia de [instalação](Installation.md).

<a id="step-benchmarks"></a>
## Referências de passos

Abaixo estão os resultados de testes de desempenho dos motores de passo. Os números mostrados representam o número total de passos por segundo no microcontrolador.

| Microcontrolador | 1 motor de passo ativo | 3 motores de passo ativos |
| --- | --- | --- |
| 16Mhz AVR | 157K | 99K |
| 20Mhz AVR | 196K | 123K |
| SAMD21 | 686K | 471K |
| STM32F042 | 814K | 578K |
| Beaglebone PRU | 866K | 708K |
| STM32G0B1 | 1103K | 790K |
| STM32F103 | 1180K | 818K |
| SAM3X8E | 1273K | 981K |
| SAM4S8C | 1690K | 1385K |
| LPC1768 | 1923K | 1351K |
| LPC1769 | 2353K | 1622K |
| SAM4E8E | 2500K | 1674K |
| SAMD51 | 3077K | 1885K |
| AR100 | 3529K | 2507K |
| STM32G431 | 3617K | 2452K |
| STM32F407 | 3652K | 2459K |
| STM32F446 | 3913K | 2634K |
| RP2040 | 4000K | 2571K |
| RP2350 | 4167K | 2663K |
| SAME70 | 6667K | 4737K |
| STM32H723 | 7429K | 8619K |

Se não souber qual é o microcontrolador de uma determinada placa, localize o [arquivo de configuração](../../config) apropriado e procure o nome do microcontrolador nos comentários no topo desse arquivo.

Mais detalhes sobre as referências estão disponíveis no documento [Benchmarks](Benchmarks.md).
