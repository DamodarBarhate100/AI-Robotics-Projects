#include <Arduino.h>
#include "JARVIS.h"

JARVIS J1;
void setup() {
    
    Serial.begin(115200);
    J1.initialize();
    delay(3000);
    J1.move_servo(2,-45);
    delay(3000);
    J1.move_servo(2,0);
    delay(3000);
    J1.move_servo(2,90);
    delay(3000);
    J1.move_servo(2 ,-90);

}

void loop() {

}