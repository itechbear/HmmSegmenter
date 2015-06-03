//
// Created by hudong on 6/2/15.
//

#include "inc/training.h"

#include <glog/logging.h>

#include "inc/string_util.h"
#include "inc/utf8.h"

DEFINE_string(training_file, "", "");

namespace hmmsegmenter {

Training::Training(const std::string &path)
    : file_reader_(path), hmm_model_() {
}

Training::~Training() {
}

bool Training::Train() {
  if (!file_reader_.IsOk()) {
    LOG(ERROR) << "Failed to open training file!";
    return false;
  }

  std::string line;
  while (file_reader_.ReadLine(&line)) {
    std::vector<std::string> tokens;
    StringUtil::Tokenize(line, " \t", &tokens);
    for (size_t i = 0; i < tokens.size(); ++i) {
      const std::string token = tokens[i];
      std::vector<std::string> characters;
      Utf8 utf8(token);
      utf8.GetUtf8Strings(&characters);
      const size_t size = characters.size();
      if (size == 1) {
        hmm_model_.AddTag(HmmModel::S);
        hmm_model_.AddCharacter(characters[0]);
        hmm_model_.AddCondition(HmmModel::S, characters[0]);
      } else {
        const size_t count = characters.size();
        for (size_t i = 0; i < count; ++i) {
          HmmModel::Tag tag;
          if (i == 0) {
            tag = HmmModel::B;
          } else if (i == count - 1) {
            tag = HmmModel::E;
          } else {
            tag = HmmModel::M;
          }
          hmm_model_.AddTag(tag);
          hmm_model_.AddCharacter(characters[i]);
          hmm_model_.AddCondition(tag, characters[i]);
        }
      }
    }
  }

  hmm_model_.Calculate();

  return true;
}

const HmmModel &Training::GetHmmModel() const {
  return hmm_model_;
}

}  // namespace hmmsegmenter
