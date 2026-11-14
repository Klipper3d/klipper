<a id="api-server"></a>
# Servidor de API

[English](../API_Server.md) | **Português brasileiro**

Este documento descreve a Interface do Programador de Aplicativos (API) da Klipper. Esta interface permite que aplicativos externos consultem e controlem o software de host Klipper.

<a id="enabling-the-api-socket"></a>
## Habilitando o soquete de API

Para usar o servidor API, o software host klippy.py deve ser iniciado com o parâmetro `-a`. Por exemplo:

```
~/klippy-env/bin/python ~/klipper/klippy/klippy.py ~/printer.cfg -a /tmp/klippy_uds -l /tmp/klippy.log
```

Isso faz com que o software host crie um Soquete de domínio Unix, um cliente pode então abrir uma conexão nesse soquete e enviar comandos para Klipper.

Consulte o projeto [Moonraker](https://github.com/Arksine/moonraker) para obter uma ferramenta popular que pode encaminhar solicitações HTTP para o soquete de domínio Unix do servidor de API do Klipper.

<a id="request-format"></a>
## Formato da solicitação

As mensagens enviadas e recebidas no soquete são strings codificadas em JSON terminadas por um caractere ASCII 0x03:

```
<json_object_1><0x03><json_object_2><0x03>...
```

O Klipper contém uma ferramenta `scripts/whconsole.py` que pode executar o enquadramento de mensagem acima. Por exemplo:

```
~/klipper/scripts/whconsole.py /tmp/klippy_uds
```

Esta ferramenta pode ler uma série de comandos JSON do stdin, enviá-los ao Klipper e relatar os resultados. A ferramenta espera que cada comando JSON esteja em uma única linha, e anexará automaticamente o terminador 0x03 ao transmitir uma solicitação. (O servidor API Klipper não possui um requisito de nova linha.)

<a id="api-protocol"></a>
## Protocolo de API

O protocolo de comando usado no soquete de comunicação é inspirado em [json-rpc](https://www.jsonrpc.org/).

Uma solicitação pode ser semelhante a:

`{"id": 123, "method": "info", "params": {}}`

e uma resposta pode ser semelhante a:

`{"id": 123, "result": {"state_message": "Printer is ready", "klipper_path": "/home/pi/klipper", "config_file": "/home/pi/printer.cfg", "software_version": "v0.8.0-823-g883b1cb6", "hostname": "octopi", "cpu_info": "4 core ARMv7 Processor rev 4 (v7l)", "state": "ready", "python_path": "/home/pi/klippy-env/bin/python", "log_file": "/tmp/klippy.log"}}`

Cada solicitação deve ser um dicionário JSON. (Este documento usa o termo "dicionário" do Python para descrever um "objeto JSON" - um mapeamento de chaves/valores contidos em `{}`.)

O dicionário da solicitação deve conter um parâmetro "method" que é o nome da string de um "endpoint" disponível do Klipper.

O dicionário da solicitação pode conter um parâmetro "params" que deve ser do tipo dicionário. O campo "params" fornece informações adicionais para o "endpoint" do Klipper que trata a solicitação. Seu conteúdo é específico desse endpoint.

O dicionário da solicitação pode conter um parâmetro "id" de qualquer tipo JSON. Se "id" estiver presente, o Klipper responderá à solicitação com uma mensagem contendo esse "id". Se "id" for omitido (ou definido como o valor JSON "null"), o Klipper não fornecerá nenhuma resposta à solicitação. Uma mensagem de resposta é um dicionário JSON contendo "id" e "result". O campo "result" é sempre um dicionário; seu conteúdo é específico do endpoint que trata a solicitação.

Se o processamento de uma solicitação resultar em erro, a mensagem de resposta conterá um campo "error" em vez de um campo "result". Por exemplo, a solicitação: `{"id": 123, "method": "gcode/script", "params": {"script": "G1 X200"}}` pode resultar em uma resposta de erro como: `{"id": 123, "error": {"message": "Must home axis first: 200.000 0.000 0.000 [0.000]", "error": "WebRequestError"}}`

O Klipper sempre começará a processar as solicitações na ordem em que forem recebidas. No entanto, algumas solicitações podem não ser concluídas imediatamente, o que pode fazer com que a resposta associada seja enviada fora de ordem em relação às outras respostas. Uma solicitação JSON nunca pausará o processamento de solicitações JSON futuras.

<a id="subscriptions"></a>
## Assinaturas (Subscriptions)

Algumas solicitações de "endpoint" do Klipper permitem "assinar" (subscribe) futuras mensagens de atualização assíncronas (async).

Por exemplo:

`{"id": 123, "method": "gcode/subscribe_output", "params": {"response_template":{"key": 345}}}`

pode responder com:

`{"id": 123, "result": {}}`

e fazer com que o Klipper envie mensagens semelhantes a:

`{"params": {"response": "ok B:22.8 /0.0 T0:22.4 /0.0"}, "key": 345}`

Uma solicitação de uma assinatura aceita um dicionário "response_template" no campo "params" da solicitação. Esse dicionário "response_template" é usado como modelo para futuras mensagens assíncronas (async) - ele pode conter pares chave/valor arbitrários. Ao enviar essas futuras mensagens assíncronas, o Klipper adicionará um campo "params" contendo um dicionário com conteúdo específico do "endpoint" ao modelo de resposta e então enviará esse modelo. Se um campo "response_template" não for fornecido, o padrão será um dicionário vazio (`{}`).

<a id="available-endpoints"></a>
## "endpoints" disponíveis

Por convenção, os "endpoints" do Klipper têm o formato `<module_name>/<some_name>`. Ao fazer uma solicitação a um "endpoint", o nome completo deve ser definido no parâmetro "method" do dicionário da solicitação (por exemplo, `{"method"="gcode/restart"}`).

<a id="info"></a>
### info

O endpoint "info" é usado para obter informações do sistema e da versão do Klipper. Também é usado para fornecer ao Klipper as informações de versão do cliente. Por exemplo: `{"id": 123, "method": "info", "params": { "client_info": { "version": "v1"}}}`

Se existir, o parâmetro "client_info" deve ser um dicionário, mas esse dicionário pode ter conteúdo arbitrário. Os clientes são incentivados a fornecer o nome do cliente e sua versão do software ao se conectarem pela primeira vez à API do Klipper.

<a id="emergency_stop"></a>
### emergency_stop

O endpoint "emergency_stop" é usado para instruir o Klipper a entrar no estado de "shutdown". Comporta-se de forma semelhante ao comando G-Code `M112`. Por exemplo: `{"id": 123, "method": "emergency_stop"}`

<a id="register_remote_method"></a>
### register_remote_method

Este endpoint permite que os clientes registrem métodos que podem ser chamados pelo klipper. Ele retornará um objeto vazio em caso de sucesso.

Por exemplo: `{"id": 123, "method": "register_remote_method", "params": {"response_template": {"action": "run_paneldue_beep"}, "remote_method": "paneldue_beep"}}` retornará: `{"id": 123, "result": {}}`

O `paneldue_beep` agora pode ser chamado pelo Klipper. Observe que se o método aceita parâmetros, eles devem ser fornecidos como argumentos de palavras-chave. Abaixo está um exemplo de como ele pode ser chamado a partir de um gcode_macro:

```
[gcode_macro PANELDUE_BEEP]
gcode:
  {action_call_remote_method("paneldue_beep", frequency=300, duration=1.0)}
```

Quando a macro PANELDUE_BEEP é executada, o Klipper enviaria algo como: `{"action": "run_paneldue_beep", "params": {"frequency": 300, "duration": 1.0}}`

<a id="objectslist"></a>
### objects/list

Este endpoint consulta a lista de "objetos" disponíveis da impressora que se pode consultar (por meio do endpoint "objects/query"). Por exemplo: `{"id": 123, "method": "objects/list"}` pode retornar: `{"id": 123, "result": {"objects": ["webhooks", "configfile", "heaters", "gcode_move", "query_endstops", "idle_timeout", "toolhead", "extruder"]}}`

<a id="objectsquery"></a>
### objects/query

Este endpoint permite consultar informações de objetos da impressora. Por exemplo: `{"id": 123, "method": "objects/query", "params": {"objects": {"toolhead": ["position"], "webhooks": null}}}` pode retornar: `{"id": 123, "result": {"status": {"webhooks": {"state": "ready", "state_message": "Printer is ready"}, "toolhead": {"position": [0.0, 0.0, 0.0, 0.0]}}, "eventtime": 3051555.377933684}}`

O parâmetro "objects" na solicitação deve ser um dicionário contendo os objetos de impressora que serão consultados - a chave contém o nome do objeto de impressora e o valor é "null" (para consultar todos os campos) ou uma lista de nomes dos campos.

A mensagem de resposta conterá um campo "status" contendo um dicionário com as informações consultadas - a chave contém o nome do objeto impressora e o valor é um dicionário contendo seus campos. A mensagem de resposta também conterá um campo “eventtime” contendo data/hora de quando a consulta foi feita.

Os campos disponíveis estão documentados no documento [Referência de status](Status_Reference.md).

<a id="objectssubscribe"></a>
### objects/subscribe

Este endpoint permite consultar e depois assinar informações de objetos da impressora. A solicitação e a resposta do endpoint são idênticas às do endpoint "objects/query". Por exemplo: `{"id": 123, "method": "objects/subscribe", "params": {"objects":{"toolhead": ["position"], "webhooks": ["state"]}, "response_template":{}}}` pode retornar: `{"id": 123, "result": {"status": {"webhooks": {"state": "ready"}, "toolhead": {"position": [0.0, 0.0, 0.0, 0.0]}}, "eventtime": 3052153.382083195}}` e resultar em mensagens assíncronas subsequentes como: `{"params": {"status": {"webhooks": {"state": "shutdown"}}, "eventtime": 3052165.418815847}}`

<a id="gcodehelp"></a>
### gcode/help

Este endpoint permite consultar comandos G-Code disponíveis que possuem uma string de ajuda. Por exemplo: `{"id": 123, "method": "gcode/help"}` pode retornar: `{"id": 123, "result": {"RESTORE_GCODE_STATE": "Restore a previously saved G-Code state", "PID_CALIBRATE": "Run PID calibration test", "QUERY_ADC": "Report the last value of an analog pin", ...}}`

<a id="gcodescript"></a>
### gcode/script

Este endpoint permite executar uma série de comandos G-Code. Por exemplo: `{"id": 123, "method": "gcode/script", "params": {"script": "G90"}}`

Se o script G-Code fornecido gerar um erro, uma resposta de erro será gerada. No entanto, se o comando G-Code produzir um output no terminal, esse output não será fornecida na resposta. (Use o endpoint "gcode/subscribe_output" para obter o output do terminal G-Code.)

Se houver um comando G-Code sendo processado quando esta solicitação for recebida, o script fornecido será enfileirado. Este atraso pode ser significativo (por exemplo, se um comando de espera por temperatura do G-Code estiver em execução). A mensagem de resposta JSON é enviada quando o processamento do script é totalmente concluído.

<a id="gcoderestart"></a>
### gcode/restart

Este endpoint permite solicitar uma reinicialização - é semelhante a executar o comando "RESTART" do G-Code. Por exemplo: `{"id": 123, "method": "gcode/restart"}`

Tal como acontece com o endpoint "gcode/script", este endpoint só é concluído após a conclusão de qualquer comando G-Code pendente.

<a id="gcodefirmware_restart"></a>
### gcode/firmware_restart

Isto é semelhante ao endpoint "gcode/restart" - ele implementa o comando de G-Code "FIRMWARE_RESTART". Por exemplo: `{"id": 123, "method": "gcode/firmware_restart"}`

Tal como acontece com o endpoint "gcode/script", este endpoint só é concluído após a conclusão de qualquer comando G-Code pendente.

<a id="gcodesubscribe_output"></a>
### gcode/subscribe_output

Este endpoint é usado para assinar mensagens do terminal de G-Code geradas pelo Klipper. Por exemplo: `{"id": 123, "method": "gcode/subscribe_output", "params": {"response_template":{}}}` pode posteriormente produzir mensagens assíncronas como: `{"params": {"response": "// Klipper state: Shutdown"}}`

Este endpoint destina-se a oferecer suporte à interação humana por meio de uma interface do terminal. A análise do conteúdo da saída do terminal G-Code não é recomendada. Use o endpoint "objects/subscribe" para obter atualizações sobre o estado do Klipper.

<a id="motion_reportdump_stepper"></a>
### motion_report/dump_stepper

Este endpoint é usado para assinar o fluxo do stepper queue_step interno do Klipper. Obter essas atualizações de movimento de baixo nível pode ser útil para fins de diagnóstico e debug. Usar este endpoint pode aumentar a carga do hardware do sistema do Klipper.

Uma solicitação pode ser semelhante a: `{"id": 123, "method":"motion_report/dump_stepper", "params": {"name": "stepper_x", "response_template": {}}}` e pode retornar: `{"id": 123, "result": {"header": ["interval", "count", "add"]}}` e pode posteriormente produzir mensagens assíncronas como: `{"params": {"first_clock": 179601081, "first_time": 8.98, "first_position": 0, "last_clock": 219686097, "last_time": 10.984, "data": [[179601081, 1, 0], [29573, 2, -8685], [16230, 4, -1525], [10559, 6, -160], [10000, 976, 0], [10000, 1000, 0], [10000, 1000, 0], [10000, 1000, 0], [9855, 5, 187], [11632, 4, 1534], [20756, 2, 9442]]}}`

O campo "header" na resposta da consulta inicial é usado para descrever os campos encontrados nas respostas de "data" (dados) posteriores.

<a id="motion_reportdump_trapq"></a>
### motion_report/dump_trapq

Este endpoint é usado para assinar a "fila de movimento trapezoidal" (trapezoid motion queue) interna do Klipper. Obter essas atualizações de movimento de baixo nível pode ser útil para fins de diagnóstico e debug. Usar este endpoint pode aumentar a carga do hardware do sistema do Klipper.

Uma solicitação pode ter a aparência: `{"id": 123, "method": "motion_report/dump_trapq", "params": {"name": "toolhead", "response_template":{}}}` e pode retornar: `{"id": 1, "result": {"header": ["time", "duration", "start_velocity", "acceleration", "start_position", "direction"]}}` e pode posteriormente produzir mensagens assíncronas, como: `{"params": {"data": [[4.05, 1.0, 0.0, 0.0, [300.0, 0.0, 0.0], [0.0, 0.0, 0.0]], [5.054, 0.001, 0.0, 3000.0, [300.0, 0.0, 0.0], [-1.0, 0.0, 0.0]]]}}`

O campo "header" na resposta da consulta inicial é usado para descrever os campos encontrados nas respostas de "data" (dados) posteriores.

<a id="adxl345dump_adxl345"></a>
### adxl345/dump_adxl345

Este endpoint é usado para assinar (subscribe) dados do acelerômetro ADXL345. A obtenção dessas atualizações de movimento de baixo nível pode ser útil para fins de diagnóstico e debug. Usar este endpoint pode aumentar a carga do hardware do sistema do Klipper.

Uma solicitação pode ser semelhante a: `{"id": 123, "method":"adxl345/dump_adxl345", "params": {"sensor": "adxl345", "response_template": {}}}` e pode retornar: `{"id": 123,"result":{"header":["time","x_acceleration","y_acceleration", "z_acceleration"]}}` e pode posteriormente produzir mensagens assíncronas como: `{"params":{"overflows":0,"data":[[3292.432935,-535.44309,-1529.8374,9561.4], [3292.433256,-382.45935,-1606.32927,9561.48375]]}}`

O campo "header" na resposta da consulta inicial é usado para descrever os campos encontrados nas respostas de "data" (dados) posteriores.

<a id="angledump_angle"></a>
### angle/dump_angle

Este endpoint é usado para assinar [dados do sensor de ângulo](Config_Reference.md#angle). A obtenção dessas atualizações de movimento de baixo nível pode ser útil para fins de diagnóstico e debug. Usar este endpoint pode aumentar a carga do hardware do sistema do Klipper.

Uma solicitação pode ser semelhante a: `{"id": 123, "method":"angle/dump_angle", "params": {"sensor": "my_angle_sensor", "response_template": {}}}` e pode retornar: `{"id": 123,"result":{"header":["time","angle"]}}` e pode posteriormente produzir mensagens assíncronas como: `{"params":{"position_offset":3.151562,"errors":0, "data":[[1290.951905,-5063],[1290.952321,-5065]]}}`

O campo "header" na resposta da consulta inicial é usado para descrever os campos encontrados nas respostas de "data" (dados) posteriores.

<a id="load_celldump_force"></a>
### load_cell/dump_force

Este endpoint é usado para assinar os dados de força produzidos por uma load_cell. O uso deste endpoint pode aumentar a carga do sistema do Klipper.

Uma requisição pode ter este aspecto: `{"id": 123, "method":"load_cell/dump_force", "params": {"sensor": "load_cell", "response_template": {}}}` e pode retornar: `{"id": 123,"result":{"header":["time", "force (g)", "counts", "tare_counts"]}}` e pode posteriormente produzir mensagens assíncronas como: `{"params":{"data":[[3292.432935, 40.65, 562534, -234467]]}}`

O campo "header" na resposta da consulta inicial é usado para descrever os campos encontrados nas respostas de "data" (dados) posteriores.

<a id="load_cell_probedump_taps"></a>
### load_cell_probe/dump_taps

Este endpoint é usado para assinar os detalhes dos eventos de “tap” da sondagem. O uso deste endpoint pode aumentar a carga do sistema do Klipper.

Uma requisição pode ter este aspecto: `{"id": 123, "method":"load_cell/dump_force", "params": {"sensor": "load_cell", "response_template": {}}}` e pode retornar: `{"id": 123,"result":{"header":["probe_tap_event"]}}` e pode posteriormente produzir mensagens assíncronas como:

```
{"params":{"tap":'{
   "time": [118032.28039, 118032.2834, ...],
   "force": [-459.4213119680034, -458.1640702543264, ...],
}}}
```

Estes dados podem ser usados para renderizar:

* O gráfico tempo/força

<a id="pause_resumecancel"></a>
### pause_resume/cancel

Este endpoint é semelhante à executar "CANCEL_PRINT". Por exemplo: `{"id": 123, "method": "pause_resume/cancel"}`

Tal como acontece com o endpoint "gcode/script", este endpoint só é concluído após a conclusão de qualquer comando G-Code pendente.

<a id="pause_resumepause"></a>
### pause_resume/pause

Este endpoint é semelhante à executar "PAUSE". Por exemplo: `{"id": 123, "method": "pause_resume/pause"}`

Tal como acontece com o endpoint "gcode/script", este endpoint só é concluído após a conclusão de qualquer comando G-Code pendente.

<a id="pause_resumeresume"></a>
### pause_resume/resume

Este endpoint é semelhante à executar"RESUME". Por exemplo: `{"id": 123, "method": "pause_resume/resume"}`

Tal como acontece com o endpoint "gcode/script", este endpoint só é concluído após a conclusão de qualquer comando G-Code pendente.

<a id="query_endstopsstatus"></a>
### query_endstops/status

Este endpoint consultará os endpoints ativos e retornará o status. Por exemplo: `{"id": 123, "method": "query_endstops/status"}` pode retornar: `{"id": 123, "result": {"y": "open", "x": "open", "z": "TRIGGERED"}}`

Tal como acontece com o endpoint "gcode/script", este endpoint só é concluído após a conclusão de qualquer comando G-Code pendente.

<a id="bed_meshdump_mesh"></a>
### bed_mesh/dump_mesh

Despeja a configuração e o estado da malha atual e de todos os perfis salvos.

Por exemplo: `{"id": 123, "method": "bed_mesh/dump_mesh"}`

pode retornar:

```
{
    "current_mesh": {
        "name": "eddy-scan-test",
        "probed_matrix": [...],
        "mesh_matrix": [...],
        "mesh_params": {
            "x_count": 9,
            "y_count": 9,
            "mesh_x_pps": 2,
            "mesh_y_pps": 2,
            "algo": "bicubic",
            "tension": 0.5,
            "min_x": 20,
            "max_x": 330,
            "min_y": 30,
            "max_y": 320
        }
    },
    "profiles": {
        "default": {
            "points": [...],
            "mesh_params": {
                "min_x": 20,
                "max_x": 330,
                "min_y": 30,
                "max_y": 320,
                "x_count": 9,
                "y_count": 9,
                "mesh_x_pps": 2,
                "mesh_y_pps": 2,
                "algo": "bicubic",
                "tension": 0.5
            }
        },
        "eddy-scan-test": {
            "points": [...],
            "mesh_params": {
                "x_count": 9,
                "y_count": 9,
                "mesh_x_pps": 2,
                "mesh_y_pps": 2,
                "algo": "bicubic",
                "tension": 0.5,
                "min_x": 20,
                "max_x": 330,
                "min_y": 30,
                "max_y": 320
            }
        },
        "eddy-rapid-test": {
            "points": [...],
            "mesh_params": {
                "x_count": 9,
                "y_count": 9,
                "mesh_x_pps": 2,
                "mesh_y_pps": 2,
                "algo": "bicubic",
                "tension": 0.5,
                "min_x": 20,
                "max_x": 330,
                "min_y": 30,
                "max_y": 320
            }
        }
    },
    "calibration": {
        "points": [...],
        "config": {
            "x_count": 9,
            "y_count": 9,
            "mesh_x_pps": 2,
            "mesh_y_pps": 2,
            "algo": "bicubic",
            "tension": 0.5,
            "mesh_min": [
                20,
                30
            ],
            "mesh_max": [
                330,
                320
            ],
            "origin": null,
            "radius": null
        },
        "probe_path": [...],
        "rapid_path": [...]
    },
    "probe_offsets": [
        0,
        25,
        0.5
    ],
    "axis_minimum": [
        0,
        0,
        -5,
        0
    ],
    "axis_maximum": [
        351,
        358,
        330,
        0
    ]
}
```

O endpoint `dump_mesh` aceita um parâmetro opcional, `mesh_args`. Este parâmetro deve ser um objeto, em que as chaves e os valores são parâmetros disponíveis para [BED_MESH_CALIBRATE](G-Codes.md#bed_mesh_calibrate). Isso atualizará a configuração da malha e os pontos de sondagem usando os parâmetros fornecidos antes de retornar o resultado. Recomenda-se omitir os parâmetros da malha, a menos que se deseje visualizar os pontos de sondagem e/ou o caminho de deslocamento antes de executar `BED_MESH_CALIBRATE`.
