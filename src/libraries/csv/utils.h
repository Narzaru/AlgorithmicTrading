#ifndef CSV_UTILS_H_
#define CSV_UTILS_H_

namespace csv {

struct empty_item final {};

struct column final {
  explicit column([[maybe_unused]] std::string_view sv) {}
  empty_item operator()([[maybe_unused]] std::string_view) { return obj; }
  static empty_item skip([[maybe_unused]] std::string_view) { return obj; }
  inline static empty_item obj;
};

#if defined(csv_skip_column)
#warning "variable csv_skip_column is already defined, an error may occur"
#else
#define csv_skip_column &csv::column::skip
#endif  // csv_skip_column

}  // namespace csv

#endif  // CSV_UTILS_H_
