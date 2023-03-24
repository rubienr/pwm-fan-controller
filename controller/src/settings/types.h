#pragma once
#include "../../lib/version/version.h"


enum class LoadStatus : uint8_t
{
    Loaded,
    OpenError,
    ReadError,
    CrcError,
    VersionMismatch,
};

const char *loadStatusToStr(LoadStatus t);


enum class StoreStatus : uint8_t
{
    Stored,
    OpenError,
    WriteError,
};

const char *storeStatusToStr(StoreStatus t);


struct Settings
{
    bool foo{ false };
} __attribute__((packed));


void reportSettings(const Settings &s);


struct Data
{ // effective data to store
    uint32_t configWrites{ 0 };
    Version version{};
    Settings settings{};
} __attribute__((packed));


void reportVersion(const Version &v);
;


void reportData(const Data &d);


struct Container
{ // package sored/loaded to/from filesystem
    uint32_t crc{ 0 };
    Data data{};
} __attribute__((packed));


void reportContainer(const Container &c);
