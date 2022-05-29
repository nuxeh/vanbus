#define VANBUS_MAX_SUBSCRIPTIONS 6
#include <vanbus.h>

Vanbus<6> vanbus;

void a_b_c(VanbusMsg *m) {
  Serial.println(F("> Function a/b/c called"));
}

void a_b_d(VanbusMsg *m) {
  Serial.println(F("> Function a/b/d called"));
}

void a_b(VanbusMsg *m) {
  Serial.println(F("> Function a/b/* called"));
}

void a(VanbusMsg *m) {
  Serial.println(F("> Function a/*/* called"));
}

void b(VanbusMsg *m) {
  Serial.println(F("> Function */b/* called"));
}

void all(VanbusMsg *m) {
  Serial.println(F("> Function * called"));
}

void setup() {
  Serial.begin(115200);

  vanbus.subscribe(a_b_c, 'a', 'b', 'c');
  vanbus.subscribe(a_b_d, 'a', 'b', 'd');
  vanbus.subscribe(a_b, 'a', 'b');
  vanbus.subscribe(a, 'a');
  vanbus.subscribe(b, 0, 'b');
  vanbus.subscribe(all);

  uint8_t buf[8];

  Serial.println(F("Message: a/b/c"));
  VanbusMsg msg('a', 'b', 'c');
  msg.writeBytes(buf, 8);
  vanbus.receive(buf, 8);

  Serial.println(F("Message: a/b/d"));
  VanbusMsg msg2('a', 'b', 'd');
  msg2.writeBytes(buf, 8);
  vanbus.receive(buf, 8);

  Serial.println(F("Message: a/b/*"));
  VanbusMsg msg3('a', 'b');
  msg3.writeBytes(buf, 8);
  vanbus.receive(buf, 8);

  Serial.println(F("Message: a/*/*"));
  VanbusMsg msg4('a');
  msg4.writeBytes(buf, 8);
  vanbus.receive(buf, 8);

  Serial.println(F("Message: */b/*"));
  VanbusMsg msg5(0, 'b');
  msg5.writeBytes(buf, 8);
  vanbus.receive(buf, 8);
}

void loop() {}
