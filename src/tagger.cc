//
// Created by hudong on 6/2/15.
//

#include "inc/tagger.h"

#include <iostream>
#include <algorithm>



namespace hmmsegmenter {

bool Tagger::Tag(const HmmModel &model,
                 const std::string &text,
                 std::vector<Character> *chracters) {
  std::vector<std::string> chars;
  Utf8::GetUtf8Strings(text, &chars);

  const size_t chars_count = chars.size();
  double *probabilities[4];
  for (int i = 0; i < 4; ++i) {
    probabilities[i] = new double[chars_count];
  }
  int *nodes[4];
  for (int i = 0; i < 4; ++i) {
    nodes[i] = new int[chars_count];
  }

  probabilities[0][0] = model.GetTagFrequency(Character::B)
                        + model.GetEmissionProbability(chars[0], Character::B);
  probabilities[1][0] = std::numeric_limits<double>::min();
  probabilities[2][0] = std::numeric_limits<double>::min();
  probabilities[3][0] = model.GetTagFrequency(Character::S)
                        + model.GetEmissionProbability(chars[0], Character::S);

  nodes[0][0] = Character::B;
  nodes[1][0] = Character::E;
  nodes[2][0] = Character::M;
  nodes[3][0] = Character::S;

  for (int i = 1; i < chars_count; ++i) {
    for (uint8_t j = Character::B; j <= Character::S; ++j) {
      probabilities[j][i] = -3.14e+100;
      nodes[j][i] = Character::E;
      for (int k = 0; k < 4; ++k) {
        Character::Tag prev_tag = (Character::Tag) k;
        double transfer_from_k = probabilities[k][i - 1]
                                 + model.GetTransferProbability(prev_tag, (Character::Tag) j)
                                 + model.GetEmissionProbability(chars[i], (Character::Tag) j);
        if (transfer_from_k > probabilities[j][i]) {
          probabilities[j][i] = transfer_from_k;
          nodes[j][i] = k;
        }
      }
    }
  }

  std::vector<Character::Tag> tags;
  int previous_tag = Character::Z;
  if (probabilities[1][chars_count - 1] >= probabilities[3][chars_count - 1]) {
    tags.push_back(Character::E);
    previous_tag =  nodes[1][chars_count - 1];
  } else {
    tags.push_back(Character::S);
    previous_tag = nodes[3][chars_count - 1];
  }
  for (size_t i = chars_count - 2; i >= 0; --i) {
    tags.push_back((Character::Tag) previous_tag);
    if (i == 0) {
      break;
    }
    previous_tag = (Character::Tag) nodes[previous_tag][i];
  }
  std::reverse(tags.begin(), tags.end());

  chracters->reserve(chars_count);
  for (size_t i = 0; i < chars_count; ++i) {
    Character character(chars[i]);
    character.SetTag(tags[i]);
    chracters->push_back(character);
  }

  for (int i = 0; i < 4; ++i) {
    delete [] probabilities[i];
    delete [] nodes[i];
  }

  return false;
}

}  // namespace hmmsegmenter
