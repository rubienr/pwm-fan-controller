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
        elapsedMillis oneSecondProcessTriggerCounterMs{ 0 };
    #if defined(SERIAL_AUTOREPORT)
        elapsedSeconds reportOnSerialCounterS{ 0 };
    #endif
    } timers;
};

struct Firmware : public Resources
{

    void printAddress(const DeviceAddress &deviceAddress)
    {
        for(uint8_t i = 0; i < 8; i++)
        {
            if(deviceAddress[i] < 16) Serial.print("0");
            Serial.print(deviceAddress[i], HEX);
        }
    }


    void printFan(uint8_t fanIndex)
    {
        const FanInfo &info{ controller.fansInfo[fanIndex] };
        if(info.hasTempError || info.hasTachoError || info.hasPwmAlert || info.hasRpmAlert)
            display.screen.setTextColor(BLACK, WHITE);
        display.screen.print(fanIndex);
        display.screen.print(" ");
        int16_t rpm{ info.rpm };
        if(rpm < 10) display.screen.print(" ");
        if(rpm < 100) display.screen.print(" ");
        if(rpm < 1000) display.screen.print(" ");
        display.screen.print(rpm);
        float tempCelsius{ info.tempCelsius };
        if(tempCelsius < 10 && tempCelsius >= 0) display.screen.print(" ");
        if(tempCelsius < 100 && tempCelsius >= 0) display.screen.print(" ");
        display.screen.print(tempCelsius, 1);
        display.screen.print(" ");
        auto power{ info.powerPercent };
        if(power < 10) display.screen.print(" ");
        if(power < 100) display.screen.print(" ");
        display.screen.print(power, 1);
        display.screen.print(" ");
        uint16_t pwm{ info.powerPwm };
        if(pwm < 10) display.screen.print(" ");
        if(pwm < 100) display.screen.print(" ");
        display.screen.print(pwm);
        display.screen.println();
        display.screen.setTextColor(WHITE, BLACK);
    }

    char getTrendSymbol(const FanInfo &info)
    {
        if(info.hasTempError) return 'T';
        if(info.hasTachoError) return 'S';
        if(info.hasRpmAlert) return 'r';
        if(info.hasPwmAlert) return 'p';
        if(info.hasTempAlert) return 't';
        if(info.trend > 0) return '^';
        if(info.trend < 0) return 'v';
        return '-';
    }


    void reportFanInfo(uint8_t fanIndex, const FanInfo &info, bool reportOnError = true)
    {
        if(!reportOnError || info.hasTempError || info.hasTachoError || info.hasRpmAlert || info.hasPwmAlert)
        {
            Serial.print(millis());
            Serial.print(" fan=");
            Serial.print(fanIndex);
            Serial.print(" rpm=");
            Serial.print(info.rpm);
            Serial.print(" power=");
            Serial.print(info.power);
            Serial.print(" powerPercent=");
            Serial.print(info.powerPercent, 1);
            Serial.print(" powerPwm=");
            Serial.print(info.powerPwm);
            Serial.print(" tempCelsius=");
            Serial.print(info.tempCelsius, 1);
            Serial.print(" tachoError=");
            Serial.print(info.hasTachoError);
            Serial.print(" tempError=");
            Serial.print(info.hasTempError);
            Serial.print(" pwmAlert=");
            Serial.print(info.hasPwmAlert);
            Serial.print(" rpmAlert=");
            Serial.print(info.hasRpmAlert);
            Serial.print(" tempAlert=");
            Serial.print(info.hasTempAlert);
            Serial.print(" trend=");
            Serial.println(info.trend);
        }
    }


    void printFansInfo()
    {
        display.screen.println("#  r/m degC  Pow% PWM");

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

        display.screen.print("Trend [");
    #if defined(FAN0)
        {
            display.screen.print(" ");
            const FanInfo &info{ controller.fansInfo[FAN0_INDEX] };
            display.screen.print(getTrendSymbol(info));
            reportFanInfo(FAN0_INDEX, info);
        }
    #endif
    #if defined(FAN1)
        {
            display.screen.print(" ");
            const FanInfo &info{ controller.fansInfo[FAN1_INDEX] };
            display.screen.print(getTrendSymbol(info));
            reportFanInfo(FAN1_INDEX, info);
        }
    #endif
    #if defined(FAN2)
        {
            display.screen.print(" ");
            const FanInfo &info{ controller.fansInfo[FAN2_INDEX] };
            display.screen.print(getTrendSymbol(info));
            reportFanInfo(FAN2_INDEX, info);
        }
    #endif
    #if defined(FAN3)
        {
            display.screen.print(" ");
            const FanInfo &info{ controller.fansInfo[FAN3_INDEX] };
            display.screen.print(getTrendSymbol(info));
            reportFanInfo(FAN3_INDEX, info);
        }
    #endif
    #if defined(FAN4)
        {
            display.screen.print(" ");
            const FanInfo &info{ controller.fansInfo[FAN4_INDEX] };
            display.screen.print(getTrendSymbol(info));
            reportFanInfo(FAN4_INDEX, info);
        }
    #endif
        display.screen.println(" ]");
    }

