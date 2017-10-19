/*
 * BedTimeApp.h
 *
 *  Created on: Dec 19, 2014
 *      Author: Konstantin Gredeskoul
 *        Code: https://github.com/kigster
 *
 *  (c) 2014 All rights reserved, MIT License.
 */

#ifndef BEDTIMEAPP_H_
#define BEDTIMEAPP_H_

#include "Wallock.h"
#include "State.h"

#ifdef ENABLE_LCD
#include <LiquidCrystal_I2C.h>
#endif

#ifdef ENABLE_SET_TIME
#include "SetTimeHelper.h"
#include "SetTimeMenu.h"
#endif

#ifdef ENABLE_NEOPIXELS
#include "NeoPixelManager.h"
#include "NeoPixelEffects.h"
#endif

namespace SetTime {
    typedef enum TimeChangeMode_e {
        Default = (1 << 0), Hour = (1 << 1), Minute = (1 << 2), Save = (1 << 4), Last = (1 << 5),
    } TimeMode;
};

namespace Wallock {

    typedef struct PinoutMapping_s {
         uint8_t pinPhotoResistor;
         uint8_t pinRotaryLeft;
         uint8_t pinRotaryRight;
         uint8_t pinRotaryButton;
         uint8_t pinNeoPixels;
         uint8_t numNeoPixels;
     } PinoutMapping;


    class App {
        private:
            PinoutMapping                               &pinout;
            State                                       &state;
            RotaryEncoderWithButton                     &rotary;
            Adafruit_7segment                           &matrix;
            OneButton                                   &button;

            uint32_t                                    lastDisplayedTime, lastKnobTouched;

            bool                                        colonOn, screenOn, neoPixelsOn;
            int                                         lastPhotoValue, currentPhotoValue;
            float                                       photoOffsetPercentage;

        public:
            App(            PinoutMapping               &_pinout,
                            State                       &_state,
                            RotaryEncoderWithButton     &_rotary,
                            Adafruit_7segment           &_matrix,
                            OneButton                   &_button);

            SetTime::TimeMode mode;

            #ifdef ENABLE_LCD
                LiquidCrystal_I2C *lcd;  // set the LCD address to 0x27 for a 16 chars and 2 line display
            #endif
            #ifdef ENABLE_MENU
                SetTimeMenu menu;
            #endif
            #ifdef ENABLE_SET_TIME
                SetTimeHelper helper;
            #endif
            #ifdef ENABLE_NEOPIXELS
                NeoPixelManager *neoPixelManager;
            #endif


            void setup();
            void run();

            void refreshUI();
            void readEnvironment();
            void brightnessChangedEvent();

            void displayTime(signed short h, signed short m);
            void displayCurrentTime();

            void toggleDisplay();
            void toggleNeoPixels();
            void neoPixelRefresh();
            void neoPixelNextEffect();

            void cb_ButtonClick();
            void cb_ButtonDoubleClick();
            void cb_ButtonHold();

            void debug(int row, const char *message, bool clear);
            void debug(const char *message);

            bool processPhotoresistorChange();
            bool processKnobEvents();

            OneButton* getButton();
            RotaryEncoderWithButton *getRotary();

    };
}
#endif /* BEDTIMEAPP_H_ */
