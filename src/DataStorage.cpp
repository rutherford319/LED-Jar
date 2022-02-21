#include <EEPROM_Rotate.h>

#define RED_DATA_LOCATION 1024
#define GREEN_DATA_LOCATION 1026
#define BLUE_DATA_LOCATION 1028
#define FIXED_RANDOM_BOOL_DATA_LOCATION 1030
#define SPEED_DATA_LOCATION 1032
#define SOFT_RANDOM_BOOL_DATA_LOCATION 1034

EEPROM_Rotate EEPROMr;
#pragma once

#ifndef DATASTORAGE
#define DATASTORAGE
#include "DataStorage.h"
#endif

DataStorage::DataStorage()
{
    Serial.println("Data initialized");
    EEPROMr.size(4);
    EEPROMr.begin(2048);
}

void writeIntegerToMemory(int startByteLocation, int valueToWrite, bool commit)
{
    EEPROMr.write(startByteLocation, valueToWrite >> 8);
    EEPROMr.write(startByteLocation + 1, valueToWrite & 0xFF);

    if (commit)
    {
        EEPROMr.commit();
    }
}

int readIntegerFromMemory(int startByteLocation)
{
    return (EEPROMr.read(startByteLocation) << 8) + EEPROMr.read(startByteLocation + 1);
}

void writeBooleanToMemory(int startByteLocation, int valueToWrite)
{
    EEPROMr.write(startByteLocation, valueToWrite);
    EEPROMr.commit();
}

int readBooleanFromMemory(int startByteLocation)
{
    return (EEPROMr.read(startByteLocation));
}

void DataStorage::writeRGB(int r, int g, int b)
{
    writeIntegerToMemory(RED_DATA_LOCATION, r, false);
    writeIntegerToMemory(GREEN_DATA_LOCATION, g, false);
    writeIntegerToMemory(BLUE_DATA_LOCATION, b, false);

    EEPROMr.commit();
}

void DataStorage::writeRandomSpeed(int value)
{
    writeIntegerToMemory(SPEED_DATA_LOCATION, value, true);
}

int DataStorage::getRandomSpeed()
{
    return readIntegerFromMemory(SPEED_DATA_LOCATION);
}

int DataStorage::getR()
{
    return readIntegerFromMemory(RED_DATA_LOCATION);
}

int DataStorage::getG()
{
    return readIntegerFromMemory(GREEN_DATA_LOCATION);
}

int DataStorage::getB()
{
    return readIntegerFromMemory(BLUE_DATA_LOCATION);
}

void DataStorage::writeFixedRandom(bool status)
{
    writeBooleanToMemory(FIXED_RANDOM_BOOL_DATA_LOCATION, status);
}

void DataStorage::writeSoftRandom(bool status)
{
    writeBooleanToMemory(SOFT_RANDOM_BOOL_DATA_LOCATION, status);
}

bool DataStorage::getFixedRandom()
{
    return readBooleanFromMemory(FIXED_RANDOM_BOOL_DATA_LOCATION);
}

bool DataStorage::getSoftRandom()
{
    return readBooleanFromMemory(SOFT_RANDOM_BOOL_DATA_LOCATION);
}
