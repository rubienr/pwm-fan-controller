#if defined(ENV_NATIVE)

int main(int argc, char **argv) { return 0; }

#else
    #include "configuration.h"
    #include "controller/FansController.h"
    #include "fan/FansPwm.h"
    #include "fan/FansTacho.h"
    #include "temp/SensorsTemp.h"
    #include <Adafruit_GFX.h>
    #include <Adafruit_SSD1306.h>
    #include <Arduino.h>
    #include <DallasTemperature.h>
    #include <OneWire.h>
    #include <Wire.h>
    #include <elapsedMillis.h>

struct Resources
{
    struct
    {
        Adafruit_SSD1306 screen{ SCREEN_WIDTH_PX, SCREEN_HEIGHT_PX, &Wire, -1 };
    } display;

    struct
    {
        FansTacho tachos;
        FansPwm pwms;
    } fan;

    struct
    {
        OneWire wire{ TEMP_ONE_WIRE_GPIO_NUM };
        DallasTemperature dsSensors{ &wire };
        SensorsTemp sensors{ dsSensors };
    } temp;

    FansController controller{ temp.sensors, fan.pwms, fan.tachos };

    struct
    {
        elapsedMillis oneSecondMs{ 0 };
    } timers;
} r;


void printAddress(DeviceAddress deviceAddress)
{
    for(uint8_t i = 0; i < 8; i++)
    {
        if(deviceAddress[i] < 16) Serial.print("0");
        Serial.print(deviceAddress[i], HEX);
    }
}

void setup(void)
{
    Serial.begin(SERIAL_BAUD);
    Wire.begin(SCREEN_WIRE_SDA, SCREEN_WIRE_SCL);
    if(!r.display.screen.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;)
            ;
    }
    r.display.screen.setTextColor(WHITE);
    r.display.screen.setRotation(2);
    r.display.screen.clearDisplay();
    r.display.screen.setCursor(0, 0);
    r.display.screen.display();

    if(r.fan.pwms.begin()) { Serial.println("pwms.begin() -> OK"); }
    else
    {
        Serial.println("pwms.begin() -> NOT OK");
        for(;;)
            ;
    }


    if(r.fan.tachos.begin()) { Serial.println("tachos.begin() -> OK"); }
    else
    {
        Serial.println("tachos.begin() -> NOT OK");
        for(;;)
            ;
    }

    r.temp.dsSensors.begin();
    r.temp.sensors.begin();

    Serial.print("found devices: ");
    Serial.println(r.temp.dsSensors.getDeviceCount());
    Serial.print("power parasite: ");
    if(r.temp.dsSensors.isParasitePowerMode()) Serial.println("ON");
    else Serial.println("OFF");

    for(uint8_t idx = r.temp.dsSensors.getDeviceCount(); idx > 0; idx--)
    {
        Serial.print("sensor idx=");
        Serial.print(idx - 1);
        DeviceAddress address;
        bool addressValid{ r.temp.dsSensors.getAddress(address, idx - 1) };
        Serial.print(" address=");
        if(addressValid) printAddress(address);
        else Serial.print("???");
        if(r.temp.dsSensors.isConnected(address)) { Serial.print(" connected=true"); }
        else { Serial.print(" connected=false"); }
        Serial.print(" t=");
        Serial.print(r.temp.dsSensors.getTempC(address), 1);
        Serial.print(" res=");
        Serial.print(r.temp.dsSensors.getResolution(address));
        Serial.println();
    }
}

void printFan(uint8_t fanIndex) {
    r.display.screen.print(fanIndex);
    r.display.screen.print(" ");
    int16_t rpm{ r.controller.fansInfo[fanIndex].rpm };
    if (rpm < 10)
        r.display.screen.print(" ");
    if (rpm < 100)
        r.display.screen.print(" ");
    if (rpm < 1000)
        r.display.screen.print(" ");
    r.display.screen.print(rpm);
    r.display.screen.print(" ");
    r.display.screen.print(r.controller.fansInfo[fanIndex].tempCelsius, 1);
    r.display.screen.print(" ");
    uint8_t power{r.controller.fansInfo[fanIndex].power};
    if (power < 10)
        r.display.screen.print(" ");
    if (power < 100)
        r.display.screen.print(" ");
    r.display.screen.print(power);
    r.display.screen.print(" ");
    uint16_t pwm{r.controller.fansInfo[fanIndex].powerPwm};
    if (pwm < 10)
        r.display.screen.print(" ");
    if (pwm < 100)
        r.display.screen.print(" ");
    if (pwm < 1000)
        r.display.screen.print(" ");
    r.display.screen.print(pwm);
    r.display.screen.println();
}

void loop(void)
{
    if(r.timers.oneSecondMs > (1000 - REDUCE_1000MS_CYCLE_MS))
    {
        auto sep{ r.timers.oneSecondMs };
        r.timers.oneSecondMs = 0;
        r.controller.process();

        r.display.screen.clearDisplay();
        r.display.screen.setCursor(0, 0);
        r.display.screen.println("#  RPM degC Pow  PWM");
    #if defined(FAN0)
        printFan(FAN0_INDEX);
    #endif
    #if defined(FAN1)
        printFan(FAN1_INDEX);
    #endif
    #if defined(FAN2)
        printFan(FAN2_INDEX);
    #endif
    #if defined(FAN3)
        printFan(FAN3_INDEX);
    #endif
    #if defined(FAN4)
        printFan(FAN4_INDEX);
    #endif
        r.display.screen.print("sep ");
        r.display.screen.println(sep);
        r.display.screen.display();
    }
}

#endif
