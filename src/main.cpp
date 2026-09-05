#include <Arduino.h>
#include "lcd_display.h"
#include "types.h"

LcdDisplay lcd;
TimingConfiguration config;

// Pines 6, 7, 8 y 9 (PH3, PH4, PH5 y PH6) para el grupo 1
constexpr PulseOutputGroup group1{
    {&DDRH, &PORTH, B00101000}, // Pines 6 y 8 (PH3 y PH5)
    {&DDRH, &PORTH, B01010000}  // Pines 7 y 9 (PH4 y PH6)
};

// pines 10, 11, 12 y 13 (PB4, PB5, PB6 y PB7) para el grupo 2
constexpr PulseOutputGroup group2{
    {&DDRB, &PORTB, B01010000}, // Pines 10 y 12 (PB4 y PB6)
    {&DDRB, &PORTB, B10100000}  // Pines 11 y 13 (PB5 y PB7)
};

inline void activatePin(const PulseOutputPin& pin);
inline void deactivatePin(const PulseOutputPin& pin);
inline void runGroup(const PulseOutputGroup& group);
inline void runSynchronizedGroups();

void setup() 
{
    int d = PORTH;
    pinMode(LED_BUILTIN, OUTPUT);
    lcd.initialize();
 
    config.frequencyHz = 30;
    config.pulsesPerCycle = 10;
    config.carrierFrequencyMicroseconds = 100;
    config.interPeakDelayMicroseconds = 1;
    config.symmetry = kSymmetryR;
    config.groupDelayMilliseconds = 0.5f;
    
    lcd.print(config);

    *group1.positive.directionRegister |= group1.positive.bitMask;
    *group1.negative.directionRegister |= group1.negative.bitMask;

    *group2.positive.directionRegister |= group2.positive.bitMask;
    *group2.negative.directionRegister |= group2.negative.bitMask;
}

void loop() 
{
    if (config.symmetry == kSymmetryS)
    {
        runSynchronizedGroups();
        delay(1000 / config.frequencyHz);
        return;
    }

    runGroup(group1);
    delayMicroseconds(config.groupDelayMilliseconds * 1000);

    const unsigned long group2StartMicroseconds = micros();
    runGroup(group2);

    const unsigned long group2DurationMicroseconds = micros() - group2StartMicroseconds;
    const unsigned long periodMicroseconds = 1000000UL / config.frequencyHz;

    if (group2DurationMicroseconds < periodMicroseconds)
        delayMicroseconds(periodMicroseconds - group2DurationMicroseconds);
}

inline void activatePin(const PulseOutputPin& pin) {
    *pin.outputRegister |= pin.bitMask;
}

inline void deactivatePin(const PulseOutputPin& pin) {
    *pin.outputRegister &= static_cast<unsigned char>(~pin.bitMask);
}

inline void runGroup(const PulseOutputGroup& group) 
{
    for (unsigned int i = 0; i < config.pulsesPerCycle; ++i) 
    {
        activatePin(group.positive);
        deactivatePin(group.negative);
        delayMicroseconds(config.interPeakDelayMicroseconds);

        deactivatePin(group.positive);
        activatePin(group.negative);
        delayMicroseconds(config.interPeakDelayMicroseconds);
    }

    deactivatePin(group.positive);
    deactivatePin(group.negative);
}

inline void runSynchronizedGroups()
{
    for (unsigned int i = 0; i < config.pulsesPerCycle; ++i) 
    {
        activatePin(group1.positive);
        deactivatePin(group1.negative);
        activatePin(group2.positive);
        deactivatePin(group2.negative);
        
        delayMicroseconds(config.interPeakDelayMicroseconds);

        deactivatePin(group1.positive);
        activatePin(group1.negative);
        deactivatePin(group2.positive);
        activatePin(group2.negative);
        delayMicroseconds(config.interPeakDelayMicroseconds);
    }

    deactivatePin(group1.positive);
    deactivatePin(group1.negative);
    deactivatePin(group2.positive);
    deactivatePin(group2.negative);
}
