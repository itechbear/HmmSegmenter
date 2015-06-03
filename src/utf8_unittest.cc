//
// Created by hudong on 6/2/15.
//

#include <gtest/gtest.h>

#include "inc/utf8.h"

TEST(Utf8, GetByteCount) {
  ASSERT_EQ(1, hmmsegmenter::Utf8::GetByteCount('a'));
  ASSERT_EQ(1, hmmsegmenter::Utf8::GetByteCount('z'));
  ASSERT_EQ(1, hmmsegmenter::Utf8::GetByteCount(' '));
  ASSERT_EQ(1, hmmsegmenter::Utf8::GetByteCount(0b00111111));
  ASSERT_EQ(2, hmmsegmenter::Utf8::GetByteCount(0b11000000));
  ASSERT_EQ(2, hmmsegmenter::Utf8::GetByteCount(0b11000100));
  ASSERT_EQ(3, hmmsegmenter::Utf8::GetByteCount(0b11100100));
  ASSERT_EQ(4, hmmsegmenter::Utf8::GetByteCount(0b11110100));
}

TEST(Utf8, GetUtf8Strings) {
  {
    hmmsegmenter::Utf8 utf8("ab");
    std::vector<std::string> chars;
    utf8.GetUtf8Strings(&chars);
    ASSERT_EQ(2, chars.size());
    ASSERT_EQ("a", chars[0]);
    ASSERT_EQ("b", chars[1]);
  }

  {
    hmmsegmenter::Utf8 utf8("人们");
    std::vector<std::string> chars;
    utf8.GetUtf8Strings(&chars);
    ASSERT_EQ(2, chars.size());
    ASSERT_EQ("人", chars[0]);
    ASSERT_EQ("们", chars[1]);
  }

  {
    hmmsegmenter::Utf8 utf8(" 人们\t#");
    std::vector<std::string> chars;
    utf8.GetUtf8Strings(&chars);
    ASSERT_EQ(5, chars.size());
    ASSERT_EQ(" ", chars[0]);
    ASSERT_EQ("人", chars[1]);
    ASSERT_EQ("们", chars[2]);
    ASSERT_EQ("\t", chars[3]);
    ASSERT_EQ("#", chars[4]);
  }
}