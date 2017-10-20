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

#include "application/Configuration.h"
#include "application/App.h"

char buffer[80];

WallClock::GaugedValue brightnessGauge("brightness", 0, 15, 1, true);
WallClock::GaugedValue photoGauge("photo-value", 0, 800, 25, false);

#ifdef ENABLE_DHT22
#define DHTPIN 5
#define DHTTYPE DHT22
#endif


// I2C uses A4 and A5

WallClock::PinoutMapping pinout = {
        A3,    // PhotoResistor

               // https://www.pjrc.com/teensy/td_libs_Encoder.html
        2,     // Left Rotary (interrupt enabled)
        3,     // Right Rotary (interrupt enabled)

        4,     // Rotary Button
#ifdef ENABLE_NEOPIXELS
        6,     // pin for NeoPixels
        2,     // Number of NeoPixels
#endif
#ifdef ENABLE_ANALOG_POTENTIOMETER
        A0,
#endif
#ifdef ENABLE_DHT22
        DHTPIN,
#endif
};

#ifdef ENABLE_DHT22
DHT dht(DHTPIN, DHTTYPE);
#endif

WallClock::State state(photoGauge, brightnessGauge);

OneButton button(pinout.pinRotaryButton, (uint8_t) pinout.pinRotaryButton);

RotaryEncoderWithButton rotary(
        (uint8_t) pinout.pinRotaryLeft,
        (uint8_t) pinout.pinRotaryRight,
        (uint8_t) pinout.pinRotaryButton);

Adafruit_7segment matrix;

WallClock::App app(pinout, state, rotary, matrix, button);


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
    Serial.println(F("[wallock] v3 (C) 2017 https://reinvent.one"));
    Serial.println(F("app->setup()"));

    app.setup();

    #ifdef ENABLE_DHT22
    dht.begin();
    app.setDHT22(&dht);
    #endif

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
