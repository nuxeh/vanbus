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
    VanbusMsg(uint8_t pathA, uint8_t pathB, uint8_t field, VanbusMessageType type=Vb_Byte, payload=0);

    void parseFromBytes(uint8_t *bytes, size_t len);
    int writeBytes(uint8_t *bytes, size_t max);

    void getType();
    void setType();

    uint8_t getByte();
    float getFloat();
    int16_t getShort();
    uint16_t getUnsignedShort();
    int32_t getLong();
    uint32_t getUnsignedLong();
    VanbusFixedPointNum getFixedPoint();

    void getByte();
    void getFloat();
    void getShort();
    void getUnsignedShort();
    void getLong();
    void getUnsignedLong();
    void getFixedPoint();

  private:
    uint8_t pathA = 0;
    uint8_t pathB = 0;
    uint8_t field = 0;
    VanbusMessageType type = Vb_Byte;
    uint32_t payload = 0;
};

typedef void (*callbackFn)(*VanBusMsg);

struct VanbusCallback {
  uint8_t pathA = 0;
  uint8_t pathB = 0;
  uint8_t field = 0;
  callbackFn callback;
};

class Vanbus {
  public:
    int begin(int pin);
    int subscribe(callbackFn F, uint8_t A, uint8_t B, uint8_t F=0);

    int update();

  private:
    VanbusCallback callbacks[VANBUS_MAX_SUBSCRIPTIONS];
    VanbusMsg msg;
    PJONSoftwareBitBang bus;
};
