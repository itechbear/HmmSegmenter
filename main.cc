#include <iostream>

#include <glog/logging.h>
#include <gflags/gflags.h>

#include "inc/training.h"
#include "inc/tagger.h"

DEFINE_string(text, "", "");

int main(int argc, char **argv) {
  google::InitGoogleLogging(*argv);
  google::ParseCommandLineFlags(&argc, &argv, false);

  hmmsegmenter::Training training(FLAGS_training_file);

  training.Train();

  std::vector<hmmsegmenter::HmmModel::Tag> tags;
  hmmsegmenter::Tagger::Tag(training.GetHmmModel(), FLAGS_text, &tags);

  return 0;
}