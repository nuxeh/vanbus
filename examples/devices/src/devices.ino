#include <vanbus.h>

Vanbus<7> vanbus;

// ceiling light callbacks
void l_c_b(VanbusMsg *m) {
  Serial.print(F("> Ceiling light brightness change: "));
  Serial.println(m->getByte());
}

void l_c_s(VanbusMsg *m) {
  Serial.print(F("> Ceiling light state change: "));
  Serial.println(m->getByte()); // bool type?
}

// side light callbacks
void l_s_b(VanbusMsg *m) {
  Serial.print(F("> Side light brightness change: "));
  Serial.println(m->getByte());
}

void l_s_s(VanbusMsg *m) {
  Serial.print(F("> Side light state change: "));
  Serial.println(m->getByte()); // bool type?
}

void l_s_t(VanbusMsg *m) {
  Serial.print(F("> Side light colour temperature change: "));
  Serial.println(m->getByte());
}

// door light callbacks
void l_d_b(VanbusMsg *m) {
  Serial.print(F("> Door light brightness change: "));
  Serial.println(m->getByte());
}

void l_d_s(VanbusMsg *m) {
  Serial.print(F("> Door light state change: "));
  Serial.println(m->getByte()); // bool type?
}


void setup() {
  Serial.begin(115200);

  // ceiling light
  // params: brightness, status
  vanbus.subscribe(l_c_b, 'l', 'c', 'b');
  vanbus.subscribe(l_c_s, 'l', 'c', 's');

  // side light
  // params: brightness, status, colour temperature
  vanbus.subscribe(l_s_b, 'l', 's', 'b');
  vanbus.subscribe(l_s_s, 'l', 's', 's');
  vanbus.subscribe(l_s_t, 'l', 's', 't');

  // door light
  // params: brightness, status
  vanbus.subscribe(l_d_b, 'l', 'd', 'b');
  vanbus.subscribe(l_d_s, 'l', 'd', 's');

  uint8_t buf[8];

  Serial.println(F("[ Ceiling light on ]"));
  VanbusMsg msg('l', 'c', 's');
  msg.set(0x01);
  msg.writeBytes(buf, 8);
  vanbus.receive(buf, 8);

  Serial.println(F("[ Ceiling light brightness 127 ]"));
  VanbusMsg msg2('l', 'c', 'b');
  msg2.set(0x7F);
  msg2.writeBytes(buf, 8);
  vanbus.receive(buf, 8);

  Serial.println(F("[ All lights brightness 90 ]"));
  VanbusMsg msg3('l', 0, 'b');
  msg3.set(0x5A);
  msg3.writeBytes(buf, 8);
  vanbus.receive(buf, 8);
}

void loop() {}
