#include <iostream>

#include <glog/logging.h>
#include <gflags/gflags.h>

#include "inc/training.h"
#include "inc/tagger.h"
#include "inc/character.h"

DEFINE_string(text, "", "");

int main(int argc, char **argv) {
  google::InitGoogleLogging(*argv);
  google::ParseCommandLineFlags(&argc, &argv, false);

  hmmsegmenter::Training training(FLAGS_training_file);

  training.Train();

  std::vector<hmmsegmenter::Character> chars;
  hmmsegmenter::Tagger::Tag(training.GetHmmModel(), FLAGS_text, &chars);

  for (size_t i = 0; i < chars.size(); ++i) {
    std::cout << chars[i].GetChracter();
    switch (chars[i].GetTag()) {
      case hmmsegmenter::Character::E:
      case hmmsegmenter::Character::S:
        std::cout << " ";
        break;
      default:
        break;
    }
  }

  return 0;
}