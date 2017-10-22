/*
 * BedTimeApp.cpp
 *
 *  Created on: Dec 19, 2014
 *      Author: Konstantin Gredeskoul
 *        Code: https://github.com/kigster
 *
 *  (c) 2014 All rights reserved, MIT License.
 */

#include <Arduino.h>
#include "Configuration.h"
#include "App.h"

namespace WallClock {
    App::App(PinoutMapping &_pinout,
             State &_state,
             RotaryEncoderWithButton &_rotary,
             Adafruit_7segment &_matrix,
             OneButton &_button) :

            pinout(_pinout),
            state(_state),
            rotary(_rotary),
            matrix(_matrix),
            button(_button) {

        screenOn = colonOn = true;
        mode = SetTime::Default;

        photoOffsetPercentage = lastPhotoValue = currentPhotoValue = lastDebugLogAt = lastPotentiometerRead = 0;

#ifdef ENABLE_MENU
        menu.setApp(this);
#endif
    }


    void App::setup() {
        matrix.begin(0x70);
        delay(100);
        matrix.clear();
        brightnessChangedEvent();
#ifdef ENABLE_PHOTORESISTOR
        pinMode(pinout.pinPhotoResistor, INPUT);
#endif
#ifdef ENABLE_ANALOG_POTENTIOMETER
        pinMode(pinout.pinAnalogPot, INPUT);
#endif
    }

    void App::run() {
        if (    processKnobEvents() ||
                processPotentiometer() ||
                #ifdef ENABLE_DHT22
                processDHTChange() ||
                #endif
                processPhotoresistorChange()
            ) {

            debugLog();
        }
    }

    bool App::processKnobEvents() {
        signed long delta = rotary.rotaryDelta();
        if (delta != 0) {
            // normalize knob to increments of 1
            delta = delta / abs(delta);
            if (state.getDisplayBrightness().changeBy(delta)) {
                lastKnobTouched = millis();
                brightnessChangedEvent();
                state.getPhotoresistorReading().syncTo(
                        &state.getDisplayBrightness());
                return true;
            }
        }
        return false;
    }

    bool App::processPotentiometer() {
#ifdef ENABLE_ANALOG_POTENTIOMETER
        if (millis() - lastPotentiometerRead < 100) {
            int val = analogRead(pinout.pinAnalogPot);
            lastPotValue = map(val, 0, 1023, -100, 100);
            lastPotentiometerRead = millis();
            return true;
        }
#endif
        return false;
    }

    bool App::processDHTChange() {
#ifdef ENABLE_DHT22
        if (millis() - dhtData.lastReadingAt < 3000) {
            readDHT();
            return true;
        }
#endif
        return false;
    }

    void App::debugLog() {
        #ifdef DEBUG
        if (millis() - lastDebugLogAt > 1000) {
            #ifdef ENABLE_PHOTORESISTOR
            Serial.print("photoresistor readout [0-1023]: ");
            Serial.println(currentPhotoValue);
            #endif

            #ifdef ENABLE_DHT22
            Serial.print("temperature readout ");
            dhtData.isFahrenheit ?
                Serial.print(F("(F'): ")) :
                Serial.print(F("(C'): "))
            ;

            Serial.println(dhtData.temperature);
            #endif

            #ifdef ENABLE_ANALOG_POTENTIOMETER
            Serial.print(F("potentiometer readout [-100..100]: "));
            Serial.println(lastPotValue);
            #endif

            lastDebugLogAt = millis();
        }
        #endif

    }
    bool App::processPhotoresistorChange() {
        GaugedValue &photo = state.getPhotoresistorReading();
        GaugedValue &display = state.getDisplayBrightness();

        currentPhotoValue = analogRead(pinout.pinPhotoResistor);

        if (millis() - lastKnobTouched > 2000 &&
            currentPhotoValue != lastPhotoValue) {
            if (photo.setCurrent(currentPhotoValue)) {
                if (display.follow(&photo)) {
                    brightnessChangedEvent();
                    return true;
                }
            }
        }


        return false;
    }

    void App::brightnessChangedEvent() {
        matrix.setBrightness(state.getDisplayBrightness().getCurrent());
    }

