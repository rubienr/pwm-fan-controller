#include "FlashSettings.h"
#include "../../lib/crc/crc.h"
#include "LittleFS.h"

FlashSettings::FlashSettings(const char *filePathName, const Version &version, const Settings &defaults) :
filename(filePathName), version(version), defaults(defaults)
{
    container.data.settings = defaults;
}


LoadStatus FlashSettings::readSettingsFromFilesystem(Container &c) const
{
    File file{ LittleFS.open(filename, "r") };
    if(!file)
    {
        file.close();
        return LoadStatus::OpenError;
    }

    const size_t bytesCount{ file.read(reinterpret_cast<uint8_t *>(&c), sizeof(Container)) };
    file.close();
    if(bytesCount != sizeof(Container)) return LoadStatus::ReadError;

    return LoadStatus::Loaded;
}


StoreStatus FlashSettings::writeSettingsToFilesystem(Container &c)
{
    StoreStatus storeStatus;
    File file{ LittleFS.open(filename, "w", true) };
    if(!file)
    {
        file.close();
        storeStatus = StoreStatus::OpenError;
    }
    else
    {
        c.data.configWrites++;
        c.crc = computeCrc(c.data);
        const size_t bytesCount{ file.write(reinterpret_cast<const uint8_t *>(&c), sizeof(Container)) };
        file.close();
        if(bytesCount != sizeof(Container)) { storeStatus = StoreStatus::WriteError; }
        else
        {
            Serial.print(millis());
            Serial.println(F(" # container stored container"));
            reportContainer();
            return StoreStatus::Stored;
        }
    }
    Serial.print(millis());
    Serial.print(F(" # failed to load container: "));
    storeStatusToStr(storeStatus);

    return storeStatus;
}


LoadStatus FlashSettings::loadSettings()
{
    Container loadedContainer;
    auto loadStatus{ readSettingsFromFilesystem(loadedContainer) };

    if(LoadStatus::Loaded == loadStatus)
    {
        if(loadedContainer.crc != computeCrc(loadedContainer.data)) loadStatus = LoadStatus::CrcError;
        else if(loadedContainer.data.version.versionNumber.major != version.versionNumber.major ||
                loadedContainer.data.version.versionNumber.minor != version.versionNumber.minor)
            loadStatus = LoadStatus::VersionMismatch;
        else
        {
            container = loadedContainer;
            Serial.print(millis());
            Serial.println(F(" # container loaded"));
            reportContainer();
            return LoadStatus::Loaded;
        }
    }
    Serial.print(millis());
    Serial.print(F(" # failed to load container: "));
    Serial.println(loadStatusToStr(loadStatus));

    Serial.print(millis());
    Serial.println(F(" # use default settings"));

    resetSettings();
    reportContainer();

    return loadStatus;
}


StoreStatus FlashSettings::storeSettings() { return writeSettingsToFilesystem(container); }


void FlashSettings::resetSettings()
{
    container.data.settings = defaults;
    container.data.version = version;
}


Settings &FlashSettings::getSettings() { return container.data.settings; }


uint32_t FlashSettings::computeCrc(const Data &data)
{
    return ::computeCrc(reinterpret_cast<const uint8_t *>(&data), sizeof(Data));
}


void FlashSettings::reportContainer() const { ::reportContainer(container); }
