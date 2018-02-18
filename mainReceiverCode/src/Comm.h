#include <Arduino.h>
#include <SPI.h>
#include <RH_RF69.h>

//defs for radio
#define RF69_FREQUENCY  900.0
#define RFM69_SLAVE     8
#define RFM69_INTERRUPT 3
#define RFM69_RST       4

class Comm {
  public:
    Comm() {}
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
          Serial.print("Gatit");
          return "!";
        }
      } else {
        Serial.println("Nothing recieved");
        return "!";
      }
    }
    void send(RH_RF69 r, String d) {
      char charra[20];
      d.toCharArray(charra, 20);
      r.send((uint8_t*)charra, strlen(charra));

      // r.waitPacketSent();
    }
};
