# Microcontrolador na RPi

[English](../RPi_microcontroller.md) | **Português brasileiro**

Este documento descreve como executar Klipper em uma RPi e usar a mesma RPi
como MCU secundária.

<a id="why-use-rpi-as-a-secondary-mcu"></a>
## Por que usar RPi como MCU secundária?

MCUs dedicadas a impressoras 3D costumam ter um número limitado e pré-configurado
de pinos expostos para funções principais (termistores, extrusoras, motores...).
Usar a RPi onde Klipper está instalado como MCU secundária permite usar
diretamente seus GPIOs e barramentos (i2c, spi) dentro do Klipper, sem plugins
Octoprint (se utilizado) nem programas externos, controlando tudo pelo GCODE
da impressão.

**Aviso:** se sua plataforma é _Beaglebone_ e você seguiu corretamente a
instalação, a MCU Linux já está instalada e configurada no sistema.

<a id="install-the-rc-script"></a>
## Instalar o script rc

Para usar o host como MCU secundária, o processo klipper_mcu precisa iniciar
antes do klippy.

Após instalar Klipper, instale o script. Execute:
```
cd ~/klipper/
sudo cp ./scripts/klipper-mcu.service /etc/systemd/system/
sudo systemctl enable klipper-mcu.service
```

<a id="building-the-micro-controller-code"></a>
## Compilar o código do microcontrolador

Para compilar o código MCU do Klipper, comece configurando-o para
`Linux process`:
```
cd ~/klipper/
make menuconfig
```

No menu, defina `Microcontroller Architecture` como `Linux process`, salve
e saia.

Para compilar e instalar o novo código, execute:
```
sudo service klipper stop
make flash
sudo service klipper start
```

Se klippy.log informar `Permission denied` ao tentar conectar a
`/tmp/klipper_host_mcu`, adicione seu usuário ao grupo tty. O comando abaixo
adiciona `pi`:
```
sudo usermod -a -G tty pi
```

<a id="remaining-configuration"></a>
## Configuração restante

Conclua configurando a MCU secundária conforme o
[exemplo RaspberryPi](../../config/sample-raspberry-pi.cfg) e o
[exemplo de múltiplas MCUs](../../config/sample-multi-mcu.cfg).

<a id="optional-enabling-spi"></a>
## Opcional: habilitar SPI

Garanta que o driver SPI Linux está habilitado: execute `sudo raspi-config`
e habilite SPI em `Interfacing options`.

<a id="optional-enabling-i2c"></a>
## Opcional: habilitar I2C

Garanta que o driver I2C Linux está habilitado: execute `sudo raspi-config`
e habilite I2C em `Interfacing options`. Para usar I2C no acelerômetro MPU,
também é necessário definir baud em 400000, adicionando/descomentando
`dtparam=i2c_arm=on,i2c_arm_baudrate=400000` em `/boot/config.txt`
(ou `/boot/firmware/config.txt` em algumas distribuições).

<a id="optional-identify-the-correct-gpiochip"></a>
## Opcional: identificar o gpiochip correto

Na Raspberry Pi e em muitos clones, os pinos expostos pertencem ao primeiro
gpiochip. Podem ser usados no Klipper pelos nomes `gpio0..n`. Porém, às vezes
pertencem a outros gpiochips, por exemplo em algumas OrangePi ou com expansor
de portas. Nesses casos, use comandos de acesso ao _dispositivo de caracteres
GPIO Linux_ para conferir a configuração.

Para instalar os binários em uma distribuição Debian, como octopi, execute:
```
sudo apt-get install gpiod
```

Para listar gpiochips disponíveis:
```
gpiodetect
```

Para conferir números e disponibilidade dos pinos:
```
gpioinfo
```

Use o pino escolhido como `gpiochip<n>/gpio<o>`: **n** é o número do chip no
`gpiodetect`; **o** é o número da linha no `gpioinfo`.

***Aviso:*** só podem ser usados GPIOs marcados como `unused`. Uma _linha_
não pode ser usada simultaneamente por vários processos.

