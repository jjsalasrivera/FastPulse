#include <Arduino.h>
#include "lcd_display.h"
#include "types.h"

#define Hz 30 // set your stimulation frequency
#define burst 10 // set your pulse number in the burst

LcdDisplay lcd;

constexpr PulseOutputGroup group1{
    {&DDRB, &PORTB, B00010000}, // Pin 10
    {&DDRH, &PORTH, B01000000}  // Pin 9
};

constexpr PulseOutputGroup group2{
    {nullptr, nullptr, 0},
    {nullptr, nullptr, 0}
};

constexpr PulseOutputGroup group11{
    {nullptr, nullptr, 0},
    {nullptr, nullptr, 0}
};

constexpr PulseOutputGroup group12{
    {nullptr, nullptr, 0},
    {nullptr, nullptr, 0}
};

inline void activatePin(const PulseOutputPin& pin) {
    *pin.outputRegister |= pin.bitMask;
}

inline void deactivatePin(const PulseOutputPin& pin) {
    *pin.outputRegister &= static_cast<unsigned char>(~pin.bitMask);
}

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    lcd.initialize();
    lcd.print("FastPulse listo", 0, 0);
    lcd.print("Estimulo activo", 0, 1);

    //pinMode(LOGIC_IN1, OUTPUT);
    //pinMode(LOGIC_IN2, OUTPUT); 
    DDRB = B00010000; // Pin 10
    DDRH = B01000000; // Pin 9
}
void loop() {
    for (int i=0; i<burst; i++) {
        // positive phase
        //digitalWrite(LOGIC_IN1, LOW);
        //digitalWrite(LOGIC_IN2, HIGH);
        activatePin(group1.positive);
        deactivatePin(group1.negative);
        delayMicroseconds(50);

        // negetive phase
        //digitalWrite(LOGIC_IN1, HIGH);    
        //digitalWrite(LOGIC_IN2, LOW);
        deactivatePin(group1.positive);
        activatePin(group1.negative);
        delayMicroseconds(50);
    }

    // stimulation delay
    //digitalWrite(LOGIC_IN1, LOW);    
    //digitalWrite(LOGIC_IN2, LOW);
    deactivatePin(group1.positive);
    deactivatePin(group1.negative);

    delay(1000/Hz);
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // toggle LED for visual feedback
}