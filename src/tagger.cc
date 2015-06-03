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
  double *probabilities[4];
  for (int i = 0; i < 4; ++i) {
    probabilities[i] = new double[chars_count];
  }
  int *nodes[4];
  for (int i = 0; i < 4; ++i) {
    nodes[i] = new int[chars_count];
  }

  probabilities[0][0] = model.GetEmissionProbability(chars[0], HmmModel::B);
  probabilities[1][0] = -3.14e+100;
  probabilities[2][0] = -3.14e+100;
  probabilities[3][0] = model.GetEmissionProbability(chars[0], HmmModel::S);

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

  std::vector<HmmModel::Tag> tags;
  int last_pos = 0;
  if (probabilities[1][chars_count - 1] > probabilities[3][chars_count - 1]) {
    tags.push_back(HmmModel::E);
    last_pos =  nodes[1][chars_count - 1];
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
  std::reverse(tags.begin(), tags.end());

  for (int i = 0; i < chars_count; ++i) {
    std::cout << chars[i];
    if (tags[i] == HmmModel::E || tags[i] == HmmModel::S) {
      std::cout << " ";
    }
  }
  std::cout << std::endl;

  //for (int i = 0; i < 4; ++i) {
  //  for (int j = 0; j < chars_count; ++j) {
  //    std::cout << probabilities[i][j] << "\t";
  //  }
  //  std::cout << std::endl;
  //}
  //std::cout << std::endl;
  //
  //for (int i = 0; i < 4; ++i) {
  //  for (int j = 0; j < chars_count; ++j) {
  //    std::cout << nodes[i][j] << "\t";
  //  }
  //  std::cout << std::endl;
  //}
  //std::cout << std::endl;

  for (int i = 0; i < 4; ++i) {
    delete [] probabilities[i];
    delete [] nodes[i];
  }

  return false;
}

}  // namespace hmmsegmenter
