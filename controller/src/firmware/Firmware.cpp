#include "Firmware.h"


char Firmware::getTrendSymbol(const FanInfo &info)
{
    if(info.tempSpecs->hasError) return 'T';
    if(info.rpmSpecs->hasError) return 'S';
    if(info.rpmSpecs->hasAlert()) return 'r';
    if(info.pwmSpecs->hasAlert()) return 'p';
    if(info.tempSpecs->hasAlert()) return 't';
    if(info.trend > 0) return '^';
    if(info.trend < 0) return 'v';
    return '-';
}


void Firmware::displayFan(uint8_t fanIndex)
{
    const FanInfo &info{ controller.getFanInfo(fanIndex) };
    if(info.tempSpecs->hasError || info.rpmSpecs->hasError || info.pwmSpecs->hasAlert() || info.rpmSpecs->hasAlert())
        display.screen.setTextColor(BLACK, WHITE);
    display.screen.print(fanIndex);
    display.screen.print(" ");
    int16_t rpm{ info.rpmSpecs->currentRpm };
    if(rpm < 10) display.screen.print(" ");
    if(rpm < 100) display.screen.print(" ");
    if(rpm < 1000) display.screen.print(" ");
    display.screen.print(rpm);
    float tempCelsius{ info.tempSpecs->currentTempC };
    if(tempCelsius < 10 && tempCelsius >= 0) display.screen.print(" ");
    if(tempCelsius < 100 && tempCelsius >= 0) display.screen.print(" ");
    display.screen.print(tempCelsius, 1);
    display.screen.print(" ");
    auto power{ info.interpolatedPowerPercent };
    if(power < 10) display.screen.print(" ");
    if(power < 100) display.screen.print(" ");
    display.screen.print(power, 1);
    display.screen.print(" ");
    uint32_t pwm{ info.pwmSpecs->currentDuty };
    if(pwm < 10) display.screen.print(" ");
    if(pwm < 100) display.screen.print(" ");
    display.screen.print(pwm);
    display.screen.println();
    display.screen.setTextColor(WHITE, BLACK);
}


void Firmware::displayFansInfo()
{
    display.screen.println("#  r/m degC  Pow% PWM");

#if defined(FAN0)
    displayFan(FAN0_INDEX);
#endif
#if defined(FAN1)
    displayFan(FAN1_INDEX);
#endif
#if defined(FAN2)
    displayFan(FAN2_INDEX);
#endif
#if defined(FAN3)
    displayFan(FAN3_INDEX);
#endif
#if defined(FAN4)
    displayFan(FAN4_INDEX);
#endif

    display.screen.print("Trend [");
#if defined(FAN0)
    {
        display.screen.print(" ");
        const FanInfo &info{ controller.getFanInfo(FAN0_INDEX) };
        display.screen.print(getTrendSymbol(info));
        reportFanInfo(FAN0_INDEX, info);
    }
#endif
#if defined(FAN1)
    {
        display.screen.print(" ");
        const FanInfo &info{ controller.getFanInfo(FAN1_INDEX) };
        display.screen.print(getTrendSymbol(info));
        reportFanInfo(FAN1_INDEX, info);
    }
#endif
#if defined(FAN2)
    {
        display.screen.print(" ");
        const FanInfo &info{ controller.getFanInfo(FAN2_INDEX) };
        display.screen.print(getTrendSymbol(info));
        reportFanInfo(FAN2_INDEX, info);
    }
#endif
#if defined(FAN3)
    {
        display.screen.print(" ");
        const FanInfo &info{ controller.getFanInfo(FAN3_INDEX) };
        display.screen.print(getTrendSymbol(info));
        reportFanInfo(FAN3_INDEX, info);
    }
#endif
#if defined(FAN4)
    {
        display.screen.print(" ");
        const FanInfo &info{ controller.getFanInfo(FAN4_INDEX) };
        display.screen.print(getTrendSymbol(info));
        reportFanInfo(FAN4_INDEX, info);
    }
#endif
    display.screen.println(" ]");
}


void Firmware::reportAddress(const DeviceAddress &deviceAddress)
{
    for(unsigned char c : deviceAddress)
    {
        if(c < 16) Serial.print("0");
        Serial.print(c, HEX);
    }
}


