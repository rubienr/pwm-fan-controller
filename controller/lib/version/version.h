#pragma once

#include "configuration.h"

struct VersionNumber
{
    uint8_t major{ VERSION_MAJOR };
    uint8_t minor{ VERSION_MINOR };
    uint8_t patch{ VERSION_PATCH };
} __attribute__((packed));

struct Version
{
    VersionNumber versionNumber{};
    char buildTimestamp[25]{ __TIMESTAMP__ };
} __attribute__((packed));
