#include <Arduino.h>
#include <SPI.h>
#include <RH_RF69.h>

#define RF69_FREQUENCY  900.0
#define RFM69_SLAVE     8
#define RFM69_INTERRUPT 3
#define RFM69_RST       4

RH_RF69 rf69(RFM69_SLAVE, RFM69_INTERRUPT);

void setup()
{
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
    Serial.println("init failed");

    Serial.println("Initialized");

  if (!rf69.setFrequency(RF69_FREQUENCY))
    Serial.println("setFrequency failed");

  uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
  rf69.setEncryptionKey(key);
}

void loop()
{
  Serial.println("Sending to rf69_server");
  // Send a message to rf69_server
  uint8_t data[] = "12:25";
  rf69.send(data, sizeof(data));

  rf69.waitPacketSent();
  Serial.println("SENT!");
  // Now wait for a reply
  // uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
  // uint8_t len = sizeof(buf);

  // if (rf69.waitAvailableTimeout(500))
  // {
  //   // Should be a reply message for us now
  //   if (rf69.recv(buf, &len))
  //   {
  //     Serial.print("got reply: ");
  //     Serial.println((char*)buf);
  //   }
  //   else
  //   {
  //     Serial.println("recv failed");
  //   }
  // }
  // else
  // {
  //   Serial.println("No reply, is rf69_server running?");
  // }
  delay(400);
}
