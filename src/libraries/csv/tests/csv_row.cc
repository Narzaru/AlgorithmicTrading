#include <gtest/gtest.h>

#include <functional>

#include "row.h"

class csv_base_parametrized_fixture
    : public ::testing::TestWithParam<
          std::tuple<std::string, std::string, int, int>> {
 protected:
  csv::Row csv_row_;
};

TEST_P(csv_base_parametrized_fixture, CSVRowOpearatorWithParameters) {
  auto [expected_string, csv_row, columns, column] = GetParam();
  csv_row_.FromString(csv_row, columns);
  ASSERT_EQ(expected_string, static_cast<std::string_view>(csv_row_[column]));
}

#define test_data_1 "aboba", "aboba,aboba2,aboba3", 3, 0
#define test_data_2 "aboba2", "aboba,aboba2,aboba3", 3, 1
#define test_data_3 "aboba3", "aboba,aboba2,aboba3", 3, 2
#define test_data_4 "\"", R"("""")", 1, 0
#define test_data_5 "\"Hello, Maksat\"", R"("""Hello, Maksat""")", 1, 0
#define test_data_6 "", ",,,", 4, 0
#define test_data_7 "", ",,,", 4, 3

INSTANTIATE_TEST_SUITE_P(CSVRowParser, csv_base_parametrized_fixture,
                         ::testing::Values(std::make_tuple(test_data_1),
                                           std::make_tuple(test_data_2),
                                           std::make_tuple(test_data_3),
                                           std::make_tuple(test_data_4),
                                           std::make_tuple(test_data_5),
                                           std::make_tuple(test_data_6),
                                           std::make_tuple(test_data_7)));

class CSVRowTestClass {
 public:
  explicit CSVRowTestClass(std::string_view string)
      : result_{string.begin(), string.end()} {
    result_ += result_;
  }

  CSVRowTestClass(std::string_view string, int number1, int number2)
      : CSVRowTestClass(string) {
    result_ = std::to_string(number1) + result_ + std::to_string(number2);
  }

  [[nodiscard]] const std::string& GetResult() const { return result_; }

 private:
  std::string result_;
};

TEST(csv_row, get_custom_method_test) {
  csv::Row row;
  row.FromString("Hello, World, Maks", {});

  auto lambda = [](std::string_view sv) -> CSVRowTestClass {
    return {sv, 10, 10};
  };

  std::function<CSVRowTestClass(std::string_view)> func = lambda;

  auto res = row[0].ToWithFunctor(func);
  ASSERT_EQ(res.GetResult(), "10HelloHello10");
}

CSVRowTestClass test_function_throw_exception(std::string_view sv) noexcept(
    false) {
  return CSVRowTestClass(sv);
}

CSVRowTestClass test_function_noexcept(std::string_view sv) noexcept {
  return CSVRowTestClass(sv);
}

TEST(csv_row, get_custom_method_test_with_raw_function) {
  csv::Row row;
  row.FromString("Hello, World, Maks", {});

  auto res = row[0].ToWithFunctor(test_function_throw_exception);
  ASSERT_EQ(res.GetResult(), "HelloHello");
}

TEST(csv_row,
     get_custom_method_test_with_raw_function_thats_may_throw_exception) {
  csv::Row row;
  ASSERT_FALSE(noexcept(row[0].ToWithFunctor(test_function_throw_exception)));
}

TEST(csv_row, get_custom_method_test_with_raw_function_with_thats_noexcept) {
  csv::Row row;
  ASSERT_TRUE(noexcept(row[0].ToWithFunctor(test_function_noexcept)));
}

TEST(csv_row, get_method_test) {
  csv::Row row;
  row.FromString("Goodbye, World", {});

  auto res = row[0].To<CSVRowTestClass>();
  ASSERT_EQ(res.GetResult(), "GoodbyeGoodbye");
}

TEST(csv_row, get_method_test_that_except) {
  csv::Row row;
  ASSERT_FALSE(noexcept(row[0].To<CSVRowTestClass>()));
}

class CSVRowTestClassNothrow {
 public:
  explicit CSVRowTestClassNothrow(std::string_view string) noexcept
      : result_{string.begin(), string.end()} {}

 private:
  std::string result_;
};

TEST(csv_row, get_method_test_that_noexcept) {
  csv::Row row;
  ASSERT_TRUE(noexcept(row[0].To<CSVRowTestClassNothrow>()));
}

TEST(csv_row, asdasdsad) {
  csv::Row row;
  row.FromString("hello,world", {});

  auto lambda = [](std::string_view sv) -> CSVRowTestClass {
    return CSVRowTestClass(sv);
  };

  auto result = row[1].ToWithFunctor(lambda);
  ASSERT_EQ(result.GetResult(), "worldworld");
}
