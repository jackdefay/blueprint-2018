// ROVER
#include <Arduino.h>
#include <SPI.h>
#include <RH_RF69.h>

//defs for radio
#define RF69_FREQUENCY  900.0
#define RFM69_SLAVE     8
#define RFM69_INTERRUPT 3
#define RFM69_RST       4

//defs for ultra
#define ULTRA_SEND  10
#define ULTRA_REC   11

String receive(RH_RF69 r, int timeOut);
String ultra();

RH_RF69 rf69(RFM69_SLAVE, RFM69_INTERRUPT);

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // radio
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, LOW);

  // manual reset
  digitalWrite(RFM69_RST, HIGH);
  delay(10);
  digitalWrite(RFM69_RST, LOW);
  delay(10);

  // ultra
  pinMode(ULTRA_SEND, OUTPUT);
  pinMode(ULTRA_REC, LOW);

  if (!rf69.init())
    Serial.println("ERROR: init failed");

  if (!rf69.setFrequency(RF69_FREQUENCY))
    Serial.println("ERROR: setFrequency failed");

  uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
  rf69.setEncryptionKey(key);
}

void loop() {
  uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

String SSS = "";

  if (rf69.waitAvailableTimeout(500)) {
    if (rf69.recv(buf, &len)) {
      char* temp = (char*)buf;
      String tempS(temp);
      tempS = '='+tempS;
      Serial.println(tempS);
      SSS = tempS;
    } else {
      Serial.println("@");
    }
  } else {
    Serial.println("!");
  }
  delay(100);

  if(SSS != "@" && SSS != "!") {
    
  }

  String d = ultra();
  int dlen = d.length();
  char charra[dlen];
  for(int i=0; i<dlen; i++)
    charra[i] = d[i];
  Serial.print("\nSending...");
  rf69.send((uint8_t*)charra, strlen(charra));
  rf69.waitPacketSent();
  Serial.print("Sent: ''");
  Serial.print(d);
  Serial.print("''");
}

String receive(RH_RF69 r, int timeOut) {
  uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  if (r.waitAvailableTimeout(timeOut)) {
    if (r.recv(buf, &len)) {
      char* temp = (char*)buf;
      String tempS(temp);
      tempS = '='+tempS;
      return tempS;
    } else {
      return "@";
    }
  } else {
    return "!";
  }
}

String ultra() {
  unsigned long sendTime;
  unsigned long receiveTime;
  unsigned long timeBetween;
  unsigned long distance;
  int returnValue;

  sendTime = micros();
  digitalWrite(ULTRA_SEND, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRA_SEND, LOW);

  while(digitalRead(ULTRA_REC) == 0);
  while(digitalRead(ULTRA_REC) == 1);

  receiveTime = micros();
  timeBetween = sendTime-receiveTime;

  distance = timeBetween*17/1000%1000;
  if(distance > 940) returnValue = 5;
  else if(distance > 930) returnValue = 4;
  else if(distance > 920) returnValue = 3;
  else if(distance > 905) returnValue = 2;
  else if(distance > 895) returnValue = 1;
  else returnValue = 0;
  delay(100);

  char tempx[1];
  itoa(returnValue, tempx, 1);
  String ret = tempx;
  return ret;
}
