#include <Arduino.h>

// Basic sketch for setting up stimulation trigger for OpenXstim
// For any queries please contact: md.malam@connect.polyu.hk
#define Hz 30 // set your stimulation frequency
#define burst 10 // set your pulse number in the burst

//#define LOGIC_IN1 9
//#define LOGIC_IN2 10
void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
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
        PORTB = B00010000; // faster digitalwrite operation
        PORTH = B00000000;
        delayMicroseconds(50);

        // negetive phase
        //digitalWrite(LOGIC_IN1, HIGH);    
        //digitalWrite(LOGIC_IN2, LOW);
        PORTB = B00000000;
        PORTH = B01000000;
        delayMicroseconds(50);
    }

    // stimulation delay
    //digitalWrite(LOGIC_IN1, LOW);    
    //digitalWrite(LOGIC_IN2, LOW);
    PORTB = B00000000;  // faster digitalwrite operation  
    PORTH = B00000000;

    delay(1000/Hz);
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // toggle LED for visual feedback
}