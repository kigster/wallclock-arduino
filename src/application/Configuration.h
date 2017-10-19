/*
 * Wallock.h – feature flags
 *
 *  Created on: Dec 19, 2014
 *      Author: Konstantin Gredeskoul
 *        Code: https://github.com/kigster
 *
 *  (c) 2014 All rights reserved, MIT License.
 */

#ifndef _WALLOCK_H_
#define _WALLOCK_H_

//#define ENABLE_LCD

#define ENABLE_PHOTORESISTOR
#define ENABLE_SET_TIME
#define ENABLE_MENU
#define ENABLE_NEOPIXELS
#define ENABLE_ANALOG_POTENTIOMETER

#define DEBUG 1

#include "Arduino.h"
#include "Encoder.h"
#include "RotaryEncoderWithButton.h"
#include <Wire.h>
#include <time.h>
#include <DS1307RTC.h>
#include <Adafruit_LEDBackpack.h>
#include <Adafruit_GFX.h>
#include <OneButton.h>
#include <SimpleTimer.h>

//extern "C" {
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
//}
extern char buffer[];

#endif