void Firmware::reportTemperatureSensor(uint8_t sensorIndex, const DeviceAddress &address)
{
    Serial.print(millis());
    Serial.print(F(" sensorIndex="));
    Serial.print(sensorIndex);
    Serial.print(F(" address="));
    reportAddress(address);
    if(temp.dsSensors.isConnected(address)) Serial.print(F(" connected=true"));
    else Serial.print(F(" connected=false"));
    Serial.print(F(" tempCelsius="));
    Serial.print(temp.dsSensors.getTempC(address), 1);
    Serial.print(F(" resolutionBits="));
    Serial.print(temp.dsSensors.getResolution(address));
    Serial.println();
}


void Firmware::reportFanInfo(uint8_t fanIndex, const FanInfo &info, bool reportOnError, bool includeTimeStamp)
{
    if(!reportOnError || info.tempSpecs->hasError || info.rpmSpecs->hasError || info.rpmSpecs->hasAlert() || info.pwmSpecs->hasAlert())
    {
        if(includeTimeStamp) Serial.print(millis());
        Serial.print(" fan=");
        Serial.print(fanIndex);
        Serial.print(" sensor=");
        Serial.print(info.tempSpecs->sensorIndex);
        Serial.print(" rpm=");
        Serial.print(info.rpmSpecs->currentRpm);
        Serial.print(" power=");
        Serial.print(info.interpolator.getInterpolatedPower());
        Serial.print(" powerPercent=");
        Serial.print(info.interpolatedPowerPercent, 1);
        Serial.print(" powerPwm=");
        Serial.print(info.pwmSpecs->currentDuty);
        Serial.print(" tempCelsius=");
        Serial.print(info.tempSpecs->currentTempC, 1);
        Serial.print(" tachoError=");
        Serial.print(info.rpmSpecs->hasError);
        Serial.print(" tempError=");
        Serial.print(info.tempSpecs->hasError);
        Serial.print(" pwmAlert=");
        Serial.print(info.pwmSpecs->hasAlert());
        Serial.print(" rpmAlert=");
        Serial.print(info.rpmSpecs->hasAlert());
        Serial.print(" tempAlert=");
        Serial.print(info.tempSpecs->hasAlert());
        Serial.print(" trend=");
        Serial.println(info.trend);
    }
}


void Firmware::reportFansInfo()
{
#if defined(FAN0)
    reportFanInfo(FAN0_INDEX, controller.getFanInfo(FAN0_INDEX), false);
#endif
#if defined(FAN1)
    reportFanInfo(FAN1_INDEX, controller.getFanInfo(FAN1_INDEX), false);
#endif
#if defined(FAN2)
    reportFanInfo(FAN2_INDEX, controller.getFanInfo(FAN2_INDEX), false);
#endif
#if defined(FAN3)
    reportFanInfo(FAN3_INDEX, controller.getFanInfo(FAN3_INDEX), false);
#endif
#if defined(FAN4)
    reportFanInfo(FAN4_INDEX, controller.getFanInfo(FAN4_INDEX), false);
#endif
}

