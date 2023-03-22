#pragma once

#include "controller/FansController.h"
#include "fan/FansPwm.h"
#include "fan/FansTacho.h"
#include "settings/FlashSettings.h"
#include "temp/TempSensors.h"
#include <Arduino.h>
#include <map>
#include <string>


template <uint8_t BufferSize = 128> struct ConsoleInterpreter
{
    using CommandCallback = bool (ConsoleInterpreter<BufferSize>::*)(char (&)[BufferSize]);
    using ClassType = ConsoleInterpreter<BufferSize>;

    struct Command
    {
        char name{ 0 };             // name, ame as input on serial console
        CommandCallback callback{}; // implementation
        std::string help{};         // help text for console
    };


    ConsoleInterpreter(char (&buffer)[BufferSize], FansController &controller, unsigned long &autoreportDelayS, FlashSettings &settings) :
    buffer(buffer), controller(controller), autoreportDelayS(autoreportDelayS), settings(settings)
    {
        // clang-format off
        commandList.push_back({ .name='h', .callback = &ClassType::commandPrintHelp,              .help = "print this help text:              h" });
        commandList.push_back({ .name='f', .callback = &ClassType::commandPrintFans,              .help = "print fan(s):                      f [fanIndex]?" });
        commandList.push_back({ .name='A', .callback = &ClassType::commandSetAutoreportSeconds,   .help = "set   auto report seconds:         A [reportSeconds]" });
        commandList.push_back({ .name='a', .callback = &ClassType::commandPrintAutoreportSeconds, .help = "print auto report seconds:         a" });
        commandList.push_back({ .name='C', .callback = &ClassType::commandSetPowerCurve,          .help = "set   power curve:                 C [fanIndex] [temp0] [power0] [temp1] [power1] [temp2] [power2] [temp3] [power3]" });
        commandList.push_back({ .name='c', .callback = &ClassType::commandPrintPowerCurve,        .help = "print power curve(s):              c [fanIndex]?" });
        commandList.push_back({ .name='P', .callback = &ClassType::commandSetPwmAlert,            .help = "set   PWM alert:                   P [fanIndex] [pwmMin] [pamMax]" });
        commandList.push_back({ .name='p', .callback = &ClassType::commandPrintPwmAlert,          .help = "print PWM alert(s):                p [fanIndex]?" });
        commandList.push_back({ .name='R', .callback = &ClassType::commandSetRpmAlert,            .help = "set   RPM alert:                   R [fanIndex] [rpmMin] [rpmMax]" });
        commandList.push_back({ .name='r', .callback = &ClassType::commandPrintRpmAlert,          .help = "print RPM alert(s):                r [fanIndex]?" });
        commandList.push_back({ .name='T', .callback = &ClassType::commandSetTemperatureAlert,    .help = "set   temperature alert:           T [fanIndex] [tempMin] [tempMax]" });
        commandList.push_back({ .name='t', .callback = &ClassType::commandPrintTemperatureAlert,  .help = "print temperature alert(s):        t [fanIndex]?" });
        commandList.push_back({ .name='I', .callback = &ClassType::commandSetTemperatureSensor,   .help = "set   temperature sensor index:    I [fanIndex] [tempSensorIndex]" });
        commandList.push_back({ .name='i', .callback = &ClassType::commandPrintTemperatureSensor, .help = "print temperature sensor(s) index: i [fanIndex]?" });
        commandList.push_back({ .name='S', .callback = &ClassType::commandSaveSettings,           .help = "save  settings:                    S" });
        commandList.push_back({ .name='s', .callback = &ClassType::commandLoadSettings,           .help = "load  settings:                    s" });
        commandList.push_back({ .name='x', .callback = &ClassType::commandResetSettings,          .help = "reset settings:                    x" });
        commandList.push_back({ .name='X', .callback = &ClassType::commandReboot,                 .help = "reboot device:                     X" });
        // clang-format on

        for(const auto &item : commandList)
            commandMap[item.name] = item.callback;
    }


    void process()
    {
        auto callbackIter{ commandMap.find(buffer[0]) };
        if(callbackIter == commandMap.end())
        {
            Serial.print("unknown command: ");
            Serial.println(buffer);
        }
        else
        {
            if(!(this->*(callbackIter->second))(buffer))
            {
                Serial.print("failed to execute command '");
                Serial.print(static_cast<char>(callbackIter->first));
                Serial.println("'");
            }
        }
    }


protected:
    bool printFan(uint8_t fanIndex)
    {
        const FanInfo &info{ controller.getFanInfo(fanIndex) };

        Serial.print("fan=");
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
        Serial.print(info.trend);
        Serial.print(" tempAlertBelow=");
        Serial.print(info.tempSpecs->alertBelowTempC, 1);
        Serial.print(" tempAlertAbove=");
        Serial.print(info.tempSpecs->alertAboveTempC, 1);
        Serial.print(" pwmAlertBelow=");
        Serial.print(info.pwmSpecs->alertBelowDuty);
        Serial.print(" pwmAlertAbove=");
        Serial.print(info.pwmSpecs->alertAboveDuty);
        Serial.print(" rpmAlertBelow=");
        Serial.print(info.rpmSpecs->alertBelowRpm);
        Serial.print(" rpmAlertAbove=");
        Serial.print(info.rpmSpecs->alertAboveRpm);
        Serial.println();
        return true;
    }


    bool commandPrintFans(char (&line)[BufferSize])
    {
        // examples: "f", "f 0", "f 4"
        char c;
        uint8_t fanIndex;
        if(2 <= sscanf(line, "%c %hhu", &c, &fanIndex))
        {
            if(isAnyFanWithIndexDefined(fanIndex)) return printFan(fanIndex);
            return false;
        }
        for(auto idx : definedFanIndices)
            printFan(idx);
        return true;
    }


    bool commandPrintHelp(char (&line)[BufferSize])
    {
        // examples: "h"
        Serial.println("Help:");
        for(const auto c : commandList)
        {
            Serial.print(c.name);
            Serial.print("  ");
            Serial.println(c.help.c_str());
        }

        Serial.println("Units:");
        Serial.println("fanIndex:        uint8");
        Serial.println("tempSensorIndex: uint8");
        Serial.println("reportSeconds:   uint16, 0=disable");
        Serial.println("power[N]:        uint8");
        Serial.println("pwm[Min|Max]:    uint8");
        Serial.println("rpm[Min|Max]:    uint16");
        Serial.println("temp[Min|Max]:   int16, 1/10 Celsius (0.1C), i.e. -273.1C=-2731, 150.1C=1501");
        Serial.println("temp[N]:         int16, 1/10 Celsius (0.1C), i.e. -273.1C=-2731, 150.1C=1501, temp[N] < temp[N+1]");

        return true;
    }

    bool commandSetAutoreportSeconds(char (&line)[BufferSize])
    {
        // examples: "A0", "A 0", "A60", "A 60"
        char c;
        unsigned long autoreportSeconds;
        if(2 <= sscanf(line, "%c %lu", &c, &autoreportSeconds))
        {
            autoreportDelayS = autoreportSeconds;
            return true;
        }
        return false;
    }


    bool commandPrintAutoreportSeconds(char (&)[BufferSize])
    {
        // examples: "a", "a "
        Serial.print("A ");
        Serial.println(autoreportDelayS);
        return true;
    }


    bool commandSetPowerCurve(char (&line)[BufferSize])
    {
        // example: "C 0 8 57 256 55 804 103 802 101"
        // fan0 temp0=8 power0=57 temp1=256 power1=55 temp2=804 power2=103 temp3=802 power3=101
        char c;
        uint8_t fanIndex;
        int16_t temp[4];
        uint8_t power[4];
        if((2 + 2 * 4) <= sscanf(line, "%c %hhu %hd %hhu %hd %hhu %hd %hhu %hd %hhu", &c, &fanIndex, &temp[0], &power[0],
                                 &temp[1], &power[1], &temp[2], &power[2], &temp[3], &power[3]))
        {
            if(isAnyFanWithIndexDefined(fanIndex))
                return controller.getFanInfo(fanIndex).interpolator.setPowerCurvePoints(power, temp);
        }
        return false;
    }


    bool printFanCurve(uint8_t fanIndex)
    {
        const auto &curve{ controller.getFanInfo(fanIndex).interpolator.getCurve() };

        Serial.print("C ");
        Serial.print(fanIndex);
        Serial.print(" ");
        Serial.print(curve[0].tempCentiCelsius);
        Serial.print(" ");
        Serial.print(curve[0].power);

        Serial.print(" ");
        Serial.print(curve[1].tempCentiCelsius);
        Serial.print(" ");
        Serial.print(curve[1].power);

        Serial.print(" ");
        Serial.print(curve[2].tempCentiCelsius);
        Serial.print(" ");
        Serial.print(curve[2].power);

        Serial.print(" ");
        Serial.print(curve[3].tempCentiCelsius);
        Serial.print(" ");
        Serial.print(curve[3].power);

        Serial.println();
        return true;
    }

    bool commandPrintPowerCurve(char (&line)[BufferSize])
    {
        // examples: "c", "c 0"
        char c;
        uint8_t fanIndex;
        if(2 <= sscanf(line, "%c %hhu", &c, &fanIndex))
        {
            if(isAnyFanWithIndexDefined(fanIndex)) return printFanCurve(fanIndex);
            return false;
        }
        for(auto idx : definedFanIndices)
            printFanCurve(idx);
        return true;
    }


    bool setPwmAlert(uint8_t fanIndex, uint8_t pwmMin, uint8_t pwmMax)
    {
        FanPwmSpec &pwmInfo{ *controller.getFanInfo(fanIndex).pwmSpecs };
        pwmInfo.alertBelowDuty = pwmMin;
        pwmInfo.alertAboveDuty = pwmMax;
        return true;
    }


    bool setRpmAlert(uint8_t fanIndex, uint8_t rpmMin, uint8_t rpmMax)
    {
        FanTachoSpec &rpmInfo{ *controller.getFanInfo(fanIndex).rpmSpecs };
        rpmInfo.alertBelowRpm = rpmMin;
        rpmInfo.alertAboveRpm = rpmMax;
        return true;
    }


    bool setTempAlert(uint8_t fanIndex, const float &rpmMin, const float &rpmMax)
    {
        TempSensorSpec &tempInfo{ *controller.getFanInfo(fanIndex).tempSpecs };
        tempInfo.alertBelowTempC = rpmMin;
        tempInfo.alertAboveTempC = rpmMax;
        return true;
    }


    bool commandSetPwmAlert(char (&line)[BufferSize])
    {
        // examples: "P 0 0 100", "P0 0 100"
        char c;
        uint8_t fanIndex;
        uint8_t pwmMin, pwmMax;
        if(4 <= sscanf(line, "%c %hhu %hhu %hhu", &c, &fanIndex, &pwmMin, &pwmMax))
        {
            if(isAnyFanWithIndexDefined(fanIndex)) return setPwmAlert(fanIndex, pwmMin, pwmMax);
        }
        return false;
    }


    bool printPwmAlert(uint8_t fanIndex)
    {
        Serial.print("P ");
        Serial.print(fanIndex);
        Serial.print(" ");
        Serial.print(controller.getFanInfo(fanIndex).pwmSpecs->alertBelowDuty);
        Serial.print(" ");
        Serial.print(controller.getFanInfo(fanIndex).pwmSpecs->alertAboveDuty);
        Serial.println();
        return true;
    }

    bool printRpmAlert(uint8_t fanIndex)
    {
        Serial.print("R ");
        Serial.print(fanIndex);
        Serial.print(" ");
        Serial.print(controller.getFanInfo(fanIndex).rpmSpecs->alertBelowRpm);
        Serial.print(" ");
        Serial.print(controller.getFanInfo(fanIndex).rpmSpecs->alertAboveRpm);
        Serial.println();
        return true;
    }

    bool printTempAlert(uint8_t fanIndex)
    {
        Serial.print("R ");
        Serial.print(fanIndex);
        Serial.print(" ");
        Serial.print(controller.getFanInfo(fanIndex).tempSpecs->alertBelowTempC * 10, 1);
        Serial.print(" ");
        Serial.print(controller.getFanInfo(fanIndex).tempSpecs->alertAboveTempC * 10, 1);
        Serial.println();
        return true;
    }

    bool commandPrintPwmAlert(char (&line)[BufferSize])
    {
        // examples: "p", "p0", "p 0"
        char c;
        uint8_t fanIndex;
        if(4 <= sscanf(line, "%c %hhu", &c, &fanIndex))
        {
            if(isAnyFanWithIndexDefined(fanIndex)) return printPwmAlert(fanIndex);
            return false;
        }

        for(auto idx : definedFanIndices)
            printPwmAlert(idx);
        return true;
    }


    bool commandSetRpmAlert(char (&line)[BufferSize])
    {
        // examples: "R 0 0 10000", "R0 0 10000"
        char c;
        uint8_t fanIndex;
        uint8_t rpmMin, rpmMax;
        if(4 <= sscanf(line, "%c %hhu %hhu %hhu", &c, &fanIndex, &rpmMin, &rpmMax))
        {
            if(isAnyFanWithIndexDefined(fanIndex)) return setRpmAlert(fanIndex, rpmMin, rpmMax);
        }
        return false;
    }


    bool commandPrintRpmAlert(char (&line)[BufferSize])
    {
        // examples: "r", "r0", "r 0"
        char c;
        uint8_t fanIndex;
        if(4 <= sscanf(line, "%c %hhu", &c, &fanIndex))
        {
            if(isAnyFanWithIndexDefined(fanIndex)) return printRpmAlert(fanIndex);
            return false;
        }

        for(auto idx : definedFanIndices)
            printRpmAlert(idx);

        return true;
    }

    bool commandSetTemperatureAlert(char (&line)[BufferSize])
    {
        // examples: "T 0 0 1501", "T0 0 1501"
        char c;
        uint8_t fanIndex;
        int16_t tempMinCentiC, tempMaxCentiC;

        if(4 <= sscanf(line, "%c %hhu %hd %hd", &c, &fanIndex, &tempMinCentiC, &tempMaxCentiC))
        {
            const float tempMin{ static_cast<float>(tempMinCentiC) * 10.0f }, tempMax{ static_cast<float>(tempMaxCentiC) * 10.0f };
            if(isAnyFanWithIndexDefined(fanIndex)) return setTempAlert(fanIndex, tempMin, tempMax);
        }
        return false;
    }


    bool commandPrintTemperatureAlert(char (&line)[BufferSize])
    {
        // examples: "t", "t0", "t 0"
        char c;
        uint8_t fanIndex;
        if(4 <= sscanf(line, "%c %hhu", &c, &fanIndex))
        {
            if(isAnyFanWithIndexDefined(fanIndex)) return printTempAlert(fanIndex);
            return false;
        }

        for(auto idx : definedFanIndices)
            printTempAlert(idx);

        return true;
    }


    bool commandSetTemperatureSensor(char (&line)[BufferSize])
    {
        // examples: "I0 1", "I 0 1"
        char c;
        uint8_t fanIndex, tempSensorIndex;
        if(3 <= sscanf(line, "%c %hhu %hhu", &c, &fanIndex, &tempSensorIndex))
            return controller.updateFanTempSensorIndex(fanIndex, tempSensorIndex);
        return false;
    }


    bool printTemperatureSensorIndex(uint8_t fanIndex)
    {
        Serial.print("I ");
        Serial.print(fanIndex);
        Serial.print(" ");
        Serial.print(controller.getFanInfo(fanIndex).tempSpecs->sensorIndex);
        Serial.println();
        return true;
    }


    bool commandPrintTemperatureSensor(char (&line)[BufferSize])
    {
        // examples: "i", "i0", "i 0"
        char c;
        uint8_t fanIndex;
        if(4 <= sscanf(line, "%c %hhu", &c, &fanIndex))
        {
            if(isAnyFanWithIndexDefined(fanIndex)) return printTemperatureSensorIndex(fanIndex);
            return false;
        }

        for(auto idx : definedFanIndices)
            printTemperatureSensorIndex(idx);
        return true;
    }


    bool commandSaveSettings(char (&)[BufferSize]) { return settings.storeSettings(); }


    bool commandLoadSettings(char (&)[BufferSize]) { return settings.loadSettings(); }


    bool commandResetSettings(char (&)[BufferSize]) { return settings.resetSettings(); }

    bool commandReboot(char (&)[BufferSize])
    {
        ESP.restart();
        return true;
    }


    char (&buffer)[BufferSize];
    FansController &controller;
    unsigned long &autoreportDelayS;
    FlashSettings &settings;
    std::unordered_map<uint8_t, CommandCallback> commandMap;
    std::vector<Command> commandList;
};
