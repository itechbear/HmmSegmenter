//
// Created by hudong on 6/2/15.
//

#ifndef HMMSEGMENTER_STRING_H_
#define HMMSEGMENTER_STRING_H_

#include <cstddef>
#include <string>
#include <vector>

namespace hmmsegmenter {

class StringUtil {
 public:
  static size_t Tokenize(const std::string &line,
                         const std::string &delimeters,
                         std::vector<std::string> *tokens);
};

}  // namespace hmmsegmenter

#endif //HMMSEGMENTER_STRING_H_
