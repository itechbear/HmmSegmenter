//
// Created by hudong on 6/2/15.
//

#include "inc/hmm_model.h"

#include <cmath>

#include <glog/logging.h>

namespace hmmsegmenter {

HmmModel::HmmModel() {
  transfer_matrix_["0:0"] = -3.14e+100;
  transfer_matrix_["0:1"] = -0.510825623765990;
  transfer_matrix_["0:2"] = -0.916290731874155;
  transfer_matrix_["0:3"] = -3.14e+100;

  transfer_matrix_["1:0"] = -0.5897149736854513;
  transfer_matrix_["1:1"] = -3.14e+100;
  transfer_matrix_["1:2"] = -3.14e+100;
  transfer_matrix_["1:3"] = -0.8085250474669937;

  transfer_matrix_["2:0"] = -3.14e+100;
  transfer_matrix_["2:1"] = -0.33344856811948514;
  transfer_matrix_["2:2"] = -1.2603623820268226;
  transfer_matrix_["2:3"] = -3.14e+100;

  transfer_matrix_["3:0"] = -0.7211965654669841;
  transfer_matrix_["3:1"] = -3.14e+100;
  transfer_matrix_["3:2"] = -3.14e+100;
  transfer_matrix_["3:3"] = -0.6658631448798212;
}

HmmModel::~HmmModel() {
}

double HmmModel::GetTransferProbability(HmmModel::Tag first,
                                        HmmModel::Tag second) const {
  const std::string key = std::to_string(first) + ":" + std::to_string(second);
  return transfer_matrix_.find(key)->second;
}

double HmmModel::GetEmissionProbability(const std::string &character, HmmModel::Tag tag) const {
  const std::string key = character + ":" + std::to_string(tag);
  std::unordered_map<std::string, double>::const_iterator iterator = emission_matrix_.find(key);
  if (iterator == emission_matrix_.end()) {
    return (tag == S) ? 0 : -3.14e+100;
  }
  return iterator->second;
}

void HmmModel::AddCharacter(const std::string &character) {
  std::unordered_map<std::string, uint32_t>::iterator iterator = characters_.find(character);

  if (iterator == characters_.end()) {
    characters_[character] = 1;
    return;
  }

  ++characters_[character];
}

void HmmModel::AddTag(Tag tag) {
  std::map<Tag, uint32_t>::iterator iterator = tags_.find(tag);

  if (iterator == tags_.end()) {
    tags_[tag] = 1;
    return;
  }

  ++tags_[tag];
}

void HmmModel::AddCondition(HmmModel::Tag tag,
                            const std::string &character) {
  const std::string key = std::to_string(tag) + ":" + character;
  std::unordered_map<std::string, uint32_t>::iterator iterator = conditions_.find(key);

  if (iterator == conditions_.end()) {
    conditions_[key] = 1;
    return;
  }

  ++conditions_[key];
}

void HmmModel::Calculate() {
  LOG(INFO) << "Begin to calculate the hmm model!";
  LOG(INFO) << "characters_ size: " << characters_.size();
  LOG(INFO) << "conditions_ size: " << conditions_.size();

  uint64_t tags_count = 0;
  for (std::map<Tag, uint32_t>::const_iterator iterator = tags_.begin();
       iterator != tags_.end();
       ++iterator) {
    tags_count += iterator->second;
  }

  uint64_t characters_count = 0;
  for (std::unordered_map<std::string, uint32_t>::const_iterator iterator = characters_.begin();
       iterator != characters_.end();
       ++iterator) {
    characters_count += iterator->second;
  }

  uint64_t condition_count = 0;
  for (std::unordered_map<std::string, uint32_t>::const_iterator iterator = conditions_.begin();
       iterator != conditions_.end();
       ++iterator) {
    condition_count += iterator->second;
  }

  LOG(INFO) << "tags_count: " << tags_count;
  LOG(INFO) << "characters_count: " << characters_count;
  LOG(INFO) << "condition_count: " << condition_count;

  std::map<Tag, double> tag_frequency;
  for (std::map<Tag, uint32_t>::const_iterator const_iterator = tags_.begin();
       const_iterator != tags_.end();
       ++const_iterator) {
    tag_frequency[const_iterator->first] = tags_[const_iterator->first] / (double) tags_count;
  }

  std::map<std::string, double> char_frequency;
  for (std::unordered_map<std::string, uint32_t>::const_iterator const_iterator = characters_.begin();
       const_iterator != characters_.end();
       ++const_iterator) {
    char_frequency[const_iterator->first] = characters_[const_iterator->first] / (double) characters_count;
  }

  std::map<std::string, double> cond_frequency;
  for (std::unordered_map<std::string, uint32_t>::const_iterator const_iterator = conditions_.begin();
       const_iterator != conditions_.end();
       ++const_iterator) {
    cond_frequency[const_iterator->first] = conditions_[const_iterator->first] / (double) condition_count;
  }

  for (std::unordered_map<std::string, uint32_t>::const_iterator char_iterator = characters_.begin();
       char_iterator != characters_.end();
       ++char_iterator) {
    for (uint8_t tag = B; tag <= S; ++tag) {
      const std::string cond_key = std::to_string(tag) + ":" + char_iterator->first;
      const std::string emit_key = char_iterator->first + ":" + std::to_string(tag);
      if (cond_frequency.find(cond_key) == cond_frequency.end()) {
        emission_matrix_[emit_key] = -3.14e+100;
      } else {
        double char_freq = char_frequency[char_iterator->first];
        double cond_freq = cond_frequency[cond_key];
        double tag_freq = tag_frequency[(HmmModel::Tag) tag];
        emission_matrix_[emit_key] = char_frequency[char_iterator->first] * cond_frequency[cond_key] / tag_frequency[(HmmModel::Tag) tag];
      }
      emission_matrix_[emit_key] = std::log2(emission_matrix_[emit_key]);
    }
  }

  LOG(INFO) << "Calculation finishes.";
}

}  // namespace hmmsegmenter
