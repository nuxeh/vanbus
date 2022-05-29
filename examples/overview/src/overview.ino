#include <vanbus.h>

Vanbus vanbus();

void setup() {
  Serial.begin(115200);

  // tests
  read_write_byte();
  read_write_ushort();
  read_write_ulong();
  read_write_float();
}

void read_write_byte() {
  uint8_t buf[8];

  VanbusMsg msg;
  uint8_t foo = 0xA2;
  msg.set(foo);

  Serial.print("Set 0xA2, length: ");
  Serial.println(msg.len());

  uint8_t written = msg.writeBytes(buf, 8);

  Serial.print("Message type: ");
  Serial.println(msg.getType());

  Serial.print("Wrote to buffer, length: ");
  Serial.println(written);

  VanbusMsg msg2;
  int read_bytes = msg2.parseFromBytes(buf, written);

  Serial.print("Parsed message type: ");
  Serial.println(msg2.getType());

  Serial.print("Parsed from buffer, read bytes: ");
  Serial.println(read_bytes);
  Serial.print("Read value: 0x");
  Serial.println(msg2.getByte(), HEX);
}

void read_write_ushort() {
  uint8_t buf[8];

  VanbusMsg msg;
  uint16_t foo = 0xA1A2;
  msg.set(foo);

  Serial.print("Set 0xA1A2, length: ");
  Serial.println(msg.len());

  uint8_t written = msg.writeBytes(buf, 8);

  Serial.print("Message type: ");
  Serial.println(msg.getType());

  Serial.print("Wrote to buffer, length: ");
  Serial.println(written);

  VanbusMsg msg2;
  int read_bytes = msg2.parseFromBytes(buf, written);

  Serial.print("Parsed message type: ");
  Serial.println(msg2.getType());

  Serial.print("Parsed from buffer, read bytes: ");
  Serial.println(read_bytes);
  Serial.print("Read value: 0x");
  Serial.println(msg2.getUnsignedShort(), HEX);
}

void read_write_ulong() {
  uint8_t buf[8];

  VanbusMsg msg;
  uint32_t foo = 0xA1A2A3A4;
  msg.set(foo);

  Serial.print("Set 0xA1A2A3A4, length: ");
  Serial.println(msg.len());

  uint8_t written = msg.writeBytes(buf, 8);

  Serial.print("Wrote to buffer, length: ");
  Serial.println(written);

  Serial.print("Message type: ");
  Serial.println(msg.getType());

  VanbusMsg msg2;
  int read_bytes = msg2.parseFromBytes(buf, written);

  Serial.print("Parsed message type: ");
  Serial.println(msg2.getType());

  Serial.print("Parsed from buffer, read bytes: ");
  Serial.println(read_bytes);
  Serial.print("Read value: 0x");
  Serial.println(msg2.getUnsignedLong(), HEX);
}

void read_write_float() {
  uint8_t buf[8];

  VanbusMsg msg;
  float foo = 1.23456789;
  msg.set(foo);

  Serial.print("Set 1.23456789, length: ");
  Serial.println(msg.len());

  uint8_t written = msg.writeBytes(buf, 8);

  Serial.print("Wrote to buffer, length: ");
  Serial.println(written);

  Serial.print("Message type: ");
  Serial.println(msg.getType());

  VanbusMsg msg2;
  int read_bytes = msg2.parseFromBytes(buf, written);

  Serial.print("Parsed message type: ");
  Serial.println(msg2.getType());

  Serial.print("Parsed from buffer, read bytes: ");
  Serial.println(read_bytes);
  Serial.print("Read value: ");
  Serial.println(msg2.getFloat());
}

void loop() {}
