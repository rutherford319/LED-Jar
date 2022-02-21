#include "LedControl.h"
#include <Arduino.h>

#pragma once

int r;
int g;
int b;

int rValue = 0, gValue = 0, bValue = 0;
int goalR = 0, goalG = 0, goalB = 0;


LedControl::LedControl(int rPin, int gPin, int bPin, DataStorage *st)
{
    r = rPin;
    g = gPin;
    b = bPin;

    pinMode(r, OUTPUT);
    pinMode(g, OUTPUT);
    pinMode(b, OUTPUT);
    storage = st;
}

void LedControl::setRGB(int rVal, int gVal, int bVal, bool shouldSave)
{
    rValue = rVal;
    gValue = gVal;
    bValue = bVal;
    analogWrite(r, constrain(rValue, 0, 255));
    analogWrite(g, constrain(gValue, 0, 255));
    analogWrite(b, constrain(bValue, 0, 255));

    if (shouldSave)
    {
        storage->writeRGB(rValue, gValue, bValue);
    }
}

char *getCharArrayFromStringTemp(String str)
{
    char *chr = (char *)malloc(str.length() + 1);
    str.toCharArray(chr, str.length() + 1);
    return chr;
}

void LedControl::setHex(String hex)
{
    int hexR, hexG, hexB;
    sscanf(getCharArrayFromStringTemp(hex), "#%02x%02x%02x", &hexR, &hexG, &hexB);
    //PWM Correction
    hexR = hexR * 4;
    hexG = hexG * 4;
    hexB = hexB * 4;

    hexR = 1024 - hexR;
    hexG = 1024 - hexG;
    hexB = 1024 - hexB;

    setRGB(hexR, hexG, hexB, true);
}

void LedControl::iterateFixedRandom(int randomSpeed)
{
    setRGB(random(256), random(256), random(256), false);
    delay(randomSpeed);
}

int calculateAdjustments(int goal, int current)
{
    if (goal > current)
    {
        return -1;
    }

    if (goal < current)
    {
        return 1;
    }
    return 0;
}

void LedControl::iterateGradualRandom(int randomSpeed)
{
    if (((goalR == 0) && (goalG == 0) && (goalB == 0)) || ((goalR == rValue) && (goalG == gValue) && (goalB == bValue)))
    {
        // Set new goal
        goalR = random(256);
        goalG = random(256);
        goalB = random(256);
    }

    int adjustmentR = calculateAdjustments(goalR, rValue);
    int adjustmentG = calculateAdjustments(goalG, gValue);
    int adjustmentB = calculateAdjustments(goalB, bValue);

    int newR = rValue - adjustmentR;
    int newG = gValue - adjustmentG;
    int newB = bValue - adjustmentB;

    setRGB(newR, newG, newB, false);
    delay(randomSpeed);
}