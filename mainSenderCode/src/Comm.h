#include <Arduino.h>
#include <SPI.h>
#include <RH_RF69.h>

//defs for radio
#define RF69_FREQUENCY  900.0
#define RFM69_SLAVE     8
#define RFM69_INTERRUPT 3
#define RFM69_RST       4

class Comm
{
  public:
    Comm(RH_RF69 r);
    String receive(RH_RF69 r, int timeOut);
    void send(RH_RF69 r, String d);
  private:
    int _pin;
};

Comm::Comm(RH_RF69 r)
{
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, LOW);

  // manual reset
  digitalWrite(RFM69_RST, HIGH);
  delay(10);
  digitalWrite(RFM69_RST, LOW);
  delay(10);

  if (!r.init())
    Serial.println("ERROR: init failed");

  if (!r.setFrequency(RF69_FREQUENCY))
    Serial.println("ERROR: setFrequency failed");

  uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
  r.setEncryptionKey(key);
}

String Comm::receive(RH_RF69 r, int timeOut) {
  uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (r.waitAvailableTimeout(timeOut))
  {
    if (r.recv(buf, &len))
    {
      char* temp = (char*)buf;
      String tempS(temp);
      tempS = '='+tempS;
      return tempS;
    }
    else
    {
      return "!";
    }
  }
  else
  {
    Serial.println("No reply, is rf69_server running?");
  }
}

void Comm::send(RH_RF69 r, String d) {
  char charra[20];
  d.toCharArray(charra, 20);
  r.send((uint8_t*)charra, strlen(charra));

  r.waitPacketSent();
}
