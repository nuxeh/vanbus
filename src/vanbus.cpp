#include <math.h>

#include "vanbus.h"

VanbusMsg::VanbusMsg() {}

VanbusMsg::VanbusMsg(uint8_t A, uint8_t B, uint8_t F) {
  pathA = A;
  pathB = B;
  field = F;
  type = Vb_Byte;
}

float fixed_to_float(vb_fixed_t input) {
  return ((float)input / (float)(1 << FIXED_POINT_FRACTIONAL_BITS));
}

vb_fixed_t float_to_fixed(float input) {
  return (vb_fixed_t)(round(input * (1 << FIXED_POINT_FRACTIONAL_BITS)));
}

int VanbusMsg::parseFromBytes(uint8_t *bytes, uint8_t len) {
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

int VanbusMsg::writeBytes(uint8_t *bytes, uint8_t max) {
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
  type = (F < 0) ? Vb_Fixed_Neg : Vb_Fixed;
  set(float_to_fixed(fabs(F)));
  length = VANBUS_HEADER_LEN + sizeof(vb_fixed_t);
}

float VanbusMsg::getFloat() {
  if (!(type == Vb_Fixed || type == Vb_Fixed_Neg)) return 0.0;

  vb_fixed_t r = payload[0];
  for (uint8_t i=1; i<sizeof(vb_fixed_t); i++) {
    r += payload[i];
  }

  return (type == Vb_Fixed_Neg) ? -1.0 * fixed_to_float(r) : fixed_to_float(r);
}

int Vanbus::subscribe(callbackFn fn, uint8_t A, uint8_t B, uint8_t F) {
  if (n_subs < VANBUS_MAX_SUBSCRIPTIONS) {
    subs[n_subs].pathA = A;
    subs[n_subs].pathB = B;
    subs[n_subs].field = F;
    subs[n_subs].fn = fn;
    n_subs++;
    return 0;
  } else {
    return -1;
  }
}

void Vanbus::receive(uint8_t *bytes, uint8_t len) {
  msg.parseFromBytes(bytes, len);

  // check all subs
  for (uint8_t s=0; s<n_subs; s++) {
    if (subs[s].pathA != 0 && subs[s].pathA != msg.getPathA()) continue;
    if (subs[s].pathB != 0 && subs[s].pathB != msg.getPathB()) continue;
    if (subs[s].field != 0 && subs[s].field != msg.getField()) continue;

    // subscription matches, call callback
    subs[s].fn(&msg);
  }
}
