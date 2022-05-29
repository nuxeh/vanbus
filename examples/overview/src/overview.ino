#include <vanbus.h>

Vanbus vanbus();

void setup() {}

void loop() {
  uint8_t buf[8];

  VanbusMsg msg;
  msg.set(0xA2);

}
