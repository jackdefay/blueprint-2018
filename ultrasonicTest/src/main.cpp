#include <Arduino.h>

String ultra();

const int sendPin = 10;
const int receivePin = 11;

void setup() {
    Serial.begin(115200);
}

void loop() {
    unsigned long sendTime;
    unsigned long receiveTime;
    unsigned long timeBetween;
    unsigned long distance;
    int returnValue;

    sendTime = micros();
    digitalWrite(sendPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(sendPin, LOW);

    while(digitalRead(receivePin) == 0);
    while(digitalRead(receivePin) == 1);

    receiveTime = micros();
    timeBetween = sendTime-receiveTime;

    // int lowerBound = 950;
    // int upperBound = 850;

    distance = timeBetween*17/1000%1000;

    // (distance - upperBound) * (5.0/(upperBound - lowerBound));

    if(distance > 940) returnValue = 5;
    else if(distance > 930) returnValue = 4;
    else if(distance > 920) returnValue = 3;
    else if(distance > 905) returnValue = 2;
    else if(distance > 895) returnValue = 1;
    else returnValue = 0;
    Serial.println(returnValue);
    delay(100);
}   //the measurements are weird, but basically, the
//value of 948 or higher is close, the value of 918 is
//a foot-a foot and a half and 900 is about 3 feet

// String ultra() {
//   unsigned long sendTime;
//   unsigned long receiveTime;
//   unsigned long timeBetween;
//   unsigned long distance;
//   int returnValue;
//
//   sendTime = micros();
//   digitalWrite(sendPin, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(sendPin, LOW);
//
//   while(digitalRead(receivePin) == 0);
//   while(digitalRead(receivePin) == 1);
//
//   receiveTime = micros();
//   timeBetween = sendTime-receiveTime;
//
//   // int lowerBound = 950;
//   // int upperBound = 850;
//
//   distance = timeBetween*17/1000%1000;
//
//   // (distance - upperBound) * (5.0/(upperBound - lowerBound));
//
//   if(distance > 940) returnValue = 5;
//   else if(distance > 930) returnValue = 4;
//   else if(distance > 920) returnValue = 3;
//   else if(distance > 905) returnValue = 2;
//   else if(distance > 895) returnValue = 1;
//   else returnValue = 0;
//   Serial.println(returnValue);
//   delay(100);
// }
