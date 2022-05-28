#ifndef VANBUS_MAX_SUBSCRIPTIONS 5

enum VanbusMsgType {
  Vb_Byte,
  Vb_Float,
  Vb_Short,
  Vb_UShort,
  Vb_Long,
  Vb_ULong,
  Vb_Fixed,
};

class VanbusFixedPointNum {
  uint16_t value;
  uint16_t divisor;
};

class VanbusMsg {
  public:
    VanbusMsg();
    VanbusMsg(uint8_t A, uint8_t B, uint8_t F);

    void parseFromBytes(uint8_t *bytes, size_t len);
    int writeBytes(uint8_t *bytes, size_t max);

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

    void setByte(uint8_t byte) { type = Vb_Byte; payload[0] = byte };
    void setFloat();
    void setShort();
    void setUnsignedShort();
    void setLong();
    void setUnsignedLong();
    void setFixedPoint();

  private:
    uint8_t pathA = 0;
    uint8_t pathB = 0;
    uint8_t field = 0;
    VanbusMessageType type = Vb_Byte;
    uint8_t payload[4] = {0};
};

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
