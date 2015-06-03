//
// Created by hudong on 6/2/15.
//

#include <gtest/gtest.h>

#include "inc/string_util.h"

TEST(StringUtil,Tokenize) {
  {
    const std::string line = "人们";
    std::vector<std::string> tokens;
    ASSERT_EQ(1, hmmsegmenter::StringUtil::Tokenize(line, " ", &tokens));
    ASSERT_EQ("人们", tokens[0]);
  }

  {
    const std::string line = "人们";
    std::vector<std::string> tokens;
    ASSERT_EQ(1, hmmsegmenter::StringUtil::Tokenize(line, "", &tokens));
    ASSERT_EQ("人们", tokens[0]);
  }

  {
    const std::string line = "人们";
    std::vector<std::string> tokens;
    ASSERT_EQ(1, hmmsegmenter::StringUtil::Tokenize(line, "\t \n", &tokens));
    ASSERT_EQ("人们", tokens[0]);
  }

  {
    const std::string line = "人们 ";
    std::vector<std::string> tokens;
    ASSERT_EQ(1, hmmsegmenter::StringUtil::Tokenize(line, "\t \n", &tokens));
    ASSERT_EQ("人们", tokens[0]);
  }

  {
    const std::string line = " 人们";
    std::vector<std::string> tokens;
    ASSERT_EQ(1, hmmsegmenter::StringUtil::Tokenize(line, "\t \n", &tokens));
    ASSERT_EQ("人们", tokens[0]);
  }

  {
    const std::string line = " 人 们";
    std::vector<std::string> tokens;
    ASSERT_EQ(2, hmmsegmenter::StringUtil::Tokenize(line, "\t \n", &tokens));
    ASSERT_EQ("人", tokens[0]);
    ASSERT_EQ("们", tokens[1]);
  }

  {
    const std::string line = " 人#们";
    std::vector<std::string> tokens;
    ASSERT_EQ(2, hmmsegmenter::StringUtil::Tokenize(line, "\t \n#", &tokens));
    ASSERT_EQ("人", tokens[0]);
    ASSERT_EQ("们", tokens[1]);
  }
}
