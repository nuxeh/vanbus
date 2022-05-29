#include <math.h>

#include "vanbus.h"

VanbusMsg::VanbusMsg() : VanbusMsg(0, 0, 0) {}

VanbusMsg::VanbusMsg(uint8_t A, uint8_t B, uint8_t C) {
  pathA = A;
  pathB = B;
  pathC = C;
  type = Vb_Byte;
}

float fixed_to_float(vb_fixed_t input) {
  return ((float)input / (float)(1 << FIXED_POINT_FRACTIONAL_BITS));
}

vb_fixed_t float_to_fixed(float input) {
  return (vb_fixed_t)(round(input * (1UL << FIXED_POINT_FRACTIONAL_BITS)));
}

uint8_t get_length_for_type() {
  // TODO
}

int VanbusMsg::parseFromBytes(uint8_t *bytes, uint8_t len) {
  if (len >= 5) {
    pathA = bytes[0];
    pathB = bytes[1];
    pathC = bytes[2];
    type = (VanbusMsgType)bytes[3];
    int read = VANBUS_HEADER_LEN;
    for (uint8_t i=0; (i+VANBUS_HEADER_LEN)<len; i++) {
      if (read > len) break;
      payload[i] = bytes[i+VANBUS_HEADER_LEN];
      read++;
    }
    return read;
  } else {
    return -1;
  }
}

int VanbusMsg::writeBytes(uint8_t *bytes, uint8_t max) {
  bytes[0] = pathA;
  bytes[1] = pathB;
  bytes[2] = pathC;
  bytes[3] = (uint8_t)type;
  int written = VANBUS_HEADER_LEN;
  for (uint8_t i=0; i<(length-VANBUS_HEADER_LEN); i++) {
    if (written >= max) return -1;
    bytes[i+VANBUS_HEADER_LEN] = payload[i];
    written++;
  }
  return written;
}

void VanbusMsg::set(float F) {
  set(float_to_fixed(fabs(F)));
  type = (F < 0.0) ? Vb_Fixed_Neg : Vb_Fixed;
  length = VANBUS_HEADER_LEN + sizeof(vb_fixed_t);
}

float VanbusMsg::getFloat() {
  if (!(type == Vb_Fixed || type == Vb_Fixed_Neg)) return 0.1;

  vb_fixed_t r = payload[0];
  for (uint8_t i=1; i<sizeof(vb_fixed_t); i++) {
    r += ((vb_fixed_t)payload[i]) << (8*i);
  }

  return (type == Vb_Fixed_Neg) ? -1.0 * fixed_to_float(r) : fixed_to_float(r);
}
