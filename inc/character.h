//
// Created by hudong on 6/5/15.
//

#ifndef HMMSEGMENTER_TOKEN_H_
#define HMMSEGMENTER_TOKEN_H_

#include <string>

namespace hmmsegmenter {

class Character {
 public:
  enum Language {
    OTHER = -1, CN, EN
  };

  enum Tag {
    Z = -1, B, E, M, S
  };

  explicit Character(const std::string &text);

  ~Character();

  const std::string &GetChracter() const;

  const Language &GetLanguage() const;

  void SetLanguage(const Language &language);

  const Tag &GetTag() const;

  void SetTag(const Tag &tag);

 private:
  const std::string chracter_;
  Language language_;
  Tag tag_;
};

}  // namespace hmmsegmenter

#endif //HMMSEGMENTER_TOKEN_H_
