#include <Arduino.h>

#ifndef DATASTORAGE
#define DATASTORAGE
#include "DataStorage.h"
#endif

class LedControl{
    private:
    DataStorage *storage;
    public:
    LedControl(int rPin, int gPin, int bPin, DataStorage *st);
    void setRGB(int r, int g, int b, bool shouldSave);
    void setHex(String hex);
    void iterateGradualRandom(int randomSpeed);
    void iterateFixedRandom(int randomSpeed);
};