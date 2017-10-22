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

#include "Configuration.h"
#include "State.h"

#ifdef ENABLE_SET_TIME
#include "SetTimeHelper.h"
#include "SetTimeMenu.h"
#endif


namespace SetTime {
    typedef enum TimeChangeMode_e {
        Default = (1 << 0), Hour = (1 << 1), Minute = (1 << 2), Save = (1 << 4), Last = (1 << 5),
    } TimeMode;
};

namespace WallClock {
    typedef struct PinoutMapping_s {
         uint8_t pinPhotoResistor;
         uint8_t pinRotaryLeft;
         uint8_t pinRotaryRight;
         uint8_t pinRotaryButton;
#ifdef ENABLE_NEOPIXELS
         uint8_t pinNeoPixels;
         uint8_t numNeoPixels;
#endif
#ifdef ENABLE_ANALOG_POTENTIOMETER
         uint8_t pinAnalogPot;
#endif
#ifdef ENABLE_DHT22
         uint8_t pinDHT22;
#endif
     } PinoutMapping;

#ifdef ENABLE_DHT22
    typedef struct DHT_SensorData_s {
         float humidity = 0;
         float temperature = 0;
         bool isFahrenheit = true;
         int lastReadingAt = 0;
    } DHT_SensorData;
#endif

    class App {
        private:
            PinoutMapping                               &pinout;
            State                                       &state;
            RotaryEncoderWithButton                     &rotary;
            Adafruit_7segment                           &matrix;
            OneButton                                   &button;

            uint32_t                                    lastDebugLogAt, lastKnobTouched, lastPotentiometerRead;

            bool                                        colonOn, screenOn, neoPixelsOn;
            int                                         lastPhotoValue, currentPhotoValue, lastPotValue;
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

            #ifdef ENABLE_DHT22
                DHT *dht;
                DHT_SensorData dhtData;
                void setDHT22(DHT *_dht);
                void readDHT();
                void changeTemperatureUnits();
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

            void cb_ButtonClick();
            void cb_ButtonDoubleClick();
            void cb_ButtonHold();

            void debug(int row, const char *message, bool clear);
            void debug(const char *message);
            void debugLog();


            bool processPhotoresistorChange();
            bool processKnobEvents();
            bool processDHTChange();
            bool processPotentiometer();


            OneButton* getButton();
            RotaryEncoderWithButton *getRotary();

    };
}
#endif /* BEDTIMEAPP_H_ */
