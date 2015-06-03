//
// Created by hudong on 6/2/15.
//

#include "inc/string_util.h"

#include <unordered_set>

namespace hmmsegmenter {

size_t StringUtil::Tokenize(const std::string &line,
                            const std::string &delimeters,
                            std::vector<std::string> *tokens) {
  std::unordered_set<char> delis;
  for (char character : delimeters) {
    delis.insert(character);
  }

  size_t slow = 0;
  size_t length = line.length();
  while (slow < length && delis.find(line[slow]) != delis.end()) {
    ++slow;
  }

  size_t fast = slow + 1;
  while (fast < length) {
    if (delis.find(line[fast]) == delis.end()) {
      ++fast;
      continue;
    }
    tokens->push_back(line.substr(slow, fast - slow));
    while (fast < length && delis.find(line[fast]) != delis.end()) {
      ++fast;
    }
    slow = fast;
  }
  if (slow < fast) {
    tokens->push_back(line.substr(slow, fast - slow));
  }

  return tokens->size();
}

}  // namespace hmmsegmenter
