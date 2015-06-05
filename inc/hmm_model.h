//
// Created by hudong on 6/2/15.
//

#ifndef HMMSEGMENTER_HMM_MODEL_H_
#define HMMSEGMENTER_HMM_MODEL_H_

#include <unordered_map>
#include <unordered_set>
#include <map>
#include <string>

#include "inc/character.h"

namespace hmmsegmenter {

class HmmModel {
 public:
  HmmModel();

  ~HmmModel();

  void AddCharacter(const std::string &character);

  void AddTag(const Character::Tag tag);

  void AddCharacterCondition(const Character::Tag tag,
                             const std::string &character);

  void AddTagCondition(const Character::Tag previous,
                       const Character::Tag current);

  void Calculate();

  double GetTransferProbability(const Character::Tag previous, const Character::Tag current) const;

  double GetEmissionProbability(const std::string &character,
                                const Character::Tag tag) const;

  double GetTagFrequency(const Character::Tag tag) const;

  void Clear();

 private:
  std::unordered_map<uint8_t, std::unordered_map<uint8_t, double> > transfer_matrix_;
  std::unordered_map<std::string, std::unordered_map<uint8_t, double> > emission_matrix_;
  std::unordered_map<uint8_t, std::unordered_map<std::string, uint32_t> > character_conditions_;
  std::unordered_map<std::string, uint32_t> characters_;
  std::unordered_map<uint8_t, std::unordered_map<uint8_t, uint32_t>> tag_conditions_;
  std::map<Character::Tag, double> tag_frequency_;
  std::map<Character::Tag, uint32_t> tags_;
  bool cleared_;
};

}  // namespace hmmsegmenter

#endif //HMMSEGMENTER_HMM_MODEL_H_
