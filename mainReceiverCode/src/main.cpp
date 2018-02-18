// ROVER
#include <Arduino.h>
#include <SPI.h>
#include <RH_RF69.h>
#include <Comm.h>

//defs for radio
// #define RF69_FREQUENCY  900.0
// #define RFM69_SLAVE     8
// #define RFM69_INTERRUPT 3
// #define RFM69_RST       4

RH_RF69 rf69(RFM69_SLAVE, RFM69_INTERRUPT);
Comm rad;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, LOW);

  // manual reset
  digitalWrite(RFM69_RST, HIGH);
  delay(10);
  digitalWrite(RFM69_RST, LOW);
  delay(10);

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

  if (rf69.waitAvailableTimeout(500)) {
    if (rf69.recv(buf, &len)) {
      char* temp = (char*)buf;
      String tempS(temp);
      tempS = '='+tempS;
      Serial.println(tempS);
    } else {
      Serial.println("@");
    }
  } else {
    Serial.println("!");
  }

  // String x = rad.receive(rf69, 500);
  // Serial.println(x);

  delay(400);
}
