//
// Created by hudong on 6/2/15.
//

#include "inc/file_reader.h"

#include "glog/logging.h"

namespace hmmsegmenter {

FileReader::FileReader(const std::string &path)
    : path_(path), ifstream_(path) {
}

FileReader::~FileReader() {
  ifstream_.close();
}

bool FileReader::ReadIntoLines(std::vector<std::string> *lines) {
  CHECK(ifstream_.good()) << "Failed to open file: " << path_;
  ifstream_.seekg(0, std::ios::beg);
  std::string line;
  while (ifstream_ >> line) {
    lines->push_back(line);
  }
  return true;
}

bool FileReader::ReadLine(std::string *line) {
  ifstream_ >> *line;
  return !ifstream_.eof();
}

bool FileReader::IsOk() {
  return ifstream_.good();
}

}  // namespace hmmsegmenter