    void reportTemperatureSensor(uint8_t sensorIndex, const DeviceAddress &address)
    {
        Serial.print(millis());
        Serial.print(F(" sensorIndex="));
        Serial.print(sensorIndex);
        Serial.print(F(" address="));
        printAddress(address);
        if(temp.dsSensors.isConnected(address)) Serial.print(F(" connected=true"));
        else Serial.print(F(" connected=false"));
        Serial.print(F(" tempCelsius="));
        Serial.print(temp.dsSensors.getTempC(address), 1);
        Serial.print(F(" resolutionBits="));
        Serial.print(temp.dsSensors.getResolution(address));
        Serial.println();
    }

    void reportFansInfo()
    {
    #if defined(FAN0)
        reportFanInfo(FAN0_INDEX, controller.fansInfo[FAN0_INDEX], false);
    #endif
    #if defined(FAN1)
        reportFanInfo(FAN1_INDEX, controller.fansInfo[FAN1_INDEX], false);
    #endif
    #if defined(FAN2)
        reportFanInfo(FAN2_INDEX, controller.fansInfo[FAN2_INDEX], false);
    #endif
    #if defined(FAN3)
        reportFanInfo(FAN3_INDEX, controller.fansInfo[FAN3_INDEX], false);
    #endif
    #if defined(FAN4)
        reportFanInfo(FAN4_INDEX, controller.fansInfo[FAN4_INDEX], false);
    #endif
    }
    void processEvery1000Ms() { controller.process(); }
} f;


void setup()
{
    Serial.begin(SERIAL_BAUD);
    Wire.begin(SCREEN_WIRE_SDA, SCREEN_WIRE_SCL);
    if(!f.display.screen.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false))
    {
        Serial.print(millis());
        Serial.println(F(" SSD1306 allocation failed"));
        for(;;)
            ;
    }
    f.display.screen.setTextSize(1, 1);
    f.display.screen.setTextColor(WHITE, BLACK);
    f.display.screen.setRotation(2);
    f.display.screen.clearDisplay();
    f.display.screen.setCursor(0, 0);
    f.display.screen.display();

    Serial.print(millis());
    if(f.fan.pwms.begin()) Serial.println(F(" pwms.begin(): OK"));
    else Serial.println(F(" pwms.begin(): ERROR"));

    Serial.print(millis());
    if(f.fan.tachos.begin()) Serial.println(F(" tachos.begin(): OK"));
    else Serial.println(F(" tachos.begin(): ERROR"));

    f.temp.dsSensors.begin();
    f.temp.sensors.begin();

    Serial.print(millis());
    Serial.print(F(" reported sensor bus power is "));
    if(f.temp.dsSensors.isParasitePowerMode()) Serial.println("parasitic");
    else Serial.println(F("VCC"));
    Serial.print(millis());
    Serial.print(F(" discovered "));
    Serial.print(f.temp.dsSensors.getDeviceCount());
    Serial.println(F(" temperature sensors (index depends on bus order):"));
    for(uint8_t idx = f.temp.dsSensors.getDeviceCount(); idx > 0; idx--)
    {
        DeviceAddress address;
        f.temp.dsSensors.getAddress(address, idx - 1);
        f.reportTemperatureSensor(idx - 1, address);
    }

    Serial.print(millis());
    Serial.println(F(" check configured addresses (index depends on configuration order):"));
    DeviceAddress configuredAddresses[] TEMP_SENSORS_ADDRESS;
    for(uint8_t idx = sizeof(configuredAddresses) / sizeof(DeviceAddress); idx > 0; idx--)
        f.reportTemperatureSensor(idx - 1, configuredAddresses[idx - 1]);
}

void loop()
{
    static const char s[]{ '_', '-' };
    static uint8_t i{ 0 };
    if(f.timers.oneSecondProcessTriggerCounterMs >= 1000)
    {
        unsigned long currentSeparationMs = f.timers.oneSecondProcessTriggerCounterMs;
        f.timers.oneSecondProcessTriggerCounterMs = 0;

        f.processEvery1000Ms();

        f.display.screen.clearDisplay();
        f.display.screen.setCursor(0, 0);
        f.printFansInfo();

    #if defined(SERIAL_AUTOREPORT)
        if(f.timers.reportOnSerialCounterS >= SERIAL_AUTOREPORT_EVERY_SECONDS)
        {
            f.timers.reportOnSerialCounterS = 0;
            f.reportFansInfo();
        }
    #endif

        f.display.screen.print("sep=");
        f.display.screen.print(currentSeparationMs);
        f.display.screen.print(" exc=");
        f.display.screen.print(f.timers.oneSecondProcessTriggerCounterMs);

        i = (i + 1) % sizeof(s);
        f.display.screen.setCursor(SCREEN_WIDTH_PX - 6, SCREEN_HEIGHT_PX - 7);
        f.display.screen.print(s[i]);

        f.display.screen.display();
    }
}

#endif
