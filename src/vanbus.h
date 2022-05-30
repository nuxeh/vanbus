#pragma once

#include <stdint.h>

#ifndef VANBUS_MAX_SUBSCRIPTIONS
#define VANBUS_MAX_SUBSCRIPTIONS 5
#endif

typedef uint32_t vb_fixed_t;
#define FIXED_POINT_FRACTIONAL_BITS 8

enum VanbusMsgType {
  Vb_Byte = 0,
  Vb_Short,
  Vb_UShort,
  Vb_Long,
  Vb_ULong,
  Vb_Fixed,
  Vb_Fixed_Neg,
};

class VanbusMsg {
  public:
    VanbusMsg();
    VanbusMsg(uint8_t A, uint8_t B=0, uint8_t F=0);

    int parseFromBytes(uint8_t *bytes, uint8_t len);
    int writeBytes(uint8_t *bytes, uint8_t max);

    uint8_t len() { return length; };

    VanbusMsgType getType() { return type; };
    void setType(VanbusMsgType T) { type = T; };

    void path(uint8_t A, uint8_t B, uint8_t C) {
      pathA = A;
      pathB = B;
      pathC = C;
    };

    uint8_t getByte() { return payload[0]; };
    uint16_t getUnsignedShort() {
      uint16_t r = payload[0];
      r += (uint16_t)payload[1]<<8;
      return r;
    };
    int16_t getShort() { return (int16_t)getUnsignedShort(); };
    uint32_t getUnsignedLong() { 
      uint32_t r = payload[0];
      r += (uint32_t)payload[1]<<8;
      r += ((uint32_t)payload[2])<<16;
      r += ((uint32_t)payload[3])<<24;
      return r;
    };
    int32_t getLong() { return (int32_t)getUnsignedLong(); };
    float getFloat();

    void set(uint8_t b) { type = Vb_Byte; payload[0] = b; length = 5; };
    void set(uint16_t s) {
      type = Vb_UShort; payload[0] = s; payload[1] = s>>8; length = 6;
    };
    void set(uint32_t b) {
      type = Vb_ULong;
      payload[0] = b;
      payload[1] = b>>8;
      payload[2] = b>>16;
      payload[3] = b>>24;
      length = 8;
    };
    void set(int16_t s) { set((uint16_t)s); type = Vb_Short; };
    void set(int32_t s) { set((uint32_t)s); type = Vb_Long; };
    void set(float f);

    uint8_t pathA = 0;            // 0
    uint8_t pathB = 0;            // 1
    uint8_t pathC = 0;            // 2
    VanbusMsgType type = Vb_Byte; // 3
    uint8_t payload[4] = {0};     // 4..(7)
  private:
    uint8_t length = 0;
};

/*   0   1   2   3   4..n
 * +---+---+---+---+---+---+---+---+
 * | A | B | F | T |    Payload    |
 * +---+---+---+---+---+---+---+---+
 *
 * A - path element A
 * B - path element B
 * F - pathC
 * T - type
 * Payload - up to 4 bytes payload
 */
#define VANBUS_HEADER_LEN 4

typedef void (*callbackFn)(VanbusMsg *);

struct VanbusSub {
  uint8_t pathA = 0;
  uint8_t pathB = 0;
  uint8_t pathC = 0;
  callbackFn fn;
};

template <int N>
class Vanbus {
  public:
    // subscribe to a topic
    int subscribe(callbackFn fn, uint8_t A=0, uint8_t B=0, uint8_t F=0);

    // provide a new message for processing, call any registered callbacks
    void receive(uint8_t *bytes, uint8_t len);

  private:
    uint8_t n_subs = 0;
    VanbusSub subs[N];
    VanbusMsg msg;
};

template <int N>
int Vanbus<N>::subscribe(callbackFn fn, uint8_t A, uint8_t B, uint8_t C) {
  if (n_subs < VANBUS_MAX_SUBSCRIPTIONS) {
    subs[n_subs].pathA = A;
    subs[n_subs].pathB = B;
    subs[n_subs].pathC = C;
    subs[n_subs].fn = fn;
    n_subs++;
    return 0;
  } else {
    return -1;
  }
}

template <int N>
void Vanbus<N>::receive(uint8_t *bytes, uint8_t len) {
  msg.parseFromBytes(bytes, len);

  // check all subs
  for (uint8_t s=0; s<n_subs; s++) {
    if (msg.pathA != 0 && subs[s].pathA != msg.pathA) continue;
    if (msg.pathB != 0 && subs[s].pathB != msg.pathB) continue;
    if (msg.pathC != 0 && subs[s].pathC != msg.pathC) continue;

    // subscription matches, call callback
    subs[s].fn(&msg);
  }
}
