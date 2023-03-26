#pragma once

#include "../controller/FansController.h"
#include "../fan/FansPwm.h"
#include "../fan/FansTacho.h"
#include "../fan/FansTemperature.h"
#include "../firmware/ConfigurationHook.h"
#include "../sensors/TempSensors.h"
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


    ConsoleInterpreter(char (&buffer)[BufferSize], ConfigurationHook &configurationHook) :
    buffer(buffer), configurationHook(configurationHook)
    {
        // clang-format off
        commandList.push_back({ .name='h', .callback = &ClassType::commandPrintHelp,           .help = "print this help text:                h" });
        commandList.push_back({ .name='f', .callback = &ClassType::commandPrintFans,           .help = "print fan(s):                        f [fanIndex]?" });
        commandList.push_back({ .name='L', .callback = &ClassType::commandSetAutoreportSec,    .help = "set   auto report seconds:           L [reportSeconds]" });
        commandList.push_back({ .name='l', .callback = &ClassType::commandPrintAutoreportSec,  .help = "print auto report seconds:           l" });
        commandList.push_back({ .name='C', .callback = &ClassType::commandSetPowerCurve,       .help = "set   power curve:                   C [fanIndex] [temp0] [power0] [temp1] [power1] [temp2] [power2] [temp3] [power3]" });
        commandList.push_back({ .name='c', .callback = &ClassType::commandPrintPowerCurve,     .help = "print power curve(s):                c [fanIndex]?" });
        commandList.push_back({ .name='P', .callback = &ClassType::commandSetPwmAlert,         .help = "set   PWM alert:                     P [fanIndex] [pwmMin] [pamMax]" });
        commandList.push_back({ .name='p', .callback = &ClassType::commandPrintPwmAlert,       .help = "print PWM alert(s):                  p [fanIndex]?" });
        commandList.push_back({ .name='R', .callback = &ClassType::commandSetRpmAlert,         .help = "set   RPM alert:                     R [fanIndex] [rpmMin] [rpmMax]" });
        commandList.push_back({ .name='r', .callback = &ClassType::commandPrintRpmAlert,       .help = "print RPM alert(s):                  r [fanIndex]?" });
        commandList.push_back({ .name='T', .callback = &ClassType::commandSetTempAlert,        .help = "set   temperature alert:             T [fanIndex] [tempMin] [tempMax]" });
        commandList.push_back({ .name='t', .callback = &ClassType::commandPrintTempAlert,      .help = "print temperature alert(s):          t [fanIndex]?" });
        commandList.push_back({ .name='I', .callback = &ClassType::commandSetTempSensorIndex,  .help = "set   temperature sensor index:      I [fanIndex] [tempSensorIndex]" });
        commandList.push_back({ .name='i', .callback = &ClassType::commandPrintTempSensorIndex,.help = "print temperature sensor(s) index:   i [fanIndex]?" });
        commandList.push_back({ .name='A', .callback = &ClassType::commandSetTempSensorAddr,   .help = "set   temperature sensor address:    A [tempSensorIndex] [aa] [aa] [aa] [aa] [aa] [aa] [aa] [aa]" });
        commandList.push_back({ .name='a', .callback = &ClassType::commandPrintTempSensorAddr, .help = "print temperature sensor(s) address: a [tempSensorIndex]?" });
        commandList.push_back({ .name='S', .callback = &ClassType::commandSaveSettings,        .help = "save  settings:                      S" });
        commandList.push_back({ .name='s', .callback = &ClassType::commandLoadSettings,        .help = "load  settings:                      s" });
        commandList.push_back({ .name='d', .callback = &ClassType::commandPrintSettings,       .help = "print settings:                      d" });
        commandList.push_back({ .name='x', .callback = &ClassType::commandResetSettings,       .help = "reset settings:                      x" });
        commandList.push_back({ .name='X', .callback = &ClassType::commandReboot,              .help = "reboot device:                       X" });
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
        const FanInfo &info{ configurationHook.getFanInfo(fanIndex) };

        Serial.print("fan=");
        Serial.print(fanIndex);
        Serial.print(" sensor=");
        Serial.print(info.fanTemperatureSpec->temperatureSensorIndex);
        Serial.print(" rpm=");
        Serial.print(info.rpmSpec->currentRpm);
        Serial.print(" power=");
        Serial.print(info.interpolator.getInterpolatedPower());
        Serial.print(" powerPercent=");
        Serial.print(info.interpolatedPowerPercent, 1);
        Serial.print(" powerPwm=");
        Serial.print(info.pwmSpec->currentDuty);
        Serial.print(" tempCelsius=");
        Serial.print(info.fanTemperatureSpec->currentTempC, 1);
        Serial.print(" tachoError=");
        Serial.print(info.rpmSpec->hasError);
        Serial.print(" tempError=");
        Serial.print(info.fanTemperatureSpec->hasError);
        Serial.print(" pwmAlert=");
        Serial.print(info.pwmSpec->hasAlert());
        Serial.print(" rpmAlert=");
        Serial.print(info.rpmSpec->hasAlert());
        Serial.print(" tempAlert=");
        Serial.print(info.fanTemperatureSpec->hasAlert());
        Serial.print(" trend=");
        Serial.print(info.trend);
        Serial.print(" tempAlertBelow=");
        Serial.print(info.fanTemperatureSpec->alertBelowTempC, 1);
        Serial.print(" tempAlertAbove=");
        Serial.print(info.fanTemperatureSpec->alertAboveTempC, 1);
        Serial.print(" pwmAlertBelow=");
        Serial.print(info.pwmSpec->alertBelowDuty);
        Serial.print(" pwmAlertAbove=");
        Serial.print(info.pwmSpec->alertAboveDuty);
        Serial.print(" rpmAlertBelow=");
        Serial.print(info.rpmSpec->alertBelowRpm);
        Serial.print(" rpmAlertAbove=");
        Serial.print(info.rpmSpec->alertAboveRpm);
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
            if(isFanDefined(fanIndex)) return printFan(fanIndex);
            return false;
        }
        for(auto idx : definedFanIndices)
            printFan(idx);
        return true;
    }


    bool commandPrintHelp(char (&line)[BufferSize])
    {
        // examples: "h"
        Serial.print(millis());
        Serial.println(" # Help:");
        for(const auto c : commandList)
        {
            Serial.print(millis());
            Serial.print(" # ");
            Serial.print(c.name);
            Serial.print("  ");
            Serial.println(c.help.c_str());
        }

        Serial.print(millis());
        Serial.println(" # Units:");
        Serial.print(millis());
        Serial.println(" # fanIndex:        uint8");
        Serial.print(millis());
        Serial.println(" # tempSensorIndex: uint8");
        Serial.print(millis());
        Serial.println(" # reportSeconds:   uint16, 0=disable");
        Serial.print(millis());
        Serial.println(" # power[N]:        uint8");
        Serial.print(millis());
        Serial.println(" # pwm[Min|Max]:    uint8");
        Serial.print(millis());
        Serial.println(" # rpm[Min|Max]:    uint16");
        Serial.print(millis());
        Serial.println(" # temp[Min|Max]:   int16, 1/10 Celsius (0.1C), i.e. -273.1C=-2731, 150.1C=1501");
        Serial.print(millis());
        Serial.println(" # temp[N]:         int16, 1/10 Celsius (0.1C), i.e. -273.1C=-2731, 150.1C=1501, temp[N] < temp[N+1]");
        Serial.print(millis());
        Serial.println(" # [aa]:            1-byte HEX address, i.e. AA, Fe, be");
        return true;
    }

    bool commandSetAutoreportSec(char (&line)[BufferSize])
    {
        // examples: "L0", "L 0", "L60", "L 60"
        char c;
        unsigned long autoreportSeconds;
        if(2 <= sscanf(line, "%c %lu", &c, &autoreportSeconds))
        {
            configurationHook.setAutoreportDelaySeconds(autoreportSeconds);
            return true;
        }
        return false;
    }


    bool commandPrintAutoreportSec(char (&)[BufferSize])
    {
        // examples: "l", "l "
        Serial.print("A ");
        Serial.println(configurationHook.getAutoreportDelaySeconds());
        return true;
    }


    bool commandSetPowerCurve(char (&line)[BufferSize])
    {
        // example: "C 0 8 57 256 55 804 103 802 101"
        // fan0 temp0=8 power0=57 temp1=256 power1=55 temp2=804 power2=103 temp3=802 power3=101
        char c;
        uint8_t fanIndex;
        int16_t temp[numCurvePoints];
        uint8_t power[numCurvePoints];
        static_assert(numCurvePoints == 4);
        if((2 + 2 * 4) <= sscanf(line, "%c %hhu %hd %hhu %hd %hhu %hd %hhu %hd %hhu", &c, &fanIndex, &temp[0], &power[0],
                                 &temp[1], &power[1], &temp[2], &power[2], &temp[3], &power[3]))
        {
            if(isFanDefined(fanIndex)) return configurationHook.setPowerCurvePoints(fanIndex, power, temp);
        }
        return false;
    }


    bool printFanCurve(uint8_t fanIndex)
    {
        const auto &curve{ configurationHook.getFanInfo(fanIndex).interpolator.getCurve() };

        Serial.print("C ");
        Serial.print(fanIndex);
        Serial.print(" ");
        Serial.print(curve[0].tempDeciCelsius);
        Serial.print(" ");
        Serial.print(curve[0].power);

        Serial.print(" ");
        Serial.print(curve[1].tempDeciCelsius);
        Serial.print(" ");
        Serial.print(curve[1].power);

        Serial.print(" ");
        Serial.print(curve[2].tempDeciCelsius);
        Serial.print(" ");
        Serial.print(curve[2].power);

        Serial.print(" ");
        Serial.print(curve[3].tempDeciCelsius);
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
            if(isFanDefined(fanIndex)) return printFanCurve(fanIndex);
            return false;
        }
        for(auto idx : definedFanIndices)
            printFanCurve(idx);
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
            if(isFanDefined(fanIndex)) return configurationHook.setPwmAlert(fanIndex, pwmMin, pwmMax);
        }
        return false;
    }


    bool printPwmAlert(uint8_t fanIndex)
    {
        Serial.print("P ");
        Serial.print(fanIndex);
        Serial.print(" ");
        Serial.print(configurationHook.getFanInfo(fanIndex).pwmSpec->alertBelowDuty);
        Serial.print(" ");
        Serial.print(configurationHook.getFanInfo(fanIndex).pwmSpec->alertAboveDuty);
        Serial.println();
        return true;
    }

    bool printRpmAlert(uint8_t fanIndex)
    {
        Serial.print("R ");
        Serial.print(fanIndex);
        Serial.print(" ");
        Serial.print(configurationHook.getFanInfo(fanIndex).rpmSpec->alertBelowRpm);
        Serial.print(" ");
        Serial.print(configurationHook.getFanInfo(fanIndex).rpmSpec->alertAboveRpm);
        Serial.println();
        return true;
    }

    bool printTempAlert(uint8_t fanIndex)
    {
        Serial.print("R ");
        Serial.print(fanIndex);
        Serial.print(" ");
        Serial.print(configurationHook.getFanInfo(fanIndex).fanTemperatureSpec->alertBelowTempC * 10, 0);
        Serial.print(" ");
        Serial.print(configurationHook.getFanInfo(fanIndex).fanTemperatureSpec->alertAboveTempC * 10, 0);
        Serial.println();
        return true;
    }

    bool commandPrintPwmAlert(char (&line)[BufferSize])
    {
        // examples: "p", "p0", "p 0"
        char c;
        uint8_t fanIndex;
        if(2 <= sscanf(line, "%c %hhu", &c, &fanIndex))
        {
            if(isFanDefined(fanIndex)) return printPwmAlert(fanIndex);
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
            if(isFanDefined(fanIndex)) return configurationHook.setRpmAlert(fanIndex, rpmMin, rpmMax);
        }
        return false;
    }


    bool commandPrintRpmAlert(char (&line)[BufferSize])
    {
        // examples: "r", "r0", "r 0"
        char c;
        uint8_t fanIndex;
        if(2 <= sscanf(line, "%c %hhu", &c, &fanIndex))
        {
            if(isFanDefined(fanIndex)) return printRpmAlert(fanIndex);
            return false;
        }

        for(auto idx : definedFanIndices)
            printRpmAlert(idx);

        return true;
    }

    bool commandSetTempAlert(char (&line)[BufferSize])
    {
        // examples: "T 0 0 1501", "T0 0 1501"
        char c;
        uint8_t fanIndex;
        int16_t tempMinDeciC, tempMaxDeciC;

        if(4 <= sscanf(line, "%c %hhu %hd %hd", &c, &fanIndex, &tempMinDeciC, &tempMaxDeciC))
            if(isFanDefined(fanIndex)) return configurationHook.setTemperatureAlert(fanIndex, tempMinDeciC, tempMaxDeciC);

        return false;
    }


    bool commandPrintTempAlert(char (&line)[BufferSize])
    {
        // examples: "t", "t0", "t 0"
        char c;
        uint8_t fanIndex;
        if(2 <= sscanf(line, "%c %hhu", &c, &fanIndex))
        {
            if(isFanDefined(fanIndex)) return printTempAlert(fanIndex);
            return false;
        }

        for(auto idx : definedFanIndices)
            printTempAlert(idx);

        return true;
    }


    bool commandSetTempSensorIndex(char (&line)[BufferSize])
    {
        // examples: "I0 1", "I 0 1"
        char c;
        uint8_t fanIndex, tempSensorIndex;
        if(3 <= sscanf(line, "%c %hhu %hhu", &c, &fanIndex, &tempSensorIndex))
            return configurationHook.updateFanTemperatureSensorIndex(fanIndex, tempSensorIndex);
        return false;
    }


    bool printTempSensorIndex(uint8_t fanIndex)
    {
        Serial.print("I ");
        Serial.print(fanIndex);
        Serial.print(" ");
        Serial.print(configurationHook.getFanInfo(fanIndex).fanTemperatureSpec->temperatureSensorIndex);
        Serial.println();
        return true;
    }


    bool commandPrintTempSensorIndex(char (&line)[BufferSize])
    {
        // examples: "i", "i0", "i 0"
        char c;
        uint8_t fanIndex;
        if(2 <= sscanf(line, "%c %hhu", &c, &fanIndex))
        {
            if(isFanDefined(fanIndex)) return printTempSensorIndex(fanIndex);
            return false;
        }

        for(auto idx : definedFanIndices)
            printTempSensorIndex(idx);
        return true;
    }


    bool printTempSensorAddress(uint8_t tempSensorIndex)
    {
        Serial.print("A ");
        Serial.print(tempSensorIndex);

        for(unsigned char c : configurationHook.getFanInfo(tempSensorIndex).temperatureSensorSpec->sensorAddress)
        {
            Serial.print(" ");
            if(c < 16) Serial.print("0");
            Serial.print(c, HEX);
        }

        Serial.println();
        return true;
    }


    bool commandSetTempSensorAddr(char (&line)[BufferSize])
    {
        // example: "A 0 00 00 00 00 00 00 00 00"
        char c;
        uint8_t tempSensorIndex;
        uint8_t address[8];
        if((2 + 8) <= sscanf(line, "%c %hhu %hhx %hhx %hhx %hhx %hhx %hhx %hhx %hhx", &c, &tempSensorIndex, &address[0],
                             &address[1], &address[2], &address[3], &address[4], &address[5], &address[6], &address[7]))
        {
            if(isTemperatureSensorDefined(tempSensorIndex))
                return configurationHook.updateTemperatureSensorAddress(tempSensorIndex, address);
        }
        return false;
    }


    bool commandPrintTempSensorAddr(char (&line)[BufferSize])
    {
        // examples: "a", "a0", "a 0"
        char c;
        uint8_t tempSensorIndex;
        if(2 <= sscanf(line, "%c %hhu", &c, &tempSensorIndex))
        {
            if(isTemperatureSensorDefined(tempSensorIndex)) return printTempSensorAddress(tempSensorIndex);
            return false;
        }
        for(const auto &idx : definedTemperatureSensorIndices)
            printTempSensorAddress(idx);
        return true;
    }


    bool commandSaveSettings(char (&)[BufferSize])
    {
        auto result{ configurationHook.storeRamToFlash() };
        if(result == StoreStatus::Stored)
        {
            Serial.println(F("settings saved from RAM to flash"));
            return true;
        }
        else
        {
            Serial.print(F("failed to save settings from RAM to flash: "));
            Serial.println(storeStatusToStr(result));
            return false;
        }
    }


    bool commandLoadSettings(char (&)[BufferSize])
    {
        auto result{ configurationHook.restoreRamFromFlash() };
        if(result == LoadStatus::Loaded)
        {
            Serial.println(F("settings loaded from flash to RAM"));
            return true;
        }
        else
        {
            Serial.print(F("failed to load settings from flash to RAM: "));
            Serial.println(loadStatusToStr(result));
            return false;
        }
    }


    bool commandPrintSettings(char (&)[BufferSize])
    {
        configurationHook.reportSettings();
        return true;
    }


    bool commandResetSettings(char (&)[BufferSize])
    {
        configurationHook.resetRamSettings();
        Serial.println(F("effective settings in RAM restored to defaults"));
        return true;
    }


    bool commandReboot(char (&)[BufferSize])
    {
        ESP.restart();
        return true;
    }


    char (&buffer)[BufferSize];
    ConfigurationHook &configurationHook;

    std::unordered_map<uint8_t, CommandCallback> commandMap;
    std::vector<Command> commandList;
};
