//
// Created by hudong on 6/2/15.
//

#ifndef HMMSEGMENTER_TAGGER_H_
#define HMMSEGMENTER_TAGGER_H_

#include "inc/training.h"
#include "inc/utf8.h"

namespace hmmsegmenter {

class Tagger {
 public:
  static bool Tag(const HmmModel &model,
                  const std::string &text,
                  std::vector<Character> *chracters);
};

}  // namespace hmmsegmenter

#endif // HMMSEGMENTER_TAGGER_H_
