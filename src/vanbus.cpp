VanbusMsg::VanbusMsg() {}

VanbusMsg::VanbusMsg(uint8_t A, uint8_t B, uint8_t F) {
  pathA = A;
  pathB = B;
  field = F;
  type = T;
  payload = P;
}

void VanbusMsg::parseFromBytes(uint8_t *bytes, size_t len) {

}

int VanbusMsg::writeBytes(uint8_t *bytes, size_t max) {

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
