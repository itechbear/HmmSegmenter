add_library(
  hmmsegmenter
  ../inc/file_reader.h
  file_reader.cc
  ../inc/utf8.h
  utf8.cc
  ../inc/string_util.h
  string_util.cc
  ../inc/training.h
  training.cc
  ../inc/hmm_model.h
  hmm_model.cc
  ../inc/tagger.h tagger.cc ../inc/character.h character.cc)

target_link_libraries(
  hmmsegmenter
  glog
  gflags
  )

add_executable(
  utf8_unittest
  utf8_unittest.cc
  )

target_link_libraries(
  utf8_unittest
  hmmsegmenter
  gtest_main
  )

add_test(
  utf8_unittest
  utf8_unittest
  )

add_executable(
  string_util_unittest
  string_util_unittest.cc
  )

target_link_libraries(
  string_util_unittest
  hmmsegmenter
  gtest_main
  )

add_test(
  string_util_unittest
  string_util_unittest
  )