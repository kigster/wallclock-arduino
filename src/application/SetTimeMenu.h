/*
 * SetTimeMenu.h
 *
 *  Created on: Dec 19, 2014
 *      Author: Konstantin Gredeskoul
 *        Code: https://github.com/kigster
 *
 *  (c) 2014 All rights reserved, MIT License.
 */

#include "Configuration.h"
#ifdef ENABLE_MENU

#ifndef SETTIMEMENU_H_
#define SETTIMEMENU_H_

namespace WallClock {
    class App;

    class SetTimeMenu {
    public:
        SetTimeMenu();
        SetTimeMenu(WallClock::App *app);
        void setApp(WallClock::App *app);
        void configureTime() ;
        void nextMode();
    private:

        signed short h, m;
        App *app;
        char *what;
        void instructions();
        void updateTimeCallback();
        void selectNumber(signed short *current, int min, int max);

    };
}
#endif /* SETTIMEMENU_H_ */
#endif /* ENABLE_MENU */
