#include <Arduino.h>
#include <Adafruit_seesaw.h>
#include <SPI.h>
#include <RH_RF69.h>
// #include <string>

#define JOYSTICK_RANGE 1023

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
}

void loop() {
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

  // Serial.print("pwml: ");
  // Serial.println(pwml);
  // Serial.print("pwmr: ");
  // Serial.println(pwmr);

  // String d = to_string(pwml)+':'+to_string(pwmr);

  String d = "";
  Serial.println(d);
  char temp[5];

  itoa(xcoord, temp, 10);
  d += temp;

  itoa(ycoord, temp, 10);
  d += ':';
  d += temp;

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
