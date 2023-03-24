#pragma once
#include "types.h"


struct FlashSettings
{
    explicit FlashSettings(const char *filePathName, const Version &version, const Settings &defaults);
    LoadStatus loadSettings();
    StoreStatus storeSettings();
    void resetSettings();

    [[nodiscard]] Settings &getSettings();
    void reportContainer() const;

protected:
    static uint32_t computeCrc(const Data &data);
    LoadStatus readSettingsFromFilesystem(Container &settings) const;
    StoreStatus writeSettingsToFilesystem(Container &settings);

    const char *filename;
    const Version &version;
    const Settings &defaults;
    Container container;
};