void Firmware::setup()
{

    Serial.begin(SERIAL_BAUD, SERIAL_CONFIG);

    { // display
        Wire.begin(SCREEN_WIRE_SDA, SCREEN_WIRE_SCL);
        if(!display.screen.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS, false, false))
        {
            Serial.print(millis());
            Serial.println(F(" SSD1306 allocation failed"));
        }
        display.screen.setRotation(2);
        display.screen.setTextSize(1, 1);
        display.screen.setTextColor(WHITE, BLACK);
    }

    { // PWM
        Serial.print(millis());
        if(fan.pwms.begin()) Serial.println(F(" init PWMs: OK"));
        else Serial.println(F(" init PWMs: ERROR"));
    }

    { // tacho input
        Serial.print(millis());
        if(fan.tachos.begin()) Serial.println(F(" init tachos: OK"));
        else Serial.println(F(" init tachos: ERROR"));
    }

    { // temperature sensors
        Serial.print(millis());
        Serial.println(F(" init temperature sensors ... "));

        temp.dsSensors.begin();
        temp.sensors.begin();

        Serial.print(millis());
        Serial.print(F(" reported sensor bus power is "));
        if(temp.dsSensors.isParasitePowerMode()) Serial.println("parasitic");
        else Serial.println(F("VCC"));
        Serial.print(millis());
        Serial.print(F(" discovered "));
        Serial.print(temp.dsSensors.getDeviceCount());
        Serial.println(F(" temperature sensors (index depends on bus order):"));
        for(uint8_t idx = temp.dsSensors.getDeviceCount(); idx > 0; idx--)
        {
            DeviceAddress address;
            temp.dsSensors.getAddress(address, idx - 1);
            reportTemperatureSensor(idx - 1, address);
        }

        Serial.print(millis());
        Serial.println(F(" check configured addresses (index depends on configuration order):"));
        DeviceAddress configuredAddresses[] TEMP_SENSORS_ADDRESS;
        for(uint8_t idx = sizeof(configuredAddresses) / sizeof(DeviceAddress); idx > 0; idx--)
            reportTemperatureSensor(idx - 1, configuredAddresses[idx - 1]);
    }

    { // explain trend symbols
        FanPwmSpecs pwmS{};
        FanTachoSpecs rpmS{};
        TempSensorSpecs tempS{};
        FanInfo i{ .pwmSpecs = &pwmS, .rpmSpecs = &rpmS, .tempSpecs = &tempS };

        Serial.print(millis());
        Serial.print(F(" severe input error flags: '"));
        rpmS.hasError = true;
        Serial.print(getTrendSymbol(i));
        Serial.print(F("'=tachoError '"));
        rpmS = {};
        tempS.hasError = true;
        Serial.print(getTrendSymbol(i));
        Serial.println(F("'=tempError"));
        Serial.print(millis());
        Serial.print(F(" alert flags if parameter(s) not in range: '"));
        tempS = {};
        pwmS.currentDuty = 0;
        pwmS.alertBelowDuty = 1;
        Serial.print(getTrendSymbol(i));
        Serial.print(F("'=pwmAlert '"));
        pwmS = {};
        rpmS.currentRpm = 0;
        rpmS.alertBelowRpm = 1;
        Serial.print(getTrendSymbol(i));
        Serial.print(F("'=rpmAlert '"));
        rpmS = {};
        tempS.currentTempC = 0;
        tempS.alertBelowTempC = 1;
        Serial.print(getTrendSymbol(i));
        Serial.println(F("'=tempAlert"));
    }

    Serial.print(millis());
    Serial.println(" Enter command 'h' for help.");
}


void Firmware::process()
{
    if(timers.oneSecondProcessTriggerCounterMs >= 1000)
    {
        unsigned long currentSeparationMs = timers.oneSecondProcessTriggerCounterMs;
        timers.oneSecondProcessTriggerCounterMs = 0;

        controller.process(); // must be invoked exactly each 1000ms

        display.screen.clearDisplay();
        display.screen.setCursor(0, 0);
        displayFansInfo();

#if defined(SERIAL_AUTOREPORT)
        if(timers.reportOnSerialCounterS >= timers.autoreportSeconds && timers.autoreportSeconds > 0)
        {
            timers.reportOnSerialCounterS = 0;
            reportFansInfo();
        }
#endif

        display.screen.print("sep=");
        display.screen.print(currentSeparationMs);
        display.screen.print(" exc=");
        display.screen.print(timers.oneSecondProcessTriggerCounterMs);

        display.processTickerIndex = (display.processTickerIndex + 1) % sizeof(display.processTicker);
        display.screen.setCursor(SCREEN_WIDTH_PX - 6, SCREEN_HEIGHT_PX - 7);
        display.screen.print(display.processTicker[display.processTickerIndex]);

        display.screen.display();
    }

    if(console.lineAvailable)
    {
        interpreter.process();
        console.lineAvailable = false;
    }
    else
    {
        while(Serial.available())
        {
            char &c{ console.buffer[console.index] };
            c = Serial.read();
            if(c == '\r')
                ; // ignore \r
            else if(c == '\n')
            {
#if defined(SERIAL_ECHO_ON)
                Serial.print(c);
#endif
                c = 0;
                console.lineAvailable = true;
                console.index = 0;
            }
            else
            {
#if defined(SERIAL_ECHO_ON)
                Serial.print(c);
#endif
                console.index++;
            }
        }
    }
}
