#include <gtest/gtest.h>

#include "file.h"
#include "reader.h"

class TestClass {
 public:
  TestClass(std::string_view sv) { str = sv; }
  std::string str;
};

TEST(csv_reader_suite, read_tuple) {
  csv::File file;
  file.Open("dataset/valid_short.csv");
  csv::Reader reader;

  auto lambda_to_string = [](std::string_view sv) {
    return std::string(sv.begin(), sv.end());
  };

  auto lambda_to_int = [&lambda_to_string](std::string_view sv) {
    return std::stoi(lambda_to_string(sv));
  };

  reader.SetCSVFile(std::move(file), true);

  auto headers = reader.GetHeaders();
  auto expected_headers = std::vector<std::string>{"Date", "\"Close\""};
  for (decltype(headers)::SizeType i = 0; i < headers.Size(); ++i) {
    ASSERT_EQ(headers[i].To<std::string_view>(), expected_headers[i]);
  }

  auto tuple = reader.ReadLineWithFunctors<2>(lambda_to_string, lambda_to_int);
  ASSERT_EQ(std::get<0>(tuple), "2021-03-22");
  ASSERT_EQ(std::get<1>(tuple), 123);
}

TEST(csv_reader_sute, read_with_empty_lines_non_empty_last_line) {
  csv::File file;
  file.Open("dataset/valid_with_empty_line_non_empty_last_line.csv");
  csv::Reader reader;
  reader.SetCSVFile(std::move(file), true);

  int counter{0};
  while (!reader.EndOfFile()) {
    reader.ReadLine();
    counter++;
  }

  ASSERT_EQ(counter, 2);
}

TEST(csv_reader_sute, read_with_empty_lines_with_headers_flag) {
  csv::File file;
  file.Open("dataset/valid_with_empty_lines.csv");
  csv::Reader reader;
  reader.SetCSVFile(std::move(file), true);

  int counter{0};
  while (!reader.EndOfFile()) {
    reader.ReadLine();
    counter++;
  }

  ASSERT_EQ(counter, 3);
}

TEST(csv_reader_sute, read_with_empty_lines_without_headers_flag) {
  csv::File file;
  file.Open("dataset/valid_with_empty_lines.csv");
  csv::Reader reader;
  reader.SetCSVFile(std::move(file), false);

  int counter{0};
  while (!reader.EndOfFile()) {
    reader.ReadLine();
    counter++;
  }

  ASSERT_EQ(counter, 4);
}
