#include <Arduino.h>
#include <Adafruit_seesaw.h>
#include <SPI.h>
#include <RH_RF69.h>

//defs for radio
#define RF69_FREQUENCY  900.0
#define RFM69_SLAVE     8
#define RFM69_INTERRUPT 3
#define RFM69_RST       4

//defs for controller
Adafruit_seesaw ss;
#define BUTTON_RIGHT 6
#define BUTTON_DOWN  7
#define BUTTON_LEFT  9
#define BUTTON_UP    10
#define BUTTON_SEL   14
uint32_t button_mask = (1 << BUTTON_RIGHT) | (1 << BUTTON_DOWN) | (1 << BUTTON_LEFT) | (1 << BUTTON_UP) | (1 << BUTTON_SEL);
#define IRQ_PIN   5

void send(RH_RF69 r, String d);

RH_RF69 rf69(RFM69_SLAVE, RFM69_INTERRUPT);

void setup() {
  Serial.begin(115200);

  //controller setup
  ss.begin(0x49);
  ss.pinModeBulk(button_mask, INPUT_PULLUP);
  ss.setGPIOInterrupts(button_mask, 1);
  pinMode(IRQ_PIN, INPUT);

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
  String d = "Jackattack";

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

  // send(rf69, "Test2okay");
  delay(400);
}

void send(RH_RF69 r, String d) {
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
