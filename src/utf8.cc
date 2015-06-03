//
// Created by hudong on 6/2/15.
//

#include "inc/utf8.h"

namespace hmmsegmenter {

Utf8::Utf8(const std::string &ascii)
    : ascii_(ascii) {
}

Utf8::~Utf8() {
}

uint8_t Utf8::GetByteCount(uint8_t byte) {
  if (byte < 0b10000000) {
    return 1;
  }

  if (byte < 0b11100000) {
    return 2;
  }

  if (byte < 0b11110000) {
    return 3;
  }

  return 4;
}

bool Utf8::GetUtf8Strings(std::vector<std::string> *chars) {
  size_t length = ascii_.length();

  for (size_t i = 0; i < length;) {
    uint8_t count = GetByteCount(ascii_[i]);
    if (i + count > length) {
      return false;
    }
    chars->push_back(ascii_.substr(i, count));
    i += count;
  }

  return true;
}

}  // namespace hmmsegmenter
