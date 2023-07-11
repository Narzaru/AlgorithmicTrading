#include <gtest/gtest.h>

#include <filesystem>

#include "file.h"

TEST(csv_file_suite, open_normal_csv_file) {
  csv::File file;
  file.Open("dataset/valid_short.csv");
  ASSERT_EQ(file.GetStatus(), csv::Status::kOpenSuccess);
}

TEST(csv_file_suite, csv_file_with_empty_lines_read) {
  csv::File file;
  file.Open("dataset/valid_with_empty_lines.csv");
  int line_counter{0};

  while (file.GetStatus() != csv::Status::kEndOfFile) {
    file.ReadLine();
    line_counter++;
  }

  ASSERT_EQ(line_counter, 7);
}

TEST(csv_reader_sute, skip_line) {
  csv::File file;
  file.Open("dataset/valid_short.csv");

  auto expected = R"(2021-03-22,123)";
  file.SkipLine();

  auto read = file.ReadLine();

  ASSERT_EQ(expected, read);
}

TEST(csv_reader_sute, skip_line_at_end_of_file) {
  csv::File file;
  file.Open("dataset/valid_short.csv");

  file.SkipLine();
  file.SkipLine();
  file.SkipLine();

  ASSERT_EQ(file.GetStatus(), csv::Status::kEndOfFile);
}

TEST(csv_reader_sute, peek_line) {
  csv::File file;
  file.Open("dataset/valid_short.csv");

  auto expected = R"("Date","""Close""")";
  auto first_read = file.PeekLine();
  auto second_read = file.PeekLine();

  ASSERT_EQ(expected, first_read);
  ASSERT_EQ(first_read, second_read);
}

TEST(csv_reader_sute, peek_line_with_end_of_file) {
  csv::File file;
  file.Open("dataset/valid_short.csv");

  file.SkipLine();
  file.SkipLine();
  auto read = file.PeekLine();

  ASSERT_EQ(read, "");
}
