#include <Arduino.h>
#include <Adafruit_seesaw.h>
#include <SPI.h>
#include <RH_RF69.h>
#include <Wire.h>
#include "Adafruit_DRV2605.h"
// #include <string>

#define JOYSTICK_RANGE 1023

//defs for radio
#define RF69_FREQUENCY  900.0
#define RFM69_SLAVE     8
#define RFM69_INTERRUPT 3
#define RFM69_RST       4

//defs for controller
Adafruit_seesaw ss;
Adafruit_DRV2605 drv;
#define BUTTON_RIGHT 6
#define BUTTON_DOWN  7
#define BUTTON_LEFT  9
#define BUTTON_UP    10
#define BUTTON_SEL   14
uint32_t button_mask = (1 << BUTTON_RIGHT) | (1 << BUTTON_DOWN) | (1 << BUTTON_LEFT) | (1 << BUTTON_UP) | (1 << BUTTON_SEL);
#define IRQ_PIN   5

// void send(RH_RF69 r, String d);

RH_RF69 rf69(RFM69_SLAVE, RFM69_INTERRUPT);

void setup() {
  Serial.begin(115200);

  //controller setup
  ss.begin(0x49);
  ss.pinModeBulk(button_mask, INPUT_PULLUP);
  ss.setGPIOInterrupts(button_mask, 1);
  pinMode(IRQ_PIN, INPUT);

  //radio
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

  drv.begin();
  drv.selectLibrary(1);

  drv.setMode(DRV2605_MODE_INTTRIG);
}

void loop() {
  static int level;
  static uint8_t effect;
  String lvl = "1";
  uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (rf69.available()) {
    if (rf69.recv(buf, &len)) {
      char* temp = (char*)buf;
      String tempS(temp);
      Serial.println(tempS);
      lvl = tempS;
    } else {
      Serial.println("@");
    }
  } else {
    Serial.println("!");
  }
  delay(100);

  level = lvl.toInt();

  int x = ss.analogRead(2);
  int y = ss.analogRead(3);
  int pwmr, pwml;
  int xcoord = y;
  int ycoord = x;

  xcoord = xcoord - (JOYSTICK_RANGE/2);
  ycoord = (JOYSTICK_RANGE/2) - ycoord;

  //-509, 512
  xcoord = xcoord;
  pwmr = (int) 2*(ycoord * 255 /JOYSTICK_RANGE);  //***just going to code it to do turns for now, will add rotation functionality later***
  pwml = pwmr;
  Serial.print("the forward power to the motors before correction is: "); Serial.println(pwmr);
  if(xcoord>-20 && xcoord<20 && ycoord>20 && ycoord<20){
    pwmr=0;
    pwml=0;
  }
  else if(xcoord > JOYSTICK_RANGE/2){
    pwmr -= (int) (2*(xcoord * 255)/JOYSTICK_RANGE);
    pwml += (int) (2*(xcoord * 255)/JOYSTICK_RANGE);
  }
  else if(xcoord < JOYSTICK_RANGE/2){
    pwml -= (int) (-2*(xcoord * 255)/JOYSTICK_RANGE);
    pwmr += (int) (-2*(xcoord * 255)/JOYSTICK_RANGE);
  }

  Serial.print("pwml: ");
  Serial.println(pwml);
  Serial.print("pwmr: ");
  Serial.println(pwmr);
  if(level == 0)
    effect = 0;
  else if(level == 1)
    effect = 51;
  else if (level == 2)
    effect = 50;
  else if (level == 3)
    effect = 49;
  else if (level == 4)
    effect = 48;
  else if (level == 5)
    effect = 47;

  drv.setWaveform(0, effect);  // play effect
  drv.setWaveform(1, 0);       // end waveform
    // plays
  drv.go();

  String d = "";
  char temp[5];

  itoa(xcoord, temp, 10);
  d += temp;

  itoa(ycoord, temp, 10);
  d += ':';
  d += temp;
  d += ':';

  Serial.println(d);

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
  delay(200);
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
