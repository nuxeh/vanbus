#include <vanbus.h>

Vanbus vanbus();

void setup() {
  Serial.begin(115200);

  // tests
  read_write_byte();
}

void read_write_byte() {
  uint8_t buf[8];

  VanbusMsg msg;
  msg.set(0xA2);

  Serial.print("Set 0xA2, length: ");
  Serial.println(msg.len());

  uint8_t written = msg.writeBytes(buf, 8);

  Serial.print("Wrote to buffer, length: ");
  Serial.println(written);

  VanbusMsg msg2;
  int read_bytes = msg2.parseFromBytes(buf, written);

  Serial.print("Parsed from buffer, read bytes: ");
  Serial.println(read_bytes);
  Serial.print("Read value: ");
  Serial.println(msg2.getByte());
}

void loop() {}
