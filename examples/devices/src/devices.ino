#include <vanbus.h>

Vanbus<7> vanbus;

// ceiling light callbacks
void l_c_b(VanbusMsg *m) {
  Serial.print(F("> Ceiling light brightness change: "));
  Serial.println(m->getByte());
}

void l_c_s(VanbusMsg *m) {
  Serial.print(F("> Ceiling light state change: "));
  Serial.println(m->getBit());
}

// side light callbacks
void l_s_b(VanbusMsg *m) {
  Serial.print(F("> Side light brightness change: "));
  Serial.println(m->getByte());
}

void l_s_s(VanbusMsg *m) {
  Serial.print(F("> Side light state change: "));
  Serial.println(m->getBit());
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
  Serial.println(m->getBit());
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
  VanbusMsg msg;

  Serial.println(F("[ Ceiling light on ]"));
  msg.path('l', 'c', 's');
  msg.set(true);
  msg.writeBytes(buf, 8);
  vanbus.receive(buf, 8);

  Serial.println(F("[ Ceiling light off ]"));
  msg.path('l', 'c', 's');
  msg.set(false);
  msg.writeBytes(buf, 8);
  vanbus.receive(buf, 8);

  Serial.println(F("[ Ceiling light brightness 127 ]"));
  msg.path('l', 'c', 'b');
  msg.set(0x7F);
  msg.writeBytes(buf, 8);
  vanbus.receive(buf, 8);

  Serial.println(F("[ Side light on ]"));
  msg.path('l', 's', 's');
  msg.set(true);
  msg.writeBytes(buf, 8);
  vanbus.receive(buf, 8);

  Serial.println(F("[ Side light brightness 127 ]"));
  msg.path('l', 's', 'b');
  msg.set(0x7F);
  msg.writeBytes(buf, 8);
  vanbus.receive(buf, 8);

  Serial.println(F("[ Door light on ]"));
  msg.path('l', 'd', 's');
  msg.set(true);
  msg.writeBytes(buf, 8);
  vanbus.receive(buf, 8);

  Serial.println(F("[ Door light brightness 127 ]"));
  msg.path('l', 'd', 'b');
  msg.set(0x7F);
  msg.writeBytes(buf, 8);
  vanbus.receive(buf, 8);

  Serial.println(F("[ All lights brightness 90 ]"));
  msg.path('l', 0x00, 'b');
  msg.set(0x5A);
  msg.writeBytes(buf, 8);
  vanbus.receive(buf, 8);
}

void loop() {}
