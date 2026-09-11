#include <Arduino.h>

#include "configuration_menu.h"

void handleKey(char key, TimingConfiguration& config, LcdDisplay& lcd)
{
    bool keyHandled = true;

    switch (key)
    {
        case 'N':
            Serial.println("NEXT");
            break;
        case 'B':
            Serial.println("BEFORE");
            break;
        case 'I':
            Serial.println("INC");
            break;
        case 'D':
            Serial.println("DEC");
            break;
        case 'S':
            Serial.println("SEL");
            break;
        default:
            keyHandled = false;
            break;
    }

    if (keyHandled)
        lcd.print(config);
}