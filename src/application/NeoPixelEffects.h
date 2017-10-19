/*
 * NeoPixelEffects.h
 *
 *  Created on: Nov 28, 2014
 *      Author: Konstantin Gredeskoul
 *        Code: https://github.com/kigster
 *
 *  (c) 2014 All rights reserved, MIT License.
 */
#include "Configuration.h"
#ifdef ENABLE_NEOPIXELS

#ifndef NEOPIXELEFFECTS_H_
#define NEOPIXELEFFECTS_H_

#include <Adafruit_NeoPixel.h>

#define MAX_EFFECTS 20

class NeoPixelEffects {
public:
    NeoPixelEffects();
    ~NeoPixelEffects();
    void setStrip(Adafruit_NeoPixel *strip);
    int effectsEnabled();
    int currentEffectIndex();
    void reset();

    // effects
    void colorRed(),
        colorBlue(),
        colorGreen(),
        colorYellow(),
        randomColor(),
        rainbow(),
        rainbowCycle(),
        theaterChaseRainbow(),
        fadeCycle();

    void fadeOut(long millis);
    // helpers
    void colorWipe(uint32_t c),
        theaterChase(uint32_t c),
        chooseNewEffect(),
        refreshCurrentEffect();


    uint32_t Wheel(byte WheelPos);

private:
    Adafruit_NeoPixel *_strip;
    void (NeoPixelEffects::*_enabledEffects[MAX_EFFECTS])();
    void (NeoPixelEffects::*_currentEffect)();
    int _numEffectsEnabled;
    int _currentEffectIndex;
    uint16_t i, j;
    bool flag;

};

inline NeoPixelEffects::~NeoPixelEffects() {};


#endif /* NEOPIXELEFFECTS_H_ */
#endif
