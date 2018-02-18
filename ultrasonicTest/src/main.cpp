#include <Arduino.h>


const int sendPin = 10;
const int receivePin = 11;

void setup() {
    pinMode(sendPin, OUTPUT)
    pinMode(receivePin, LOW);
    Serial.begin(115200);
}

void loop() {
    unsigned long sendTime = 0;
    unsigned long receiveTime = 0;
    unsigned long timeBetween = 0;
    unsigned int distanceCm = 0;

    digitalWrite(sendPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(sendPin, LOW);

    while(digitalRead(receivePin) == 0);

    sendTime = micros();

}
