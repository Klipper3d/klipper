<a id="overview"></a>
# Visão geral

[English](../Overview.md) | **Português brasileiro**

Bem-vindo à documentação do Klipper. Se for novo no Klipper, comece com os documentos de [recursos](Features.md) e [Instalação](Installation.md).

<a id="overview-information"></a>
## Informações gerais

- [Recursos](Features.md): Uma lista de alto nível dos recursos do Klipper.
- [FAQ](FAQ.md): Perguntas frequentes.
- [Lançamentos](Releases.md): O histórico de lançamentos do Klipper.
- [Alterações de Configuração](Config_Changes.md): Mudanças recentes no software que podem exigir que os usuários atualizem o arquivo de configuração da impressora.
- [Contato](Contact.md): Informações sobre como reportar erros (bugs) e comunicação geral com os desenvolvedores do Klipper.

<a id="installation-and-configuration"></a>
## Instalação e Configuração

- [Instalação](Installation.md): Guia de instalação do Klipper.
   - [Octoprint](OctoPrint.md): Guia de instalação do Octoprint com o Klipper.
- [Referência de Configuração](Config_Reference.md): Descrição dos parâmetros de configuração.
   - [Distância de Rotação](Rotation_Distance.md): Calculando o parâmetro do motor de passo "rotation_distance".
- [Verificação de Configuração](Config_checks.md): Verificar as configurações básicas dos pinos no arquivo de configuração.
- [Nivelamento da Cama](Bed_Level.md): Informações sobre o "nivelamento da cama" no Klipper.
   - [Calibração Delta](Delta_Calibrate.md): Calibração da cinemática delta.
   - [Calibração da Sonda](Probe_Calibrate.md): Calibração das sondas automáticas de Z.
   - [BL-Touch](BLTouch.md): Configurar uma sonda Z "BL-Touch".
   - [Nivelamento Manual](Manual_Level.md): Calibração de fim de curso de Z (e similares).
   - [Malha da Cama](Bed_Mesh.md): Correção de altura da cama baseada em coordenadas XY.
   - [Fase do Fim de Curso](Endstop_Phase.md): Posicionamento do fim de curso de Z assistido por motor de passo.
   - [Compensação de torção do eixo](Axis_Twist_Compensation.md): Uma ferramenta para compensar leituras imprecisas da sonda devido à torção no pórtico X.
- [Compensação de ressonância](Resonance_Compensation.md): Uma ferramenta para reduzir o ringing (vibrações indesejadas) nas impressões.
   - [Medindo ressonâncias](Measuring_Resonances.md): Informações sobre como usar um acelerômetro adxl345 para medir ressonâncias.
- [Avanço de pressão](Pressure_Advance.md): Calibrar a pressão do extrusor.
- [G-Codes](G-Codes.md): Informações sobre comandos suportados pelo Klipper.
- [Modelos de comandos](Command_Templates.md): Macros G-Code e avaliação condicional.
   - [Referência de Status](Status_Reference.md): Informações disponíveis para macros (e similares).
- [Drivers TMC](TMC_Drivers.md): Usando drivers de motor de passo Trinamic com Klipper.
- [Homing Multi-MCU](Multi_MCU_Homing.md): Homing e sondagem usando múltiplos microcontroladores.
- [Slicers](Slicers.md): Configurar o software "slicer" (fatiador) para o Klipper.
- [Correção de Distorção](Skew_Correction.md): Ajustes para eixos não perfeitamente quadrados.
- [Ferramentas PWM](Using_PWM_Tools.md): Guia sobre como usar ferramentas controladas por PWM, como lasers ou fresas.
- [Excluir Objeto](Exclude_Object.md): O guia para a implementação de Excluir Objetos.

<a id="developer-documentation"></a>
## Documentação do Desenvolvedor

- [Visão Geral do Código](Code_Overview.md): Os desenvolvedores devem ler isso primeiro.
- [Cinemática](Kinematics.md): Detalhes técnicos de como o Klipper implementa o movimento.
- [Protocolo](Protocol.md): Informações sobre o protocolo de mensagens de baixo nível entre host e microcontrolador.
- [Servidor API](API_Server.md): Informações sobre a API de comando e controle do Klipper.
- [Comandos MCU](MCU_Commands.md): Uma descrição dos comandos de baixo nível implementados no software do microcontrolador.
- [Protocolo de barramento CAN](CANBUS_protocol.md): Formato de mensagem do barramento CAN do Klipper.
- [Depuração](Debugging.md): Informações sobre como testar e depurar o Klipper.
- [Benchmarks](Benchmarks.md): Informações sobre o método de medição de performance do Klipper.
- [Contribuindo](CONTRIBUTING.md): Informações sobre como enviar melhorias para o Klipper.
- [Empacotamento](Packaging.md): Informações sobre a construção de pacotes de sistema operacional.

<a id="device-specific-documents"></a>
## Documentos de Dispositivos Específicos

- [Configurações de exemplo](Example_Configs.md): Informação de como adicionar um arquivo de configuração de exemplo no Klipper.
- [Atualizações do Cartão SD](SDCard_Updates.md): Programar um microcontrolador copiando um binário para um cartão SD no microcontrolador.
- [Raspberry Pi como Microcontrolador](RPi_microcontroller.md): Detalhes para controlar dispositivos conectados aos pinos GPIO de um Raspberry Pi.
- [Beaglebone](Beaglebone.md): Detalhes para rodar Klipper no Beaglebone PRU.
- [Bootloaders](Bootloaders.md): Informações de desenvolvedor sobre a gravação de microcontrolador.
- [Entrada do bootloader](Bootloader_Entry.md): Solicitando o bootloader.
- [Barramento CAN](CANBUS.md): Informações sobre o uso do barramento CAN com Klipper.
   - [Solução de problemas do barramento CAN](CANBUS_Troubleshooting.md): Dicas para solução de problemas do barramento CAN.
- [Sensor de largura de filamento TSL1401CL](TSL1401CL_Filament_Width_Sensor.md)
- [Sensor de largura de filamento Hall](Hall_Filament_Width_Sensor.md)
- [Sonda indutiva de corrente parasita](Eddy_Probe.md)
- [Células de carga](Load_Cell.md)
