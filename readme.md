# Work in progress ...

# PWM Fan Controller

A simple fan controller that controls up to 5 PWM-Fans (i.e. Nocuta 4-Pin PWM fan,
see [whitepaper](./docs/noctua_pwm_specifications_white_paper.pdf)) by monitoring
the temperature from up to 5 thermal sensors (i.e. DS18B20). The firmware watches
the temperature and generates the corresponding PWM signal according to the
pre-configured fan curve, additionally the tacho input-signal is interpreted and
presented on the display as RPM.

Due to the required tacho-inputs and PWM-outputs for the moment only Wroom32 is
supported (i.e. [lolin32 board with oled display](https://randomnerdtutorials.com/esp32-built-in-oled-ssd1306/)).

Required hardware:

- one PWM channel each fan: `ledcSetup()`, `ledcAttachPin()` and `ledcWrite()` (
  see [LEDC reference](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/ledc.html))
- one PCNT (pulse count controller) each fan (
  see [PCNT reference](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/pcnt.html))
- one GPIO for OneWire (software OneWire for Dallas temperature sensor bus)
- see also [ESP32 manual](./docs/esp32_technical_reference_manual_en.pdf)

# Not implemented yet
- configuration at runtime
- store/load configuration from flash
- config via Wifi
