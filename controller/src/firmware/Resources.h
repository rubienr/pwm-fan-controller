#pragma once
#include "../lib/version/version.h"
#include "configuration.h"
#include "console/ConsoleInterpreter.hpp"
#include "controller/FansController.h"
#include "fan/FansPwm.h"
#include "fan/FansTacho.h"
#include "settings/FlashSettings.h"
#include "settings/defaults.h"
#include "temp/TempSensors.h"
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <Wire.h>
#include <elapsedMillis.h>

struct Resources
{
    struct
    { // version and settings storage
        const Version version{};
        FlashSettings storage{ FLASH_SETTINGS_FILE_PATH_NAME, version, defaultSettings() };
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
        OneWire wire{ TEMP_ONE_WIRE_GPIO_NUM };
        DallasTemperature dsSensors{ &wire };
        TempSensors sensors{ dsSensors };
    } temp;

    FansController controller{ temp.sensors, fan.pwms, fan.tachos };

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

    struct
    {
        char buffer[128]{ 0 };
        uint8_t index : 7; // 0 - 127
        bool lineAvailable{ false };
    } console;
    ConsoleInterpreter<128> interpreter{ console.buffer, controller, temp.sensors, timers.autoreportSeconds, settings.storage };

    Resources() { console.index = 0; }
};
