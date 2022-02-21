#include <Arduino.h>

class DataStorage{
    public:
    DataStorage();
    void writeRGB(int r, int g, int b);
    void writeFixedRandom(bool status);
    void writeSoftRandom(bool status);
    void writeRandomSpeed(int value);
    int getR();
    int getG();
    int getB();
    int getRandomSpeed();
    bool getFixedRandom();
    bool getSoftRandom();
};