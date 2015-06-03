//
// Created by hudong on 6/2/15.
//

#ifndef HMMSEGMENTER_TRAINING_H_
#define HMMSEGMENTER_TRAINING_H_

#include <gflags/gflags.h>

#include "inc/file_reader.h"
#include "inc/hmm_model.h"

DECLARE_string(training_file);

namespace hmmsegmenter {

class Training {
 public:
  explicit Training(const std::string &path);

  ~Training();

  bool Train();

  const HmmModel &GetHmmModel() const;

 private:
  FileReader file_reader_;
  HmmModel hmm_model_;
};

}  // namespace hmmsegmenter

#endif //HMMSEGMENTER_TRAINING_H_
