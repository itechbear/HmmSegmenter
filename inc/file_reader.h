//
// Created by hudong on 6/2/15.
//

#ifndef HMMSEGMENTER_FILE_H_
#define HMMSEGMENTER_FILE_H_

#include <string>
#include <vector>
#include <fstream>

namespace hmmsegmenter {

class FileReader {
 public:
  explicit FileReader(const std::string &path);

  ~FileReader();

  bool ReadIntoLines(std::vector<std::string> *lines);

  bool ReadLine(std::string *line);

  bool IsOk();

 private:
  const std::string path_;
  std::ifstream ifstream_;
};

}  // namespace hmmsegmenter

#endif //HMMSEGMENTER_FILE_H_
