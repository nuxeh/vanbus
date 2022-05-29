#include <math.h>

#include "vanbus.h"

VanbusMsg::VanbusMsg() {}

VanbusMsg::VanbusMsg(uint8_t A, uint8_t B, uint8_t F) {
  pathA = A;
  pathB = B;
  field = F;
  type = T;
  payload = P;
}

float fixed_to_float(uint16_t input) {
  return ((float)input / (float)(1 << FIXED_POINT_FRACTIONAL_BITS));
}

uint16_t float_to_fixed(float input) {
  return (uint16_t)(round(input * (1 << FIXED_POINT_FRACTIONAL_BITS)));
}

int VanbusMsg::parseFromBytes(uint8_t *bytes, size_t len) {
  if (len >= 5) {
    pathA = bytes[0];
    pathB = bytes[1];
    field = bytes[2];
    type = (VanbusMsgType)bytes[3];
    for (uint8_t i=0; i<4; i++) {
      payload[i] = bytes[i + 4];
    }
    return 0;
  } else {
    return -1;
  }
}

int VanbusMsg::writeBytes(uint8_t *bytes, size_t max) {
  bytes[0] = pathA;
  bytes[1] = pathB;
  bytes[2] = field;
  type = (VanbusMsgType)bytes[3];
  for (uint8_t i=0; i<4; i++) {
    bytes[i + 4] = payload[i];
  }
  return 0;
}

int Vanbus::subscribe(callbackFn F, uint8_t A, uint8_t B, uint8_t F=0) {
  if (n_subs < VANBUS_MAX_SUBSCRIPTIONS) {
    subs[n_subs].pathA = A;
    subs[n_subs].pathB = B;
    subs[n_subs].field = F;
    subs[n_subs].fn = F;
    n_subs++;
    return 0;
  } else {
    return -1;
  }
}

int Vanbus::receive(uint8_t bytes, uint8_t len) {
  msg.parseFromBytes(bytes, len);
}
