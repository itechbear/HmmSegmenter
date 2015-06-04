//
// Created by hudong on 6/2/15.
//

#include "inc/utf8.h"

namespace hmmsegmenter {

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

bool Utf8::GetUtf8Strings(const std::string &text,
                          std::vector<std::string> *chars) {
  size_t length = text.length();

  for (size_t i = 0; i < length;) {
    uint8_t count = GetByteCount(text[i]);
    if (i + count > length) {
      return false;
    }
    chars->push_back(text.substr(i, count));
    i += count;
  }

  return true;
}

}  // namespace hmmsegmenter
