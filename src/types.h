#pragma once

constexpr char kSymmetryR = 'R';
constexpr char kSymmetryS = 'S';

struct TimingConfiguration {
	unsigned short frequencyHz;
	unsigned short carrierFrequencyMicroseconds;
	unsigned short pulsesPerCycle;
	unsigned short interPeakDelayMicroseconds;
	char symmetry;
	float groupDelayMilliseconds;
};

struct PulseOutputPin {
	volatile unsigned char* const directionRegister;
	volatile unsigned char* const outputRegister;
	const unsigned char bitMask;
};

struct PulseOutputGroup {
	const PulseOutputPin positive;
	const PulseOutputPin negative;
};
