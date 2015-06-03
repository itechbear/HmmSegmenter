//
// Created by hudong on 6/2/15.
//

#ifndef HMMSEGMENTER_HMM_MODEL_H_
#define HMMSEGMENTER_HMM_MODEL_H_

#include <unordered_map>
#include <unordered_set>
#include <map>

namespace hmmsegmenter {

class HmmModel {
 public:
  enum Tag {
    B, E, M, S
  };

  HmmModel();

  ~HmmModel();

  double GetTransferProbability(Tag first, Tag second) const;

  double GetEmissionProbability(const std::string &character, Tag tag) const;

  void AddCharacter(const std::string &character);

  void AddTag(Tag tag);

  void AddCondition(Tag tag, const std::string &character);

  void Calculate();

 private:
  std::unordered_map<std::string, double> transfer_matrix_;
  std::unordered_map<std::string, double> emission_matrix_;
  std::unordered_map<std::string, uint32_t> conditions_;
  std::unordered_map<std::string, uint32_t> characters_;
  std::map<Tag, uint32_t> tags_;
};

}  // namespace hmmsegmenter

#endif //HMMSEGMENTER_HMM_MODEL_H_
