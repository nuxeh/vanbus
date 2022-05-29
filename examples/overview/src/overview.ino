#include <vanbus.h>

Vanbus vanbus();

void setup() {}

void loop() {
  uint8_t buf[8];

  VanbusMsg msg;
  msg.set(0xA2);

  Serial.print("Set 0xA2, length: ");
  Serial.println(msg.len());

  uint8_t written = msg.writeBytes(buf, 8);

  Serial.print("Wrote to buffer, length: ");
  Serial.println(written);

}
