//
// Created by hudong on 6/2/15.
//

#include "inc/tagger.h"

#include <iostream>
#include <algorithm>



namespace hmmsegmenter {

bool Tagger::Tag(const HmmModel &model,
                 const std::string &text,
                 std::vector<HmmModel::Tag> *tags) {
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

  probabilities[0][0] = model.GetTagFrequency(HmmModel::B)
                        + model.GetEmissionProbability(chars[0], HmmModel::B);
  probabilities[1][0] = std::numeric_limits<double>::min();
  probabilities[2][0] = std::numeric_limits<double>::min();
  probabilities[3][0] = model.GetTagFrequency(HmmModel::S)
                        + model.GetEmissionProbability(chars[0], HmmModel::S);

  nodes[0][0] = HmmModel::B;
  nodes[1][0] = HmmModel::E;
  nodes[2][0] = HmmModel::M;
  nodes[3][0] = HmmModel::S;

  for (int i = 1; i < chars_count; ++i) {
    for (uint8_t j = HmmModel::B; j <= HmmModel::S; ++j) {
      probabilities[j][i] = -3.14e+100;
      nodes[j][i] = HmmModel::E;
      for (int k = 0; k < 4; ++k) {
        HmmModel::Tag prev_tag = (HmmModel::Tag) k;
        double transfer_from_k = probabilities[k][i - 1]
                                 + model.GetTransferProbability(prev_tag, (HmmModel::Tag) j)
                                 + model.GetEmissionProbability(chars[i], (HmmModel::Tag) j);
        if (transfer_from_k > probabilities[j][i]) {
          probabilities[j][i] = transfer_from_k;
          nodes[j][i] = k;
        }
      }
    }
  }

  int previous_tag = HmmModel::Z;
  if (probabilities[1][chars_count - 1] >= probabilities[3][chars_count - 1]) {
    tags->push_back(HmmModel::E);
    previous_tag =  nodes[1][chars_count - 1];
  } else {
    tags->push_back(HmmModel::S);
    previous_tag = nodes[3][chars_count - 1];
  }
  for (size_t i = chars_count - 2; i >= 0; --i) {
    tags->push_back((HmmModel::Tag) previous_tag);
    previous_tag = (HmmModel::Tag) nodes[previous_tag][i];
  }
  std::reverse(tags->begin(), tags->end());

  for (int i = 0; i < 4; ++i) {
    delete [] probabilities[i];
    delete [] nodes[i];
  }

  return false;
}

}  // namespace hmmsegmenter
