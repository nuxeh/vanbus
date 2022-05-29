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

float fixed_to_float(vb_fixed_t input) {
  return ((float)input / (float)(1 << FIXED_POINT_FRACTIONAL_BITS));
}

vb_fixed_t float_to_fixed(float input) {
  return (vb_fixed_t)(round(input * (1 << FIXED_POINT_FRACTIONAL_BITS)));
}

int VanbusMsg::parseFromBytes(uint8_t *bytes, size_t len) {
  if (len >= 5) {
    pathA = bytes[0];
    pathB = bytes[1];
    field = bytes[2];
    type = (VanbusMsgType)bytes[3];
    for (uint8_t i=0; (i+VANBUS_HEADER_LEN)<len; i++) {
      payload[i] = bytes[i+VANBUS_HEADER_LEN];
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
  for (uint8_t i=0; i<(length-VANBUS_HEADER_LEN); i++) {
    if ((i+VANBUS_HEADER_LEN) >= max) return -1;
    bytes[i+VANBUS_HEADER_LEN] = payload[i];
  }
  return 0;
}

void VanbusMsg::set(float F) {
  sign = (F < 0); 
  set(float_to_fixed(abs(F));
  length = sizeof(fixed_t) + VANBUS_HEADER_LEN + 1;
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

  // check all subs
  for (uint8_t s=0; s<n_subs; s++) {
    if (subs[s].pathA != 0 && subs[s].pathA != msg.pathA) continue;
    if (subs[s].pathB != 0 && subs[s].pathB != msg.pathB) continue;
    if (subs[s].field != 0 && subs[s].field != msg.field) continue;

    // subscription matches, call callback
    subs[s].fn(&msg);
  }
}
