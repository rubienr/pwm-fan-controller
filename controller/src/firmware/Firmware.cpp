#include "Firmware.h"
#include "LittleFS.h"

char Firmware::getTrendSymbol(const FanInfo &info)
{
    if(info.temperatureSensorSpec->hasError) return 'S';

    if(info.rpmSpec->hasError) return 'R';
    if(info.rpmSpec->hasAlert()) return 'r';

    if(info.pwmSpec->hasError) return 'P';
    if(info.pwmSpec->hasAlert()) return 'p';

    if(info.fanTemperatureSpec->hasError) return 'T';
    if(info.fanTemperatureSpec->hasAlert()) return 't';

    if(info.trend > 0) return '^';
    if(info.trend < 0) return 'v';
    return '-';
}


void Firmware::displayFan(uint8_t fanIndex)
{
    if(controller.hasError(fanIndex) || controller.hasAlert(fanIndex)) display.screen.setTextColor(BLACK, WHITE);

    display.screen.print(fanIndex);
    const FanInfo &info{ controller.getFanInfo(fanIndex) };

    display.screen.print(F(" "));
    int16_t rpm{ info.rpmSpec->currentRpm };
    if(rpm < 10) display.screen.print(F(" "));
    if(rpm < 100) display.screen.print(F(" "));
    if(rpm < 1000) display.screen.print(F(" "));
    if(rpm < 10000) display.screen.print(F(" "));
    display.screen.print(rpm);

    display.screen.print(F(" "));
    float tempCelsius{ info.fanTemperatureSpec->currentTempC };
    if(tempCelsius > -100 && tempCelsius < 0) display.screen.print(F(" "));
    if(tempCelsius > -10 && tempCelsius < 0) display.screen.print(F(" "));
    if(tempCelsius >= 0) display.screen.print(F(" ")); // reserved space for negative sign
    if(tempCelsius < 10 && tempCelsius >= 0) display.screen.print(F(" "));
    if(tempCelsius < 100 && tempCelsius >= 0) display.screen.print(F(" "));
    display.screen.print(tempCelsius, 1);

    display.screen.print(F(" "));
    auto power{ info.interpolatedPowerPercent };
    if(power < 10) display.screen.print(F(" "));
    if(power < 100) display.screen.print(F(" "));
    display.screen.print(power, 1);

    display.screen.println();
    display.screen.setTextColor(WHITE, BLACK);
}


void Firmware::displayFansInfo()
{
    display.screen.println(F("f rnd/m degreC power%"));

    for(auto fanIndex : definedFanIndices)
        displayFan(fanIndex);

    display.screen.print(F("Trend ["));
    for(auto fanIndex : definedFanIndices)
    {
        display.screen.print(F(" "));
        const FanInfo &info{ controller.getFanInfo(fanIndex) };
        display.screen.print(getTrendSymbol(info));
        reportFanInfo(fanIndex, info);
    }
    display.screen.println(F(" ]"));
}


void Firmware::reportAddress(const DeviceAddress &deviceAddress)
{
    for(unsigned char c : deviceAddress)
    {
        if(c < 16) Serial.print(0);
        Serial.print(c, HEX);
    }
}


void Firmware::reportTemperatureSensor(uint8_t sensorIndex, const DeviceAddress &address)
{
    Serial.print(millis());
    Serial.print(F(" # sensorIndex="));
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
    if(!reportOnError || info.temperatureSensorSpec->hasError || info.rpmSpec->hasError || info.rpmSpec->hasAlert() ||
       info.pwmSpec->hasAlert())
    {
        if(includeTimeStamp) Serial.print(millis());
        Serial.print(F(" fan="));
        Serial.print(fanIndex);
        Serial.print(F(" sensor="));
        Serial.print(info.fanTemperatureSpec->temperatureSensorIndex);
        Serial.print(F(" rpm="));
        Serial.print(info.rpmSpec->currentRpm);
        Serial.print(F(" power="));
        Serial.print(info.interpolator.getInterpolatedPower());
        Serial.print(F(" powerPercent="));
        Serial.print(info.interpolatedPowerPercent, 1);
        Serial.print(F(" powerPwm="));
        Serial.print(info.pwmSpec->currentDuty);
        Serial.print(F(" tempCelsius="));
        Serial.print(info.fanTemperatureSpec->currentTempC, 1);
        Serial.print(F(" tachoError="));
        Serial.print(info.rpmSpec->hasError);
        Serial.print(F(" tempError="));
        Serial.print(info.fanTemperatureSpec->hasError);
        Serial.print(F(" tempSensorError="));
        Serial.print(info.temperatureSensorSpec->hasError);
        Serial.print(F(" pwmAlert="));
        Serial.print(info.pwmSpec->hasAlert());
        Serial.print(F(" rpmAlert="));
        Serial.print(info.rpmSpec->hasAlert());
        Serial.print(F(" tempAlert="));
        Serial.print(info.fanTemperatureSpec->hasAlert());
        Serial.print(F(" trend="));
        Serial.println(info.trend);
    }
}


