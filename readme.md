# Work in progress ...

# PWM Fan Controller

A simple fan controller that controls up to 5 PWM-Fans (i.e. Nocuta 4-Pin PWM fan,
see [whitepaper](./docs/noctua_pwm_specifications_white_paper.pdf)) by monitoring
the temperature from up to 5 thermal sensors (i.e. DS18B20). The firmware monitors
the temperature and generates the corresponding PWM signal for the fan.
The PWM signal is computed according to a pre-configured power curve.
The response from the fan (tacho signal) is interpreted and presented on the display.

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

- fault detection: 
  - tacho signal PCNT setup error
  - temperature sensor readout error
- fault action
  - fall back to specific error-PWM
- alert if parameter exceeds configured min/max boundary:
  - RPM
  - temperature
  - PWM
- reporting
  - briefly on display
  - detailed by serial
- configuration at runtime: 
  - serial console, see 'h' for help
 
## Roadmap (not implemented yet)
- store/load configuration to/from flash
- modify sensor address at runtime
- OTA firmware update
