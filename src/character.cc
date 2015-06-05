//
// Created by hudong on 6/5/15.
//

#include "inc/character.h"

namespace hmmsegmenter {

Character::Character(const std::string &text)
    : chracter_(text) {
}

Character::~Character() {
}

const std::string &Character::GetChracter() const {
  return chracter_;
}

const Character::Language &Character::GetLanguage() const {
  return language_;
}

void Character::SetLanguage(const Character::Language &language) {
  language_ = language;
}

const Character::Tag &Character::GetTag() const {
  return tag_;
}

void Character::SetTag(const Character::Tag &tag) {
  tag_ = tag;
}
}  // namespace hmmsegmenter