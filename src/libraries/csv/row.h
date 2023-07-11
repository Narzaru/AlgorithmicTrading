#ifndef CSV_ROW_H_
#define CSV_ROW_H_

#include <functional>
#include <iterator>
#include <string>
#include <vector>

#include "utils.h"

namespace csv {

class Row {
 public:
  using SizeType = std::string::size_type;

  class RowData {
   public:
    RowData() = delete;
    explicit RowData(std::string_view view) : view_(view) {}
    RowData(const RowData&) = delete;
    RowData(RowData&&) = default;

    template <typename T>
    explicit operator T() const
        noexcept(std::is_nothrow_constructible<T, std::string>()) {
      static_assert(
          std::is_constructible<T, std::string>(),
          "Cant construct object of type T with string_view parameter type");
      return T(view_);
    }

    template <typename T>
    T To() noexcept(noexcept(this->operator T())) {
      return static_cast<T>(*this);
    }

    template <typename Fun>
    decltype(auto) ToWithFunctor(Fun functor) noexcept(
        std::is_nothrow_invocable<Fun, std::string_view>()) {
      static_assert(
          std::is_invocable<Fun, std::string_view>(),
          "Fun object is not invocable object or mismatch parameters");
      return functor(view_);
    }

   private:
    std::string_view view_;
  };

  // !TODO(bgreydon): implement iterator for a row data

  Row() = default;
  Row(const Row& other) = default;
  Row(Row&& other) noexcept;
  Row(std::string row, int number_of_columns);
  Row& operator=(Row other);

  Row& FromString(std::string row, int number_of_columns);
  Row& Clear() noexcept;

  [[nodiscard]] RowData operator[](SizeType index) const noexcept;

  [[nodiscard]] SizeType Size() const noexcept;

  [[nodiscard]] bool Good() const noexcept;

  [[nodiscard]] bool Empty() const noexcept;

  template <typename T>
  T Next() {
    RowData data((*this)[index_for_next_]);
    index_for_next_++;
    return data.To<T>();
  }

  template <typename Fun>
  auto NextWithFunctor(Fun functor)
      -> decltype(functor(std::declval<std::string_view>())) {
    RowData data((*this)[index_for_next_]);
    index_for_next_++;
    return data.ToWithFunctor(functor);
  }

 private:
  void ParseRow();

  bool parsing_error_{false};
  SizeType index_for_next_{0};
  SizeType number_of_items_{0};
  std::string original_row_;
  // first - start index
  // second - past end index
  std::vector<std::pair<SizeType, SizeType>> word_boundaries_;
};

}  // namespace csv

#endif  // CSV_ROW_H_
