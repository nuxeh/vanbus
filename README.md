# Vanbus

A simple pub sub protocol (encoder/decoder) for embedded systems.

Addressing is by a path, similar(ish) to MQTT, where devices are addressed by
paths composed of single byte/character path elements.

e.g. `a/b/c`.

The `a` path element is intended to be a group.
The `b` path element is intended to be a device.
The `c` path element is intended to be a field.

Message payloads are typed, and can be:

- Byte
- Float (32 bit IEEE 754)
- Short
- Signed Short
- Long
- Signed Long

Encoding is intended to be endianness safe, and platform independent, and the
library is intended to be universal - it's a basic C++ library, working on
Arduino, but not relying on the arduino API.

Messages are written to, and parsed from, a buffer of bytes, so transport can
be done in any manner desired, though the intention is to work well with PJON,
and generally packetised/framed transport protocols.

A device can subscribe to a topic/path, and dispatches callback functions on
processing of relevant packets for further processing.

    Vanbus<2> vanbus; // template parameter is the number of callbacks to
                      // assign memory for

    void l_c_b(VanbusMsg *m) {
      Serial.print(F("Ceiling light brightness change: "));
      Serial.println(m->getByte());
    }
    
    void l_c_s(VanbusMsg *m) {
      Serial.print(F("Ceiling light state change: "));
      Serial.println(m->getByte()); // bool type?
    }
    
    void setup() {    
      vanbus.subscribe(l_c_b, 'l', 'c', 'b');
      vanbus.subscribe(l_c_s, 'l', 'c', 's');
    }
    
Messages can be sent to paths which include a wildcard, 0x00, e.g. to send the
same message to all devices in the same group.

    uint8_t buf[8];
    
    // send all lights brightness 90 message
    msg.path('l', 0x00, 'b');
    msg.set(0x5A);
    msg.writeBytes(buf, 8);
    
    vanbus.receive(buf, 8);
