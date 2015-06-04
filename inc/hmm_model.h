//
// Created by hudong on 6/2/15.
//

#ifndef HMMSEGMENTER_HMM_MODEL_H_
#define HMMSEGMENTER_HMM_MODEL_H_

#include <unordered_map>
#include <unordered_set>
#include <map>
#include <string>

namespace hmmsegmenter {

class HmmModel {
 public:
  enum Tag {
    Z = -1, B, E, M, S
  };

  static inline char TagToChar(const Tag tag) {
    switch (tag) {
      case B:
        return 'B';
      case E:
        return 'E';
      case M:
        return 'M';
      case S:
        return 'S';
      default:
        return 'Z';
    }
  }

  HmmModel();

  ~HmmModel();

  void AddCharacter(const std::string &character);

  void AddTag(const Tag tag);

  void AddCharacterCondition(const Tag tag,
                             const std::string &character);

  void AddTagCondition(const Tag previous,
                       const Tag current);

  void Calculate();

  double GetTransferProbability(const Tag previous, const Tag current) const;

  double GetEmissionProbability(const std::string &character,
                                const Tag tag) const;

  double GetTagFrequency(const Tag tag) const;

  void Clear();

 private:
  std::unordered_map<uint8_t, std::unordered_map<uint8_t, double> > transfer_matrix_;
  std::unordered_map<std::string, std::unordered_map<uint8_t, double> > emission_matrix_;
  std::unordered_map<uint8_t, std::unordered_map<std::string, uint32_t> > character_conditions_;
  std::unordered_map<std::string, uint32_t> characters_;
  std::unordered_map<uint8_t, std::unordered_map<uint8_t, uint32_t>> tag_conditions_;
  std::map<Tag, double> tag_frequency_;
  std::map<Tag, uint32_t> tags_;
  bool cleared_;
};

}  // namespace hmmsegmenter

#endif //HMMSEGMENTER_HMM_MODEL_H_
