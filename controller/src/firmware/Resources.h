#pragma once
#include "../lib/version/version.h"
#include "configuration.h"
#include "console/ConsoleInterpreter.hpp"
#include "controller/FansController.h"
#include "fan/FansPwm.h"
#include "fan/FansTacho.h"
#include "fan/FansTemperature.h"
#include "sensors/TempSensors.h"
#include "settings/FlashSettings.h"
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <Wire.h>
#include <elapsedMillis.h>


struct Resources
{
    static const Version constexpr version{};

    struct
    { // version and settings storage
        FlashSettings storage{ FLASH_SETTINGS_FILE_PATH_NAME, version };
    } settings;

    struct
    {
        Adafruit_SSD1306 screen{ SCREEN_WIDTH_PX, SCREEN_HEIGHT_PX, &Wire, -1 };
        const char processTicker[2]{ '_', '-' };
        uint8_t processTickerIndex{ 0 };
    } display;

    struct
    {
        FansTacho tachos;
        FansPwm pwms;
    } fan;

    struct
    {
        OneWire wire{ TEMPERATURE_SENSORS_ONE_WIRE_GPIO_NUM };
        DallasTemperature dsSensors{ &wire };
        TempSensors sensors{ dsSensors };
        FansTemperature fansTemperature{ sensors };
    } temp;

    FansController controller{ temp.sensors, temp.fansTemperature, fan.pwms, fan.tachos };

    struct
    {
        elapsedMillis oneSecondProcessTriggerCounterMs{ 0 };
#if defined(SERIAL_AUTOREPORT)
        elapsedSeconds reportOnSerialCounterS{ 0 };
        unsigned long autoreportSeconds{ SERIAL_AUTOREPORT_EVERY_SECONDS };
#else
        unsigned long autoreportSeconds{ 0 };
#endif
    } timers;

    ConfigurationHook configChangedHook{ controller, settings.storage, timers.autoreportSeconds };

    struct
    {
        char buffer[128]{ 0 };
        uint8_t index : 7; // 0 - 127
        bool lineAvailable{ false };
    } console;
    ConsoleInterpreter<128> interpreter{ console.buffer, configChangedHook };

    Resources() { console.index = 0; }
};
