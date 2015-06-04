//
// Created by hudong on 6/2/15.
//

#ifndef HMMSEGMENTER_UTF8_H_
#define HMMSEGMENTER_UTF8_H_

#include <vector>
#include <string>

namespace hmmsegmenter {

class Utf8 {
 public:
  static uint8_t GetByteCount(uint8_t byte);

  static bool GetUtf8Strings(const std::string &text,
                             std::vector<std::string> *chars);
};

}  // namespace hmmsegmenter

#endif // HMMSEGMENTER_UTF8_H_
