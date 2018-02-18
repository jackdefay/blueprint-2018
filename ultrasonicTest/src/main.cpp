#include <Arduino.h>


const int sendPin = 10;
const int receivePin = 11;

void setup() {
    pinMode(sendPin, OUTPUT);
    pinMode(receivePin, LOW);
    Serial.begin(115200);
}

void loop() {
    unsigned long sendTime;
    unsigned long receiveTime;
    unsigned long timeBetween;
    unsigned long distanceCm;

    sendTime = micros();
    digitalWrite(sendPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(sendPin, LOW);

    while(digitalRead(receivePin) == 0);
    while(digitalRead(receivePin) == 1);

    receiveTime = micros();
    timeBetween = sendTime-receiveTime;

    distanceCm = timeBetween*17/1000%1000;
    Serial.println(distanceCm);
    delay(100);
}   //the measurements are weird, but basically, the
//value of 948 or higher is close, the value of 918 is
//a foot-a foot and a half and 900 is about 2 feet
