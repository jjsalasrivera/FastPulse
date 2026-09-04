#include "lcd_display.h"
#include "types.h"
#include <Wire.h>

LcdDisplay::LcdDisplay(unsigned char address) : lcd(address) 
{
}

void LcdDisplay::initialize() 
{
    Wire.begin();
    lcd.begin(16, 2);
    lcd.setBacklight(255);
    lcd.noCursor();
    lcd.clear();
}

void LcdDisplay::print(const char* message, unsigned char column, unsigned char row) 
{
    lcd.setCursor(column, row);
    lcd.print(message);
}

void LcdDisplay::clear() 
{
    lcd.clear();
}

void LcdDisplay::print(const TimingConfiguration& config) 
{
    lcd.clear();
    lcd.setCursor(0, 0);
    
    lcd.print(config.frequencyHz);
    lcd.print("Hz  ");

    lcd.print(config.carrierFrequencyMicroseconds);
    lcd.print("\%s ");

    lcd.print(config.pulsesPerCycle);
    lcd.print("P");
    
    lcd.setCursor(0, 1);

    char buffer[4];
    snprintf(buffer, sizeof(buffer), "%03d", config.interPeakDelayMicroseconds);
    lcd.print(buffer);
    lcd.print("\%s ");
    
    lcd.print(config.symmetry);
    lcd.print(" ");

    lcd.print(config.groupDelayMilliseconds);
    lcd.print("ms ");

    lcd.print("MD");
}