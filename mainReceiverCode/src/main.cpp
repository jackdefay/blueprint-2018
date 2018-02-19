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
#define ULTRA_SEND  15
#define ULTRA_REC   16

#define rfpos A5
#define rfneg A2
#define rbpos A1
#define rbneg 5
#define lfpos 11
#define lfneg 10
#define lbpos 9
#define lbneg 6

#define pwmrf 13
#define pwmrb 12
#define pwmlf A3
#define pwmlb A4

// String receive(RH_RF69 r, int timeOut);
String ultra();
void setDirection(char motor, bool direction);
int clip(int num);
void setSpeed(int pwmr, int pwml);

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

  pinMode(rfpos, OUTPUT);
  pinMode(rfneg, OUTPUT);
  pinMode(rbpos, OUTPUT);
  pinMode(rbneg, OUTPUT);
  pinMode(lfpos, OUTPUT);
  pinMode(lfneg, OUTPUT);
  pinMode(lbpos, OUTPUT);
  pinMode(lbneg, OUTPUT);
  pinMode(pwmrf, OUTPUT);
  pinMode(pwmrb, OUTPUT);
  pinMode(pwmlf, OUTPUT);
  pinMode(pwmlb, OUTPUT);

  setDirection('r', true);
  setDirection('l', true);

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

  String coords = "";
  Serial.print("TEST");

  if (rf69.available()) {
    if (rf69.recv(buf, &len)) {
      char* temp = (char*)buf;
      String tempS(temp);
      Serial.println(tempS);
      coords = tempS;
    } else {
      // Serial.println("@");
      coords = "@";
    }
  } else {
    // Serial.println("!");
    coords = "!";
  }
  delay(100);
  Serial.println(coords);
//-32:23:
  // coords = "-32:23:";

  String pwml="", pwmr="";

  int nextStart = 0;
  if(coords != "@" && coords != "!") {
    for(int i=0; i<coords.length(); i++) {
      if(coords[i] != ':')
        pwml += coords[i];
      else {
        nextStart = i+1;
        break;
      }
    }
    for(int i=nextStart; i<coords.length()-1; i++) {
      pwmr += coords[i];
    }

    Serial.print("TEST");
    Serial.println(pwml);
    Serial.println(pwmr);

    int pwmlint = pwml.toInt();
    int pwmrint = pwmr.toInt();

    setSpeed(pwmrint, pwmlint);
  }

  Serial.print("SONIC: ");
  String d = ultra();
  Serial.println(d);
  int dlen = d.length();
  char charra[dlen];
  for(int i=0; i<dlen; i++)
    charra[i] = d[i];
  Serial.print("\nSending... ");
  rf69.send((uint8_t*)charra, strlen(charra));
  rf69.waitPacketSent();
  Serial.print("Sent: ''");
  Serial.print(d);
  Serial.print("''");
}

// String receive(RH_RF69 r, int timeOut) {
//   uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
//   uint8_t len = sizeof(buf);
//   if (r.waitAvailableTimeout(timeOut)) {
//     if (r.recv(buf, &len)) {
//       char* temp = (char*)buf;
//       String tempS(temp);
//       tempS = '='+tempS;
//       return tempS;
//     } else {
//       return "@";
//     }
//   } else {
//     return "!";
//   }
// }

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

  Serial.print(returnValue);

  char tempx[1];
  itoa(returnValue, tempx, 1);
  String ret = tempx;
  return ret;
}

void setDirection(char motor, bool direction){  //1 = forwards, 0 = backwards
  Serial.println("In function setDirection");
  if(motor == 'r'){
    digitalWrite(rfpos, (int) direction);
    digitalWrite(rfneg, (int) !direction);
    digitalWrite(rbpos, (int) direction);
    digitalWrite(rbneg, (int) !direction);
  }
  else if(motor == 'l'){                        //the left wheels are reversed
    digitalWrite(lfpos, (int) !direction);
    digitalWrite(lfneg, (int) direction);
    digitalWrite(lbpos, (int) !direction);
    digitalWrite(lbneg, (int) direction);
  }
}

void setSpeed(int pwmr, int pwml){
  if(pwmr>=0) setDirection('r', true);
  else setDirection('r', false);

  if(pwml>=0) setDirection('l', true);
  else setDirection('l', false);

  pwmr = clip(pwmr);
  pwml = clip(pwml);

  Serial.print("the values being written to the motors are: "); Serial.print(pwmr); Serial.print(", "); Serial.println(pwml);

  analogWrite(pwmrf, pwmr);
  analogWrite(pwmrb, pwmr);
  analogWrite(pwmlf, pwml);
  analogWrite(pwmlb, pwml);
}

int clip(int num){
  if(num>=0 && num<=255) return num;
  else if(num>=-255 && num<0) return -num;
  else if(num>255 || num<-255) return 255;
  else return 0;
}