Por exemplo, em uma RPi 3B+ com Klipper usando GPIO20 para um switch:
```
$ gpiodetect
gpiochip0 [pinctrl-bcm2835] (54 lines)
gpiochip1 [raspberrypi-exp-gpio] (8 lines)

$ gpioinfo
gpiochip0 - 54 lines:
        line   0:      unnamed       unused   input  active-high
        line   1:      unnamed       unused   input  active-high
        line   2:      unnamed       unused   input  active-high
        line   3:      unnamed       unused   input  active-high
        line   4:      unnamed       unused   input  active-high
        line   5:      unnamed       unused   input  active-high
        line   6:      unnamed       unused   input  active-high
        line   7:      unnamed       unused   input  active-high
        line   8:      unnamed       unused   input  active-high
        line   9:      unnamed       unused   input  active-high
        line  10:      unnamed       unused   input  active-high
        line  11:      unnamed       unused   input  active-high
        line  12:      unnamed       unused   input  active-high
        line  13:      unnamed       unused   input  active-high
        line  14:      unnamed       unused   input  active-high
        line  15:      unnamed       unused   input  active-high
        line  16:      unnamed       unused   input  active-high
        line  17:      unnamed       unused   input  active-high
        line  18:      unnamed       unused   input  active-high
        line  19:      unnamed       unused   input  active-high
        line  20:      unnamed    "klipper"  output  active-high [used]
        line  21:      unnamed       unused   input  active-high
        line  22:      unnamed       unused   input  active-high
        line  23:      unnamed       unused   input  active-high
        line  24:      unnamed       unused   input  active-high
        line  25:      unnamed       unused   input  active-high
        line  26:      unnamed       unused   input  active-high
        line  27:      unnamed       unused   input  active-high
        line  28:      unnamed       unused   input  active-high
        line  29:      unnamed       "led0"  output  active-high [used]
        line  30:      unnamed       unused   input  active-high
        line  31:      unnamed       unused   input  active-high
        line  32:      unnamed       unused   input  active-high
        line  33:      unnamed       unused   input  active-high
        line  34:      unnamed       unused   input  active-high
        line  35:      unnamed       unused   input  active-high
        line  36:      unnamed       unused   input  active-high
        line  37:      unnamed       unused   input  active-high
        line  38:      unnamed       unused   input  active-high
        line  39:      unnamed       unused   input  active-high
        line  40:      unnamed       unused   input  active-high
        line  41:      unnamed       unused   input  active-high
        line  42:      unnamed       unused   input  active-high
        line  43:      unnamed       unused   input  active-high
        line  44:      unnamed       unused   input  active-high
        line  45:      unnamed       unused   input  active-high
        line  46:      unnamed       unused   input  active-high
        line  47:      unnamed       unused   input  active-high
        line  48:      unnamed       unused   input  active-high
        line  49:      unnamed       unused   input  active-high
        line  50:      unnamed       unused   input  active-high
        line  51:      unnamed       unused   input  active-high
        line  52:      unnamed       unused   input  active-high
        line  53:      unnamed       unused   input  active-high
gpiochip1 - 8 lines:
        line   0:      unnamed       unused   input  active-high
        line   1:      unnamed       unused   input  active-high
        line   2:      unnamed       "led1"  output   active-low [used]
        line   3:      unnamed       unused   input  active-high
        line   4:      unnamed       unused   input  active-high
        line   5:      unnamed       unused   input  active-high
        line   6:      unnamed       unused   input  active-high
        line   7:      unnamed       unused   input  active-high
```

<a id="optional-hardware-pwm"></a>
## Opcional: PWM por hardware

Raspberry Pi tem dois canais PWM (PWM0/PWM1), expostos no conector ou que podem
ser direcionados a GPIOs existentes. O daemon MCU Linux usa a interface sysfs
pwmchip para controlar PWM por hardware no host. Ela não fica exposta por
padrão na Raspberry e pode ser habilitada adicionando a `/boot/config.txt`:
```
# Enable pwmchip sysfs interface
dtoverlay=pwm,pin=12,func=4
```
Isso habilita somente PWM0 e o direciona a gpio12. Para os dois canais, use
`pwm-2chan`:
```
# Enable pwmchip sysfs interface
dtoverlay=pwm-2chan,pin=12,func=4,pin2=13,func2=4
```
Esse exemplo também habilita PWM1 em gpio13.

O overlay não expõe a linha no sysfs ao iniciar. Exporte escrevendo o número
do canal em `/sys/class/pwm/pwmchip0/export`, criando
`/sys/class/pwm/pwmchip0/pwm0`. O modo mais simples é adicionar a
`/etc/rc.local` antes de `exit 0`:
```
# Enable pwmchip sysfs interface
echo 0 > /sys/class/pwm/pwmchip0/export
```
Com dois canais, também escreva o número do segundo:
```
# Enable pwmchip sysfs interface
echo 0 > /sys/class/pwm/pwmchip0/export
echo 1 > /sys/class/pwm/pwmchip0/export
```

Com sysfs pronto, use os canais adicionando ao `printer.cfg`:
```
[output_pin caselight]
pin: host:pwmchip0/pwm0
pwm: True
hardware_pwm: True
cycle_time: 0.000001

[output_pin beeper]
pin: host:pwmchip0/pwm1
pwm: True
hardware_pwm: True
value: 0
shutdown_value: 0
cycle_time: 0.0005
```
Isso adiciona controle PWM por hardware a gpio12/gpio13 na Pi, pois o overlay
direciona pwm0 a pin=12 e pwm1 a pin=13.

PWM0 pode ser direcionado a gpio12/gpio18, PWM1 a gpio13/gpio19:

| PWM | Pino GPIO | Função |
| --- | -------- | ---- |
|   0 |       12 |    4 |
|   0 |       18 |    2 |
|   1 |       13 |    4 |
|   1 |       19 |    2 |
