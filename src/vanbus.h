#pragma once

#include <stdint.h>

#ifndef VANBUS_MAX_SUBSCRIPTIONS
#define VANBUS_MAX_SUBSCRIPTIONS 5
#endif

typedef uint16_t vb_fixed_t;
#define FIXED_POINT_FRACTIONAL_BITS 5

enum VanbusMsgType {
  Vb_Byte,
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
    VanbusMsg(uint8_t A, uint8_t B, uint8_t F);

    int parseFromBytes(uint8_t *bytes, uint8_t len);
    int writeBytes(uint8_t *bytes, uint8_t max);

    uint8_t getLength() { return length; };
    VanbusMsgType getType() { return type; };
    void setType(VanbusMsgType T) { type = T; };

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

    uint8_t getPathA() { return pathA; };
    uint8_t getPathB() { return pathB; };
    uint8_t getField() { return field; };

    void setPathA(uint8_t A) { pathA = A; };
    void setPathB(uint8_t B) { pathB = B; };
    void setField(uint8_t F) { field = F; };

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

  private:
    uint8_t pathA = 0;                // 0
    uint8_t pathB = 0;                // 1
    uint8_t field = 0;                // 2
    VanbusMsgType type = Vb_Byte; // 3
    uint8_t payload[4] = {0};         // 4..(7)
    uint8_t length = 0;
};

/*   0   1   2   3   4..n
 * +---+---+---+---+---+---+---+---+
 * | A | B | F | T |    Payload    |
 * +---+---+---+---+---+---+---+---+
 *
 * A - path element A
 * B - path element B
 * F - field
 * T - type
 * Payload - up to 4 bytes payload
 */
#define VANBUS_HEADER_LEN 4

typedef void (*callbackFn)(VanbusMsg *);

struct VanbusSub {
  uint8_t pathA = 0;
  uint8_t pathB = 0;
  uint8_t field = 0;
  callbackFn fn;
};

class Vanbus {
  public:
    // subscribe to a topic
    int subscribe(callbackFn fn, uint8_t A=0, uint8_t B=0, uint8_t F=0);

    // provide a new message for processing, call any registered callbacks
    void receive(uint8_t *bytes, uint8_t len);

  private:
    uint8_t n_subs = 0;
    VanbusSub subs[VANBUS_MAX_SUBSCRIPTIONS];
    VanbusMsg msg;
};
