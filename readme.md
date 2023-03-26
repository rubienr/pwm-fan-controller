# PWM Fan Controller

A simple fan controller that controls up to 5 PWM-Fans (i.e. Nocuta 4-Pin PWM fan,
see [whitepaper](./docs/noctua_pwm_specifications_white_paper.pdf)) by monitoring
the temperature from up to 5 thermal sensors (i.e. DS18B20). The firmware monitors
the temperature and generates a PWM signal for the fan.
The PWM signal is computed according to a pre-configured power curve.
The response from the fan (tacho signal) is monitored as well.

Due to the required tacho-inputs and PWM-outputs for the moment only Wroom32 is
supported (i.e. [lolin32 board with oled display](https://randomnerdtutorials.com/esp32-built-in-oled-ssd1306/)).

Required hardware:

- one PWM channel at 25kHz each fan: `ledcSetup()`, `ledcAttachPin()` and `ledcWrite()` (
  see [LEDC reference](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/ledc.html))
- one PCNT (Pulse Count Controller) each fan (
  see [PCNT reference](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/pcnt.html))
- one GPIO for OneWire (software OneWire for Dallas temperature sensor bus)

References

- [ESP32 manual](./docs/esp32_technical_reference_manual_en.pdf)

# Features

- fault detection
  - temperature sensor readout error
  - tacho signal PCNT setup error
- fault action
  - fall back to specific error-PWM
- alert if parameter exceeds configured boundary
  - temperature
  - PWM
  - RPM
- reporting
  - briefly on display
  - detailed by serial
- serial console, see 'h' for help
- load modify and save configuration from/to flash at runtime (serial console)
  - power curve 
  - alert thresholds
  - sensor address
  - fan reference sensor
  - logging

## Roadmap (not implemented yet)
- OTA firmware update

# Examples

## Console commands 'h'

```bash
$ pio device monitor
h
Help:
h  print this help text:                h
f  print fan(s):                        f [fanIndex]?
L  set   auto report seconds:           L [reportSeconds]
l  print auto report seconds:           l
C  set   power curve:                   C [fanIndex] [temp0] [power0] [temp1] [power1] [temp2] [power2] [temp3] [power3]
c  print power curve(s):                c [fanIndex]?
P  set   PWM alert:                     P [fanIndex] [pwmMin] [pamMax]
p  print PWM alert(s):                  p [fanIndex]?
R  set   RPM alert:                     R [fanIndex] [rpmMin] [rpmMax]
r  print RPM alert(s):                  r [fanIndex]?
T  set   temperature alert:             T [fanIndex] [tempMin] [tempMax]
t  print temperature alert(s):          t [fanIndex]?
I  set   temperature sensor index:      I [fanIndex] [tempSensorIndex]
i  print temperature sensor(s) index:   i [fanIndex]?
A  set   temperature sensor address:    A [tempSensorIndex] [aa] [aa] [aa] [aa] [aa] [aa] [aa] [aa]
a  print temperature sensor(s) address: a [tempSensorIndex]?
S  save  settings:                      S
s  load  settings:                      s
d  print settings:                      d
x  reset settings:                      x
X  reboot device:                       X
Units:
fanIndex:        uint8
tempSensorIndex: uint8
reportSeconds:   uint16, 0=disable
power[N]:        uint8
pwm[Min|Max]:    uint8
rpm[Min|Max]:    uint16
temp[Min|Max]:   int16, 1/10 Celsius (0.1C), i.e. -273.1C=-2731, 150.1C=1501
temp[N]:         int16, 1/10 Celsius (0.1C), i.e. -273.1C=-2731, 150.1C=1501, temp[N] < temp[N+1]
[aa]:            1-byte HEX address, i.e. AA, Fe, be
```

## Modify Fan Sensor
```bash
# print fan to sensor dependencies
i 
I 0 0
I 1 1
I 2 2
I 3 3
I 4 4
# print temperature sensors' address
a 
A 0 28 FF 64 02 19 86 14 FC
A 1 28 D9 EC 09 06 00 00 F5
A 2 28 FF 64 02 19 8B D0 DE
A 3 28 64 41 0A 06 00 00 EF
A 4 28 05 52 08 06 00 00 2E
# let fan 0 depend on temperature sensor 1
I 0 1
# modify address of temperature sensor 1
A 1 28 D9 EC 09 06 00 00 FF
# save settings to flash
S
# reboot (optional)
X
```

## Boot Log

```bash
PWM controller version 0.0.2 built Fri Mar 24 00:44:03 2023
fans=5 sensors=5
Notes:
  - lines with leading '#' are human readable logs
  - lines without leading '#' are slightly machine parseable
LittleFS mounted: used=8192, total=1441792 [Bytes]
container loaded
Container:
  crc=3890207414
  Data:
    configWrites=5
    Version:
      versionNumber
        major=0
        minor=0
        patch=2
      buildTimestamp="Fri Mar 24 00:44:03 2023"
     Settings:
       serialAutoreportSeconds=0
       temperatureSensors:
         address[0]="28 FF 64 02 19 86 14 FC"
         address[1]="28 D9 EC 09 06 00 00 F5"
         address[2]="28 FF 64 02 19 8B D0 DE"
         address[3]="28 64 41 0A 06 00 00 EF"
         address[4]="28 05 52 08 06 00 00 2E"
       fans:
         fans[0]:
            defaultPwmDuty=100
            errorPwmDuty=255
            temperatureSensorIndex=0
            fanCurvePower={50, 50, 100, 100}
            fanCurveDeciCelsius={0, 250, 700, 800}
            alertBelowPwm=50
            alertAbovePwm=50
            alertBelowRpm=0
            alertAboveRpm=1500
            alertBelowTempDeciC=0
            alertAboveTempDeciC=600
         fans[1]:
            defaultPwmDuty=100
            errorPwmDuty=255
            temperatureSensorIndex=1
            fanCurvePower={50, 50, 100, 100}
            fanCurveDeciCelsius={0, 250, 700, 800}
            alertBelowPwm=1
            alertAbovePwm=254
            alertBelowRpm=0
            alertAboveRpm=1500
            alertBelowTempDeciC=0
            alertAboveTempDeciC=600
         fans[2]:
            defaultPwmDuty=100
            errorPwmDuty=255
            temperatureSensorIndex=2
            fanCurvePower={50, 50, 100, 100}
            fanCurveDeciCelsius={0, 250, 700, 800}
            alertBelowPwm=1
            alertAbovePwm=254
            alertBelowRpm=0
            alertAboveRpm=1500
            alertBelowTempDeciC=0
            alertAboveTempDeciC=600
         fans[3]:
            defaultPwmDuty=100
            errorPwmDuty=255
            temperatureSensorIndex=3
            fanCurvePower={50, 50, 100, 100}
            fanCurveDeciCelsius={0, 250, 700, 800}
            alertBelowPwm=1
            alertAbovePwm=254
            alertBelowRpm=0
            alertAboveRpm=1500
            alertBelowTempDeciC=0
            alertAboveTempDeciC=600
         fans[4]:
            defaultPwmDuty=100
            errorPwmDuty=255
            temperatureSensorIndex=4
            fanCurvePower={50, 50, 100, 100}
            fanCurveDeciCelsius={0, 250, 700, 800}
            alertBelowPwm=1
            alertAbovePwm=254
            alertBelowRpm=0
            alertAboveRpm=1500
            alertBelowTempDeciC=0
            alertAboveTempDeciC=600
 init temperature sensors ... 
 reported sensor bus power is VCC
 discovered 5 temperature sensors (index depends on bus order):
 sensorIndex=0 address=2864410A060000EF connected=true tempCelsius=24.0 resolutionBits=10
 sensorIndex=1 address=28D9EC09060000F5 connected=true tempCelsius=24.0 resolutionBits=10
 sensorIndex=2 address=280552080600002E connected=true tempCelsius=24.0 resolutionBits=10
 sensorIndex=3 address=28FF6402198614FC connected=true tempCelsius=23.9 resolutionBits=12
 sensorIndex=4 address=28FF6402198BD0DE connected=true tempCelsius=24.1 resolutionBits=12
 check configured addresses (index depends on configuration order):
 sensorIndex=0 address=28FF6402198614FC connected=true tempCelsius=23.9 resolutionBits=12
 sensorIndex=1 address=28D9EC09060000F5 connected=true tempCelsius=24.0 resolutionBits=10
 sensorIndex=2 address=28FF6402198BD0DE connected=true tempCelsius=24.1 resolutionBits=12
 sensorIndex=3 address=2864410A060000EF connected=true tempCelsius=24.0 resolutionBits=10
 sensorIndex=4 address=280552080600002E connected=true tempCelsius=24.0 resolutionBits=10
 severe input error flags: 'R'=tachoError 'T'=tempError 'S'=tempSensorError
 alert flags if parameter(s) not in range: 'p'=pwmAlert 'r'=rpmAlert 't'=tempAlert
 Enter command 'h' for help.
```
