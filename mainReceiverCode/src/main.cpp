// ROVER
#include <Arduino.h>
#include <SPI.h>
#include <RH_RF69.h>
#include <Comm.h>
// #include "../common/Defs.h"

//defs for radio
// #define RF69_FREQUENCY  900.0
// #define RFM69_SLAVE     8
// #define RFM69_INTERRUPT 3
// #define RFM69_RST       4

RH_RF69 rf69(RFM69_SLAVE, RFM69_INTERRUPT);
Comm rad(rf69);

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;
}

void loop()
{
  // Serial.println("Sending to rf69_server");
  // Send a message to rf69_server
  // uint8_t data[] = "12:25";
  // rf69.send(data, sizeof(data));
  //
  // rf69.waitPacketSent();
  // Now wait for a reply
  // uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
  // uint8_t len = sizeof(buf);
  //
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

  rad.send(rf69, "test");

  delay(400);
}
