#include <Arduino.h>
#include <SPI.h>
#include <RH_RF69.h>

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
          return "@";
        }
      } else {
        return "!";
      }
    }
    void send(RH_RF69 r, String d) {
      int dlen = d.length();
      char charra[dlen];

      for(int i=0; i<dlen; i++)
        charra[i] = d[i];

      for(char c: charra)
        Serial.println(c);

      r.send((uint8_t*)charra, strlen(charra));

      r.waitPacketSent();
    }
};