    void App::debug(const char *message) {
        Serial.println(message);
    }

    void App::debug(int row, const char *message, bool clear) {
        Serial.println(message);
    }

    void App::displayCurrentTime() {
        tmElements_t tm;
        short h, m;

#ifdef TEENSYDUINO
        breakTime(now(), tm);
#else
        if (!RTC.read(tm)) {
            if (RTC.chipPresent()) {
                debug(1, "Time chip detected, but not set. Resetting", true);
#ifdef ENABLE_SET_TIME
                helper.setDateToCompileTime();
#endif
            } else {
                matrix.printError();
                debug(1, "Time chip not detected", true);
                colonOn = !colonOn;
            }
            return;
        }
#endif

        h = tm.Hour % 12;
        if (h == 0) { h = 12; }
        m = tm.Minute;
        if (screenOn) displayTime(h, m);
        Serial.print(F("> "));
        sprintf(buffer, "%2d:%02d:%02d %d/%02d/%d, Brightness [%d], Photo [%d]",
                h, m, tm.Second, tm.Month, tm.Day, 1970 + tm.Year,
                state.getDisplayBrightness().getCurrent(),
                state.getPhotoresistorReading().getCurrent());
        debug(2, buffer, true);
    }

/**
 * We receive negative hours or minutes when the other
 * element is being setup / modified. A bit of nasty overloading, but hey. Whatever.
 */
    void App::displayTime(signed short h, signed short m) {
        if (!screenOn && h >= 0 && m >= 0) return;
        matrix.clear();
        colonOn = !colonOn;
        if (h < 0 || m < 0) colonOn = false;
        if (h > 0) {
            if (h >= 10) {
                matrix.writeDigitNum(0, h / 10, false);
            }
            matrix.writeDigitNum(1, h % 10, false);
        }
        matrix.drawColon(colonOn);
        if (m >= 0) {
            matrix.writeDigitNum(3, m / 10, false);
            matrix.writeDigitNum(4, m % 10, false);
        }
        matrix.writeDisplay();
    }

    void App::cb_ButtonClick() {
        Serial.print(F("Entering BedTimeApp::cb_ButtonClick, mode = "));
        Serial.println((int) mode);
        if (mode != SetTime::Default) {
#ifdef ENABLE_MENU
            menu.nextMode();
#endif
        }
    }

    void App::cb_ButtonDoubleClick() {
        if (mode != SetTime::Default) {
            mode = SetTime::Default;
            debug(0, "Cancel Setup", true);
        } else {
            toggleDisplay();
        }
    }

    void App::cb_ButtonHold() {
        Serial.print(F("Entering BedTimeApp::cb_ButtonHold, mode = "));
        Serial.println((int) mode);
        if (mode == SetTime::Default) {
            Serial.println(F("Mode is Default -> calling configureTime()"));
#ifdef ENABLE_MENU
            menu.configureTime();
#endif
        } else {
            Serial.println(F("Mode is not Default, Hold is ignored."));
        }
    }

#ifdef ENABLE_DHT22
    void App::setDHT22(DHT *_dht) {
        dht = _dht;
    }

    void App::readDHT() {

        // Reading temperature or humidity takes about 250 milliseconds!
        dhtData.humidity = dht->readHumidity();
        dhtData.temperature = dht->readTemperature(dhtData.isFahrenheit);

        // Check if any reads failed and exit early (to try again).
        if (isnan(dhtData.humidity) || isnan(dhtData.temperature)) {
            Serial.println(F("Failed to read from DHT sensor!"));
            dhtData.humidity = dhtData.temperature = 0;
        }

        dhtData.lastReadingAt = millis();
    }

    void App::changeTemperatureUnits() {
        dhtData.isFahrenheit = !dhtData.isFahrenheit;
    }

#endif

    void App::toggleDisplay() {
        screenOn = !screenOn;
        matrix.clear();
        matrix.writeDisplay();
        if (screenOn) {
            displayCurrentTime();
        }
    }


    OneButton *App::getButton() {
        return &button;
    }

    RotaryEncoderWithButton *App::getRotary() {
        return &rotary;
    }


}
