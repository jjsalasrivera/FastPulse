#include <Arduino.h>

#include "pulse_outputs.h"

namespace 
{
    // Pines 6, 7, 8 y 9 (PH3, PH4, PH5 y PH6) para el grupo 1.
    constexpr PulseOutputGroup group1{
        {&DDRH, &PORTH, B00101000},
        {&DDRH, &PORTH, B01010000}
    };

    // Pines 10, 11, 12 y 13 (PB4, PB5, PB6 y PB7) para el grupo 2.
    constexpr PulseOutputGroup group2{
        {&DDRB, &PORTB, B01010000},
        {&DDRB, &PORTB, B10100000}
    };

    inline void activatePin(const PulseOutputPin& pin)
    {
        *pin.outputRegister |= pin.bitMask;
    }

    inline void deactivatePin(const PulseOutputPin& pin)
    {
        *pin.outputRegister &= static_cast<unsigned char>(~pin.bitMask);
    }

    inline void runGroup(const PulseOutputGroup& group, const TimingConfiguration& config)
    {
        const int delayMicrosecondsValue = (config.carrierFrequencyMicroseconds - (2 * config.interPeakDelayMicroseconds)) / 2;

        for (unsigned int i = 0; i < config.pulsesPerCycle; ++i)
        {
            deactivatePin(group.negative);
            activatePin(group.positive);
            delayMicroseconds(delayMicrosecondsValue);

            deactivatePin(group.positive);
            delayMicroseconds(config.interPeakDelayMicroseconds);

            activatePin(group.negative);
            delayMicroseconds(delayMicrosecondsValue);
        }

        deactivatePin(group.positive);
        deactivatePin(group.negative);
    }

    inline void runSynchronizedGroups(const TimingConfiguration& config)
    {
        const int delayMicrosecondsValue =
            (config.carrierFrequencyMicroseconds -
            (2 * config.interPeakDelayMicroseconds)) / 2;

        for (unsigned int i = 0; i < config.pulsesPerCycle; ++i)
        {
            deactivatePin(group1.negative);
            deactivatePin(group2.negative);
            activatePin(group1.positive);
            activatePin(group2.positive);

            delayMicroseconds(delayMicrosecondsValue);

            deactivatePin(group1.positive);
            deactivatePin(group2.positive);
            delayMicroseconds(config.interPeakDelayMicroseconds);

            activatePin(group1.negative);
            activatePin(group2.negative);
            delayMicroseconds(delayMicrosecondsValue);
        }

        deactivatePin(group1.positive);
        deactivatePin(group1.negative);
        deactivatePin(group2.positive);
        deactivatePin(group2.negative);
    }
}

void initializePulseOutputs()
{
    *group1.positive.directionRegister |= group1.positive.bitMask;
    *group1.negative.directionRegister |= group1.negative.bitMask;
    *group2.positive.directionRegister |= group2.positive.bitMask;
    *group2.negative.directionRegister |= group2.negative.bitMask;
}

void runPulseOutputs(const TimingConfiguration& config)
{
    if (config.symmetry == kSymmetryS)
    {
        runSynchronizedGroups(config);
        delay(1000 / config.frequencyHz);
        return;
    }

    runGroup(group1, config);
    delayMicroseconds(config.groupDelayMilliseconds * 1000);

    const unsigned long group2StartMicroseconds = micros();
    runGroup(group2, config);

    const unsigned long group2DurationMicroseconds =
        micros() - group2StartMicroseconds;
    const unsigned long periodMicroseconds = 1000000UL / config.frequencyHz;

    if (group2DurationMicroseconds < periodMicroseconds)
        delayMicroseconds(periodMicroseconds - group2DurationMicroseconds);
}