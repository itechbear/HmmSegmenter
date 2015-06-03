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
  explicit Utf8(const std::string &ascii);

  ~Utf8();

  static uint8_t GetByteCount(uint8_t byte);

  bool GetUtf8Strings(std::vector<std::string> *chars);

 private:
  const std::string ascii_;
};

}  // namespace hmmsegmenter

#endif //HMMSEGMENTER_UTF8_H_
