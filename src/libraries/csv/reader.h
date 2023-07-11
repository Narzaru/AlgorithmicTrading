#ifndef CSV_READER_H_
#define CSV_READER_H_

#include "file.h"
#include "row.h"
#include "status.h"

namespace csv {

class Reader final {
 public:
  Reader() = default;

  template <int columns = 0>
  Reader(File file, bool have_header) {
    SetCSVFile(std::move(file), have_header);
  }

  ~Reader() = default;

  template <int columns = 0>
  void SetCSVFile(File file, bool have_header) {
    file_ = std::move(file);
    if (have_header) {
      headers_ = ReadLine<columns>();
    }
  }

  template <int columns = 0, typename... Args>
  std::tuple<Args...> ReadLineWithTypes() {
    row_ = std::move(ReadLine<columns>());
    return {row_.Next<Args>()...};
  }

  template <int columns = 0, typename... Args>
  auto ReadLineWithFunctors(Args... args)
      -> std::tuple<decltype(args(std::declval<std::string_view>()))...> {
    row_ = ReadLine<columns>();
    return {row_.NextWithFunctor(args)...};
  }

  template <int columns = 0>
  Row ReadLine() {
    do {
      row_.FromString(file_.ReadLine(), columns);
    } while (file_.GetStatus() != Status::kEndOfFile && row_.Size() == 0);
    if (file_.PeekLine().empty()) {
    }
    return std::move(row_);
  }

  void SkipLine() { file_.SkipLine(); }

  Row GetHeaders() const noexcept { return headers_; }

  bool Good() const { return row_.Good(); }

  bool EndOfFile() const { return file_.GetStatus() == Status::kEndOfFile; };

 private:
  File file_;
  Row row_;
  Row headers_;
};

}  // namespace csv

#endif  // CSV_READER_H_
