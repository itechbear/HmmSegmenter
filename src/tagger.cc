//
// Created by hudong on 6/2/15.
//

#include "inc/tagger.h"

#include <iostream>
#include <algorithm>

#include "inc/utf8.h"

namespace hmmsegmenter {

bool Tagger::Segment(const HmmModel &model,
                     const std::string &text,
                     std::vector<std::string> *tokens) {
  Utf8 utf(text);
  std::vector<std::string> chars;
  utf.GetUtf8Strings(&chars);

  const size_t chars_count = chars.size();
  double *weights[4];
  for (int i = 0; i < 4; ++i) {
    weights[i] = new double[chars.size()];
  }
  int *nodes[4];
  for (int i = 0; i < 4; ++i) {
    nodes[i] = new int[chars.size()];
  }

  weights[0][0] = model.GetEmissionProbability(chars[0], HmmModel::B);
  weights[1][0] = model.GetEmissionProbability(chars[0], HmmModel::B);
  weights[2][0] = model.GetEmissionProbability(chars[0], HmmModel::B);
  weights[3][0] = model.GetEmissionProbability(chars[0], HmmModel::B);

  nodes[0][0] = HmmModel::B;
  nodes[1][0] = HmmModel::B;
  nodes[2][0] = HmmModel::B;
  nodes[3][0] = HmmModel::B;

  for (int i = 1; i < chars_count; ++i) {
    for (int j = 0; j < 4; ++j) {
      HmmModel::Tag cur_tag = (HmmModel::Tag) j;
      weights[j][i] = -3.14e+100;
      nodes[j][i] = HmmModel::E;
      for (int k = 0; k < 4; ++k) {
        HmmModel::Tag prev_tag = (HmmModel::Tag) k;
        double transfer_from_k = weights[k][i - 1] + model.GetTransferProbability(prev_tag, cur_tag) + model.GetEmissionProbability(chars[i], cur_tag);
        if (transfer_from_k > weights[j][i]) {
          weights[j][i] = transfer_from_k;
          nodes[j][i] = k;
        }
      }
    }
  }

  std::vector<HmmModel::Tag> tags;
  size_t last_pos = 0;
  if (weights[1][chars_count - 1] > weights[3][chars_count - 1]) {
    tags.push_back(HmmModel::E);
    last_pos =  nodes[0][chars_count - 1];
  } else {
    tags.push_back(HmmModel::S);
    last_pos = nodes[3][chars_count - 1];
  }
  for (size_t i = chars_count - 2; i >= 0; --i) {
    tags.push_back((HmmModel::Tag) last_pos);
    if (i == 0) {
      break;
    }
    last_pos = (HmmModel::Tag) nodes[last_pos][i];
  }

  for (int i = 0; i < tags.size(); ++i) {
    std::cout << tags[i];
  }
  std::cout << std::endl;

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < chars_count; ++j) {
      std::cout << weights[i][j] << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < chars_count; ++j) {
      std::cout << nodes[i][j] << "\t";
    }
    std::cout << std::endl;
  }

  for (int i = 0; i < 4; ++i) {
    delete [] weights[i];
    delete [] nodes[i];
  }

  return false;
}

}  // namespace hmmsegmenter
