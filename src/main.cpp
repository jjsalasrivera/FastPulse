#include <Arduino.h>
#include "configuration_menu.h"
#include "lcd_display.h"
#include "keypad_config.h"
#include "pulse_outputs.h"
#include "types.h"

LcdDisplay lcd;
TimingConfiguration config;

void setup() 
{
    Serial.begin(115200);
    lcd.initialize();
 
    config.frequencyHz = 30;
    config.pulsesPerCycle = 10;
    config.carrierFrequencyMicroseconds = 100;
    config.interPeakDelayMicroseconds = 1;
    config.symmetry = kSymmetryR;
    config.groupDelayMilliseconds = 0.5f;
    
    lcd.print(config);

    initializePulseOutputs();
}

void loop() 
{
    const char key = readKeypadKey();
    if (key != kNoKey)
        handleKey(key, config, lcd);

    runPulseOutputs(config);
}