void Firmware::reportFansInfo() const
{
    for(auto fanIndex : definedFanIndices)
        reportFanInfo(fanIndex, controller.getFanInfo(fanIndex), false);
}

void Firmware::setup()
{
    Serial.begin(SERIAL_BAUD, SERIAL_CONFIG);
    { // firmware info
        Serial.print(millis());
        Serial.print(F(" # PWM controller version "));
        Serial.print(version.versionNumber.major);
        Serial.print('.');
        Serial.print(version.versionNumber.minor);
        Serial.print('.');
        Serial.print(version.versionNumber.patch);
        Serial.print(F(" built "));
        Serial.print(version.buildTimestamp);
        Serial.println();

        Serial.print(millis());
        Serial.print(F(" # fans="));
        Serial.print(getDefinedFansCount());
        Serial.print(F(" sensors="));
        Serial.println(getDefinedTemperatureSensorsCount());

        Serial.print(millis());
        Serial.println(F(" # Notes:"));
        Serial.print(millis());
        Serial.println(F(" #   - lines with leading '#' are human readable logs"));
        Serial.print(millis());
        Serial.println(F(" #   - lines without leading '#' are slightly machine parseable"));
    }

    { // filesystem and stored settings
        if(LittleFS.begin(true))
        {
            Serial.print(millis());
            Serial.print(F(" # LittleFS mounted: used="));
            Serial.print(LittleFS.usedBytes());
            Serial.print(F(", total="));
            Serial.print(LittleFS.totalBytes());
            Serial.println(F(" [Bytes]"));
            settings.storage.loadSettings();
        }
        else
        {
            Serial.print(millis());
            Serial.println(F(" # failed to mount LittleFS, use default settings"));
        }
    }

    { // display
        Wire.begin(SCREEN_WIRE_SDA, SCREEN_WIRE_SCL);
        if(!display.screen.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS, false, false))
        {
            Serial.print(millis());
            Serial.println(F(" # SSD1306 allocation failed"));
        }
        display.screen.setRotation(2);
        display.screen.setTextSize(1, 1);
        display.screen.setTextColor(WHITE, BLACK);
    }

    { // PWM
        if(!fan.pwms.begin())
        {
            Serial.print(millis());
            Serial.println(F(" # init PWMs: ERROR"));
        }
    }

    { // tacho input

        if(!fan.tachos.begin())
        {
            Serial.print(millis());
            Serial.println(F(" # init tachos: ERROR"));
        }
    }

    { // temperature sensors
        Serial.print(millis());
        Serial.println(F(" # init temperature sensors ... "));

        temp.dsSensors.begin();
        temp.sensors.begin();

        Serial.print(millis());
        Serial.print(F(" # reported sensor bus power is "));
        if(temp.dsSensors.isParasitePowerMode()) Serial.println(F("parasitic"));
        else Serial.println(F("VCC"));
        Serial.print(millis());
        Serial.print(F(" # discovered "));
        Serial.print(temp.dsSensors.getDeviceCount());
        Serial.println(F(" temperature sensors (index depends on bus order):"));
        for(uint8_t idx{ 0 }; idx < temp.dsSensors.getDeviceCount(); idx++)
        {
            DeviceAddress address;
            temp.dsSensors.getAddress(address, idx);
            reportTemperatureSensor(idx, address);
        }

        Serial.print(millis());
        Serial.println(F(" # check configured addresses (index depends on configuration order):"));

        for(const auto &idx : definedTemperatureSensorIndices)
            reportTemperatureSensor(idx, definedTemperatureSensors[idx]);
    }

    { // explain trend symbols
        FanPwmSpec pwmS{};
        FanTachoSpec rpmS{};
        FanTemperatureSpec fanS{};
        TempSensorSpec tempS{};
        FanInfo i{ .pwmSpec = &pwmS, .rpmSpec = &rpmS, .fanTemperatureSpec = &fanS, .temperatureSensorSpec = &tempS };

        Serial.print(millis());
        Serial.print(F(" # severe input error flags: '"));

        rpmS.hasError = true;
        Serial.print(getTrendSymbol(i));
        Serial.print(F("'=tachoError '"));

        rpmS = {};
        fanS.hasError = true;
        Serial.print(getTrendSymbol(i));
        Serial.print(F("'=tempError '"));

        fanS = {};
        tempS.hasError = true;
        Serial.print(getTrendSymbol(i));
        Serial.print(F("'=tempSensorError"));

        Serial.println();
        Serial.print(millis());
        Serial.print(F(" # alert flags if parameter(s) not in range: '"));

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
        fanS.currentTempC = 0;
        fanS.alertBelowTempC = 1;
        Serial.print(getTrendSymbol(i));
        Serial.print(F("'=tempAlert"));

        Serial.println();
    }

    Serial.print(millis());
    Serial.println(F(" # Enter command 'h' for help."));
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

        display.screen.print(F("sep="));
        display.screen.print(currentSeparationMs);
        display.screen.print(F(" exc="));
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
