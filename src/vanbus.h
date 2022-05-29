#ifndef VANBUS_MAX_SUBSCRIPTIONS
#define VANBUS_MAX_SUBSCRIPTIONS 5
#endif

#define FIXED_POINT_FRACTIONAL_BITS 5

enum VanbusMsgType {
  Vb_Byte,
  Vb_Float,
  Vb_Short,
  Vb_UShort,
  Vb_Long,
  Vb_ULong,
};

class VanbusMsg {
  public:
    VanbusMsg();
    VanbusMsg(uint8_t A, uint8_t B, uint8_t F);

    int parseFromBytes(uint8_t *bytes, size_t len);
    int writeBytes(uint8_t *bytes, size_t max);

    uint8_t getLength() { return length };
    VanbusMessageType getType( return type; );
    void setType(VanbusMessageType T) { type = T };

    uint8_t getByte() { return payload[0] };
    float getFloat();
    int16_t getShort();
    uint16_t getUnsignedShort();
    int32_t getLong();
    uint32_t getUnsignedLong();
    VanbusFixedPointNum getFixedPoint();

    void setPathA(uint8_t A) { pathA = A; };
    void setPathB(uint8_t B) { pathB = B; };
    void setField(uint8_t F) { field = F; };

    void set(uint8_t b) { type = Vb_Byte; payload[0] = b; length = 5; };
    void set(uint16_t b) {
      type = Vb_UShort; payload[0] = b; payload[1] = b>>8; length = 6;
    };
    void set(uint32_t b) {
      type = Vb_ULong;
      payload[0] = b;
      payload[1] = b>>8;
      payload[2] = b>>16;
      payload[3] = b>>24;
      length = 8;
    };

  private:
    uint8_t pathA = 0;                // 0
    uint8_t pathB = 0;                // 1
    uint8_t field = 0;                // 2
    VanbusMessageType type = Vb_Byte; // 3
    uint8_t payload[4] = {0};         // 4..(7)
    uint8_t length = 0;
};

/*   0   1   2   3   4   5   6   7
 * +---+---+---+---+---+---+---+---+
 * | A | B | F | T |    Payload    |
 * +---+---+---+---+---+---+---+---+
 */
#define VANBUS_HEADER_LEN 4

typedef void (*callbackFn)(*VanBusMsg);

struct VanbusSub {
  uint8_t pathA = 0;
  uint8_t pathB = 0;
  uint8_t field = 0;
  callbackFn fn;
};

class Vanbus {
  public:
    // subscribe to a topic
    int subscribe(callbackFn F, uint8_t A, uint8_t B, uint8_t F=0);

    // provide a new message for processing, call any registered callbacks
    int receive(uint8_t bytes, uint8_t len);

  private:
    uint8_t n_subs = 0;
    VanbusSub subs[VANBUS_MAX_SUBSCRIPTIONS];
    VanbusMsg msg;
};
