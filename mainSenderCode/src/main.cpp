#include <Arduino.h>
#include <Adafruit_seesaw.h>
#include <SPI.h>
#include <RH_RF69.h>

//defs for controller
Adafruit_seesaw ss;
#define BUTTON_RIGHT 6
#define BUTTON_DOWN  7
#define BUTTON_LEFT  9
#define BUTTON_UP    10
#define BUTTON_SEL   14
uint32_t button_mask = (1 << BUTTON_RIGHT) | (1 << BUTTON_DOWN) | (1 << BUTTON_LEFT) | (1 << BUTTON_UP) | (1 << BUTTON_SEL);
#define IRQ_PIN   5

//defs for radio
#define RF69_FREQUENCY  900.0
#define RFM69_SLAVE     8
#define RFM69_INTERRUPT 3
#define RFM69_RST       4

RH_RF69 rf69(RFM69_SLAVE, RFM69_INTERRUPT);

void setup() {
  Serial.begin(115200);

  //controller setup
  ss.begin(0x49);
  ss.pinModeBulk(button_mask, INPUT_PULLUP);
  ss.setGPIOInterrupts(button_mask, 1);
  pinMode(IRQ_PIN, INPUT);

  //radio setup
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, LOW);

  //manual reset
  digitalWrite(RFM69_RST, HIGH);
  delay(10);
  digitalWrite(RFM69_RST, LOW);
  delay(10);

  if (!rf69.init()) {
    while (1);
  }
  Serial.println("RFM69 initialized");
  if (!rf69.setFrequency(RF69_FREQUENCY))
    Serial.println("setFrequency failed");

  uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
  rf69.setEncryptionKey(key);
}

void loop() {
  // char radiopacket[20];
  // char temp[5];
  // String tempWord = "####";
  // int x = ss.analogRead(2);
  // int y = ss.analogRead(3);

  Serial.println("Sending to rf69_server");
  // Send a message to rf69_server
  uint8_t data[] = "12:25";
  rf69.send(data, sizeof(data));

  rf69.waitPacketSent();
  Serial.println("SENT!");

  delay(400);

  delay(10);
}
