#include <Arduino.h>
#include "lcd_display.h"
#include "types.h"

LcdDisplay lcd;
TimingConfiguration config;

// Pines 0, 1, 2 y 3
constexpr PulseOutputGroup group1{
    {&DDRE, &PORTE, B00010001}, // Pines 0 y 2 (PE0 y PE4)
    {&DDRE, &PORTE, B00100010}  // Pines 1 y 3 (PE1 y PE5)
};

constexpr PulseOutputGroup group2{
    {&DDRH, &PORTH, B00101000}, // Pines 6 y 8 (PH3 y PH5)
    {&DDRH, &PORTH, B01010000}  // Pines 7 y 9 (PH4 y PH6)
};

inline void activatePin(const PulseOutputPin& pin) {
    *pin.outputRegister |= pin.bitMask;
}

inline void deactivatePin(const PulseOutputPin& pin) {
    *pin.outputRegister &= static_cast<unsigned char>(~pin.bitMask);
}

void setup() 
{
    pinMode(LED_BUILTIN, OUTPUT);
    lcd.initialize();
 
    config.frequencyHz = 30;
    config.pulsesPerCycle = 10;
    config.carrierFrequencyMicroseconds = 100;
    config.interPeakDelayMicroseconds = 1;
    config.symmetry = kSymmetryR;
    config.groupDelayMilliseconds = 0.5f;
    
    lcd.print(config);

    //pinMode(LOGIC_IN1, OUTPUT);
    //pinMode(LOGIC_IN2, OUTPUT); 
    //DDRB = B00010000; // Pin 10
    //DDRH = B01000000; // Pin 9
    pinMode(0, OUTPUT);
    pinMode(1, OUTPUT);
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
}
void loop() 
{
    //for (int i=0; i<burst; i++) {
    for (unsigned int i = 0; i < config.pulsesPerCycle; ++i) 
    {
        // positive phase
        activatePin(group1.positive);
        deactivatePin(group1.negative);
        delayMicroseconds(config.interPeakDelayMicroseconds);

        // negetive phase
        deactivatePin(group1.positive);
        activatePin(group1.negative);
        //delayMicroseconds(50);
        delayMicroseconds(config.interPeakDelayMicroseconds);
    }

    // stimulation delay
    //digitalWrite(LOGIC_IN1, LOW);    
    //digitalWrite(LOGIC_IN2, LOW);
    deactivatePin(group1.positive);
    deactivatePin(group1.negative);

    delay(1000/config.frequencyHz);

}
