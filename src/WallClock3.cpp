/*
 * BedTime
 *
 * 7-Segment Display for current time, based on the RTC chip.
 *
 * Rotary encoder is used to change s, and set time. The button
 * on rotary encoder is used in single click, double click and hold capacity
 * for various options. See README for more info.
 *
 * Press and hold to enter configuration menu, then click to advance
 * to the next.
 *
 * (c) 2014 All rights reserved.  Please see LICENSE.
 *
 * Author: Konstantin Gredeskoul <kigster@gmail.com>
 */

#include "app_lib/Wallock.h"
#include "app_lib/App.h"

char buffer[80];

Wallock::GaugedValue brightnessGauge("brightness", 0, 15, 1, true);
Wallock::GaugedValue photoGauge("photo-value", 0, 800, 25, false);

Wallock::PinoutMapping pinout = {
        A3,     // PhotoResistor
        2,     // Left Rotary
        3,     // Right Rotary
        4,     // Rotary Button
        2      // Number of NeoPixels
};

Wallock::State state(photoGauge, brightnessGauge);

OneButton button(pinout.pinRotaryButton, 1);

RotaryEncoderWithButton rotary(
        (uint8_t) pinout.pinRotaryLeft,
        (uint8_t) pinout.pinRotaryRight,
        (uint8_t) pinout.pinRotaryButton);

Adafruit_7segment matrix;

Wallock::App app(pinout, state, rotary, matrix, button);

SimpleTimer timer(1);

#ifdef TEENSYDUINO
time_t getTeensy3Time() {
    return Teensy3Clock.get();
}
#endif

void rotaryButtonClick() {
    Serial.println(F("rotaryButtonClick()"));
    app.cb_ButtonClick();
    delay(100);
}
void rotaryButtonDoubleClick() {
    Serial.println(F("rotaryButtonDoubleClick()"));
    app.cb_ButtonDoubleClick();
    delay(100);
}
void rotaryButtonLongPress() {
    Serial.println(F("rotaryButtonLongPress()"));
    app.cb_ButtonHold();
    delay(100);
}
void displayTimeNow() {
    app.displayCurrentTime();
}
void readPhotoResistor() {
#ifdef ENABLE_PHOTORESISTOR
    app.processPhotoresistorChange();
#endif
}
void neoPixelRefresh() {
    app.neoPixelRefresh();
}
void neoPixelNextEffect() {
    app.neoPixelNextEffect();
}

bool detectRTC() {
    tmElements_t tm;
    bool rtcResult = RTC.read(tm);
    if (RTC.chipPresent()) {
        if (!rtcResult) {
            Serial.println(F("RTC.read() returned false, resetting to compile time"));
            app.helper.setDateToCompileTime();
        } else {
            sprintf(buffer, "RTC.read() successful, booting at %d/%d/%d %d:%d", tm.Month, tm.Day, tm.Year, tm.Hour, tm.Second);
            Serial.println(buffer);
            return true;
        }
    } else {
        Serial.println(F("RTC chip was not detected."));
    }
    return false;
}

void setup() {
    Serial.begin(9600);
#ifdef TEENSYDUINO
    setSyncProvider(getTeensy3Time);
#endif
    Serial.println(F("[wallock] v2.1(c) 2015 kiguino.moos.io"));
    Serial.println(F("app->setup()"));

    app.setup();

    Serial.println(F("setting up timers.."));

    app.getButton()->attachClick(rotaryButtonClick);
    app.getButton()->attachLongPressStart(rotaryButtonLongPress);
    app.getButton()->attachDoubleClick(rotaryButtonDoubleClick);

    timer.setInterval( 1000, (timer_callback) displayTimeNow);
    timer.setInterval( 2000, (timer_callback) readPhotoResistor);
    timer.setInterval( 5000, (timer_callback) neoPixelNextEffect);
    timer.setInterval(    5, (timer_callback) neoPixelRefresh);


#ifdef ENABLE_SET_TIME
    #ifdef TEENSYDUINO
    if (year() < 2014) {
        Serial.println("[BLOOPERS!]");
        Serial.prhint("Year returned is < 2014, resetting time to compile time, 12:00. Year: "); Serial.println(year());
        app.helper.setDateToCompileTime();
    } else {
        Serial.println("[OK]");
    }
#else
    while(!detectRTC()) {
        Serial.print(F("RTC chip not detected, can not proceed."));
        delay(1000);
    }
#endif /* TEENSYDUINO */
#endif /* ENABLE_SET_TIME */

#ifdef ENABLE_NEOPIXELS
    NeoPixelEffects *n = app.neoPixelManager->effects();
    n->reset();
    for (int i = 0; i < 3 * 255; i++) {
        n->fadeCycle();
        delay(2);
    }
    n->fadeOut(2000);
    app.neoPixelManager->shutoff();
    Serial.println("Setup complete, entering loop...");
#endif
}

void loop() {
    timer.run();
    app.run();
    delay(10);
}
