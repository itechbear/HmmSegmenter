//
// Created by hudong on 6/2/15.
//

#include "inc/training.h"

#include <glog/logging.h>

#include "inc/string_util.h"
#include "inc/utf8.h"
#include "inc/character.h"

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
  int last_tag = -1;
  while (file_reader_.ReadLine(&line)) {
    std::vector<std::string> tokens;
    StringUtil::Tokenize(line, " \t", &tokens);
    for (size_t i = 0; i < tokens.size(); ++i) {
      const std::string token = tokens[i];
      std::vector<std::string> characters;
      Utf8::GetUtf8Strings(token, &characters);
      const size_t size = characters.size();
      if (size == 1) {
        Character::Tag current_tag = Character::S;
        hmm_model_.AddTag(current_tag);
        hmm_model_.AddCharacter(characters[0]);
        hmm_model_.AddCharacterCondition(Character::S, characters[0]);
        if (last_tag != -1) {
          hmm_model_.AddTagCondition((Character::Tag) last_tag, current_tag);
        }
        last_tag = current_tag;
      } else {
        const size_t count = characters.size();
        for (size_t i = 0; i < count; ++i) {
          Character::Tag current_tag;
          if (i == 0) {
            current_tag = Character::B;
          } else if (i == count - 1) {
            current_tag = Character::E;
          } else {
            current_tag = Character::M;
          }
          hmm_model_.AddTag(current_tag);
          hmm_model_.AddCharacter(characters[i]);
          hmm_model_.AddCharacterCondition(current_tag, characters[i]);
          if (last_tag != -1) {
            hmm_model_.AddTagCondition((Character::Tag) last_tag, current_tag);
          }
          last_tag = current_tag;
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
