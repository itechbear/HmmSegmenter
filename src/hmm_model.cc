//
// Created by hudong on 6/2/15.
//

#include "inc/hmm_model.h"

#include <cmath>

#include <glog/logging.h>
#include <limits>

namespace hmmsegmenter {

HmmModel::HmmModel() : transfer_matrix_(),
                       emission_matrix_(),
                       character_conditions_(),
                       tag_conditions_(),
                       characters_(),
                       cleared_(false) {
}

HmmModel::~HmmModel() {
}

void HmmModel::AddCharacter(const std::string &character) {
  auto const_iterator = characters_.find(character);

  if (const_iterator == characters_.end()) {
    characters_[character] = 1;
    return;
  }

  ++characters_[character];
}

void HmmModel::AddTag(Tag tag) {
  auto const_iterator = tags_.find(tag);

  if (const_iterator == tags_.end()) {
    tags_[tag] = 1;
    return;
  }

  ++tags_[tag];
}

void HmmModel::AddCharacterCondition(const Tag tag,
                                     const std::string &character) {
  auto outer_iterator = character_conditions_.find(tag);
  if (outer_iterator == character_conditions_.end()) {
    character_conditions_[tag][character] = 1;
    return;
  }
  auto inner_iterator = outer_iterator->second.find(character);
  if (inner_iterator == outer_iterator->second.end()) {
    character_conditions_[tag][character] = 1;
    return;
  }
  ++character_conditions_[tag][character];
}

void HmmModel::AddTagCondition(const Tag previous,
                               const Tag current) {
  auto outer_iterator = tag_conditions_.find(previous);
  if (outer_iterator == tag_conditions_.end()) {
    tag_conditions_[previous][current] = 1;
    return;
  }
  auto inner_iterator = outer_iterator->second.find(current);
  if (inner_iterator == outer_iterator->second.end()) {
    tag_conditions_[previous][current] = 1;
    return;
  }
  ++tag_conditions_[previous][current];
}

void HmmModel::Calculate() {
  LOG(INFO) << "Begin to calculate the hmm model!";

  uint64_t tags_count = 0;
  for (auto iterator = tags_.begin();
       iterator != tags_.end();
       ++iterator) {
    tags_count += iterator->second;
  }

  uint64_t characters_count = 0;
  for (auto const_iterator = characters_.begin();
       const_iterator != characters_.end();
       ++const_iterator) {
    characters_count += const_iterator->second;
  }

  uint64_t character_condition_count = 0;
  for (auto outer_iterator = character_conditions_.begin();
       outer_iterator != character_conditions_.end();
       ++outer_iterator) {
    for (auto inner_iterator = outer_iterator->second.begin();
         inner_iterator != outer_iterator->second.end();
         ++inner_iterator) {
      character_condition_count += inner_iterator->second;
    }
  }

  uint64_t tag_condition_count = 0;
  for (auto outer_iterator = tag_conditions_.begin();
       outer_iterator != tag_conditions_.end();
       ++outer_iterator) {
    for (auto inner_iterator = outer_iterator->second.begin();
         inner_iterator != outer_iterator->second.end();
         ++inner_iterator) {
      tag_condition_count += inner_iterator->second;
    }
  }

  for (auto const_iterator = tags_.begin();
       const_iterator != tags_.end();
       ++const_iterator) {
    tag_frequency_[const_iterator->first] = tags_[const_iterator->first] / (double) tags_count;
  }

  std::map<std::string, double> character_frequency;
  for (auto const_iterator = characters_.begin();
       const_iterator != characters_.end();
       ++const_iterator) {
    character_frequency[const_iterator->first] = characters_[const_iterator->first] / (double) characters_count;
  }

  std::unordered_map<uint8_t, std::unordered_map<std::string, double>> character_condition_frequency;
  for (auto outer_iterator = character_conditions_.begin();
       outer_iterator != character_conditions_.end();
       ++outer_iterator) {
    for (auto inner_iterator = outer_iterator->second.begin();
         inner_iterator != outer_iterator->second.end();
         ++inner_iterator) {
      character_condition_frequency[outer_iterator->first][inner_iterator->first] = inner_iterator->second / (double) character_condition_count;
    }
  }

  for (auto character_iterator = characters_.begin();
       character_iterator != characters_.end();
       ++character_iterator) {
    for (uint8_t tag = B; tag <= S; ++tag) {
      if (character_condition_frequency.find(tag) == character_condition_frequency.end() 
          || character_condition_frequency[tag].find(character_iterator->first) == character_condition_frequency[tag].end()) {
        emission_matrix_[character_iterator->first][tag] = -19.9315685693;
      } else {
        emission_matrix_[character_iterator->first][tag] = character_frequency[character_iterator->first] * character_condition_frequency[tag][character_iterator->first] / tag_frequency_[(Tag) tag];
        emission_matrix_[character_iterator->first][tag] = std::log2(emission_matrix_[character_iterator->first][tag]);
      }
    }
  }

  for (uint8_t previous = B; previous <= S; ++previous) {
    for (uint8_t current = B; current <= S; ++current) {
      if (tag_conditions_.find(previous) != tag_conditions_.end() 
          && tag_conditions_[previous].find(current) != tag_conditions_[previous].end()) {
        transfer_matrix_[previous][current] = std::log2(tag_conditions_[previous][current] / (double) tag_condition_count);
      }
    }
  }

  LOG(INFO) << "Calculation finishes.";
}

double HmmModel::GetTransferProbability(const Tag previous,
                                        const Tag current) const {
  auto outer_iterator = transfer_matrix_.find(previous);
  if (outer_iterator == transfer_matrix_.end()) {
    return -19.9315685693;
  }
  auto inner_iterator = outer_iterator->second.find(current);
  if (inner_iterator == outer_iterator->second.end()) {
    return -19.9315685693;
  }
  return inner_iterator->second;
}

double HmmModel::GetEmissionProbability(const std::string &character,
                                        const Tag tag) const {
  auto outer_iterator = emission_matrix_.find(character);
  if (outer_iterator == emission_matrix_.end()) {
    return (tag == S) ? 0 : -19.9315685693;
  }
  auto inner_iterator = outer_iterator->second.find(tag);
  if (inner_iterator == outer_iterator->second.end()) {
    return (tag == S) ? 0 : -19.9315685693;
  }
  return inner_iterator->second;
}

void HmmModel::Clear() {
  character_conditions_.clear();
  tag_conditions_.clear();
  characters_.clear();
  tags_.clear();
  cleared_ = true;
}

double HmmModel::GetTagFrequency(const HmmModel::Tag tag) const {
  auto const_iterator = tag_frequency_.find(tag);

  if (const_iterator == tag_frequency_.end()) {
    return std::numeric_limits<double>::min();
  }

  return const_iterator->second;
}

}  // namespace hmmsegmenter
