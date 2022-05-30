#include "vanbus.h"

VanbusMsg::VanbusMsg() : VanbusMsg(0, 0, 0) {}

VanbusMsg::VanbusMsg(uint8_t A, uint8_t B, uint8_t C) {
  pathA = A;
  pathB = B;
  pathC = C;
  type = Vb_Byte;
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
  uint32_t *long_ptr = (uint32_t*)&F;
  set(*long_ptr);
  type = Vb_Float;
}

float VanbusMsg::getFloat() {
  float *float_ptr = (float*)&payload;
  return *float_ptr;
}
