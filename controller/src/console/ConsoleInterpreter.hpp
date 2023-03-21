#pragma once

#include "controller/FansController.h"
#include "fan/FansPwm.h"
#include "fan/FansTacho.h"
#include "temp/TempSensors.h"
#include <Arduino.h>
#include <string>


template <uint8_t BufferSize = 128> struct ConsoleInterpreter
{
    typedef bool (ConsoleInterpreter<BufferSize>::*CommandCallback)(char (&)[BufferSize]);


    struct Command
    {
        CommandCallback process;
        std::string help;
    };


    ConsoleInterpreter(char (&buffer)[BufferSize], TempSensors &sensors, FansPwm &pwms, FansTacho &tachos, FansController &controller, unsigned long &autoreportDelayS) :
    buffer(buffer), sensors(sensors), pwms(pwms), tachos(tachos), controller(controller), autoreportDelayS(autoreportDelayS)
    {
        commands['f'] = { .process = &ConsoleInterpreter<BufferSize>::commandPrintFans, .help = "print fan(s): f [uint]?" };
        commands['h'] = { .process = &ConsoleInterpreter<BufferSize>::commandPrintHelp, .help = "print help: h" };
        commands['A'] = { .process = &ConsoleInterpreter<BufferSize>::commandSetAutoreportSeconds,
                          .help = "set auto report seconds (0 to disable): A [uint]" };
        commands['a'] = { .process = &ConsoleInterpreter<BufferSize>::commandPrintAutoreportSeconds, .help = "get auto report seconds: A" };
    }


    void process()
    {
        auto callbackIter{ commands.find(buffer[0]) };
        if(callbackIter == commands.end())
        {
            Serial.print("unknown command: ");
            Serial.println(buffer);
        }
        else { (this->*(callbackIter->second.process))(buffer); }
    }


protected:
    void printFan(uint8_t fanIndex)
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
        Serial.println(info.trend);
    }


    bool commandPrintFans(char (&line)[BufferSize])
    {
        // examples: "f", "f 0", "f 4"
        char c;
        uint8_t fanIndex;

        if(2 <= sscanf(line, "%c %hhu", &c, &fanIndex))
        {
#if defined(FAN0)
            if(fanIndex == 0) printFan(FAN0_INDEX);
#endif
#if defined(FAN1)
            if(fanIndex == 1) printFan(FAN1_INDEX);
#endif
#if defined(FAN2)
            if(fanIndex == 2) printFan(FAN2_INDEX);
#endif
#if defined(FAN3)
            if(fanIndex == 3) printFan(FAN3_INDEX);
#endif
#if defined(FAN4)
            if(fanIndex == 4) printFan(FAN4_INDEX);
#endif
            return true;
        }
        else if(1 <= sscanf(line, "%c ", &c))
        {
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
            return true;
        }
        return false;
    }


    bool commandPrintHelp(char (&line)[BufferSize])
    {
        Serial.println("Help:");
        for(const auto c : commands)
        {
            Serial.print(static_cast<char>(c.first));
            Serial.print("  ");
            Serial.println(c.second.help.c_str());
        }
        return true;
    }

    bool commandSetAutoreportSeconds(char (&line)[BufferSize])
    {
        char c;
        unsigned long autoreportSeconds;
        if(2 <= sscanf(line, "%c %lu", &c, &autoreportSeconds))
        {
            autoreportDelayS = autoreportSeconds;
            return true;
        }
        return false;
    }


    bool commandPrintAutoreportSeconds(char (&line)[BufferSize])
    {
        Serial.println(autoreportDelayS);
        return true;
    }

    char (&buffer)[BufferSize];
    TempSensors &sensors;
    FansPwm &pwms;
    FansTacho &tachos;
    FansController &controller;
    unsigned long &autoreportDelayS;
    std::unordered_map<uint8_t, Command> commands;
};
