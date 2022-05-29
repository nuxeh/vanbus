#include <vanbus.h>

Vanbus vanbus();

void setup() {
  Serial.begin(115200);

  // tests
  read_write_byte();
  read_write_short();
}

void read_write_byte() {
  uint8_t buf[8];

  VanbusMsg msg;
  uint8_t foo = 0xA2;
  msg.set(foo);

  Serial.print("Set 0xA2, length: ");
  Serial.println(msg.len());

  uint8_t written = msg.writeBytes(buf, 8);

  Serial.print("Wrote to buffer, length: ");
  Serial.println(written);

  VanbusMsg msg2;
  int read_bytes = msg2.parseFromBytes(buf, written);

  Serial.print("Parsed from buffer, read bytes: ");
  Serial.println(read_bytes);
  Serial.print("Read value: 0x");
  Serial.println(msg2.getByte(), HEX);
}

void read_write_short() {
  uint8_t buf[8];

  VanbusMsg msg;
  uint16_t foo = 0xA2A2;
  msg.set(foo);

  Serial.print("Set 0xA2A2, length: ");
  Serial.println(msg.len());

  uint8_t written = msg.writeBytes(buf, 8);

  Serial.print("Wrote to buffer, length: ");
  Serial.println(written);

  VanbusMsg msg2;
  int read_bytes = msg2.parseFromBytes(buf, written);

  Serial.print("Parsed from buffer, read bytes: ");
  Serial.println(read_bytes);
  Serial.print("Read value: 0x");
  Serial.println(msg2.getUnsignedShort(), HEX);
}

void loop() {}
