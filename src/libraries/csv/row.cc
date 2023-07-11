#include "row.h"

#include <algorithm>
#include <utility>

namespace csv {

Row::Row(Row&& other) noexcept
    : parsing_error_{std::exchange(other.parsing_error_, false)},
      index_for_next_{std::exchange(other.index_for_next_, 0)},
      number_of_items_{std::exchange(other.number_of_items_, 0)},
      original_row_{std::move(other.original_row_)},
      word_boundaries_{std::move(other.word_boundaries_)} {}

Row::Row(std::string row, int number_of_columns)
    : parsing_error_{false},
      index_for_next_{0},
      number_of_items_{0},
      original_row_{std::move(row)},
      word_boundaries_{} {
  word_boundaries_.resize(number_of_columns, std::make_pair(0, 0));
  ParseRow();
}

Row& Row::operator=(Row other) {
  if (this == &other) {
    return *this;
  }
  parsing_error_ = other.parsing_error_;
  index_for_next_ = other.index_for_next_;
  number_of_items_ = other.number_of_items_;
  original_row_ = std::move(other.original_row_);
  word_boundaries_ = std::move(other.word_boundaries_);
  return *this;
}

Row& Row::FromString(std::string row, int number_of_columns) {
  parsing_error_ = false;
  number_of_items_ = 0;
  index_for_next_ = 0;
  original_row_ = std::move(row);
  word_boundaries_.reserve(number_of_columns);
  if (original_row_.empty()) {
    word_boundaries_.emplace_back(0, 0);
  } else {
    ParseRow();
  }
  return *this;
}

Row::RowData Row::operator[](Row::SizeType index) const noexcept {
  const static std::string empty;

  std::string_view string_view;
  auto& bounds = word_boundaries_[index];

  if (bounds.first == bounds.second) {
    string_view = empty;
  } else {
    string_view = {&original_row_[bounds.first], bounds.second - bounds.first};
  }
  return RowData(string_view);
}

Row& Row::Clear() noexcept {
  parsing_error_ = false;
  number_of_items_ = 0;
  original_row_.clear();
  word_boundaries_.clear();
  return *this;
}

auto Row::Size() const noexcept -> SizeType { return number_of_items_; }

bool Row::Good() const noexcept { return !parsing_error_; }

bool Row::Empty() const noexcept { return Size() == 0; }
// https://www.aleksandrhovhannisyan.com/blog/implementing-a-finite-state-machine-in-cpp/
/// Yes, I know that this is a very long code,
/// but I did not think of another way.
/// If you shove everything into methods / functions,
/// then the methods or functions become too ugly.
void Row::ParseRow() {
  enum class CSVState {
    StartOfToken,
    EmptyToken,
    QuotedToken,
    ExtraQuote,
    EndOfQuotedToken,
    DoubleQuote,
    UnquotedToken,
    EndOfUnquotedToken,
    ParsingError,
  };

  CSVState state = CSVState::StartOfToken;
  word_boundaries_.emplace_back(0, 0);

  SizeType bounds_idx{0};
  SizeType character_start_idx{0};
  std::string::difference_type character_idx{0};

  while (static_cast<std::size_t>(character_idx) <= original_row_.length()) {
    switch (state) {
      case CSVState::StartOfToken: {
        if (original_row_.begin() + character_idx == original_row_.end()) {
          state = CSVState::EmptyToken;
        } else if (original_row_[character_idx] == ',') {
          state = CSVState::EmptyToken;
        } else if (original_row_[character_idx] == '"') {
          state = CSVState::QuotedToken;
        } else {
          state = CSVState::UnquotedToken;
        }
      } break;
      case CSVState::EmptyToken: {
        state = CSVState::StartOfToken;
        character_start_idx++;
        character_idx++;
        bounds_idx++;
        word_boundaries_.emplace_back(0, 0);
        number_of_items_++;
      } break;
      case CSVState::QuotedToken: {
        character_idx++;
        if (original_row_[character_idx] == '"') {
          state = CSVState::ExtraQuote;
        }
      } break;
      case CSVState::ExtraQuote: {
        character_idx++;
        if (original_row_.begin() + character_idx == original_row_.end()) {
          state = CSVState::EndOfQuotedToken;
        } else if (original_row_[character_idx] == ',') {
          state = CSVState::EndOfQuotedToken;
        } else if (original_row_[character_idx] == '"') {
          state = CSVState::DoubleQuote;
        } else {
          state = CSVState::ParsingError;
        }
      } break;
      case CSVState::EndOfQuotedToken: {
        state = CSVState::StartOfToken;
        // +1 because start idx is in position at open quote
        word_boundaries_[bounds_idx].first = character_start_idx + 1;
        // -1 because current idx is in position after a close quote
        word_boundaries_[bounds_idx].second = character_idx - 1;
        character_idx++;
        character_start_idx = character_idx;
        bounds_idx++;
        word_boundaries_.emplace_back(0, 0);
        number_of_items_++;
      } break;
      case CSVState::DoubleQuote: {
        state = CSVState::QuotedToken;
        character_idx--;
        original_row_.erase(original_row_.begin() + character_idx);
      } break;
      case CSVState::UnquotedToken: {
        if (original_row_.begin() + character_idx == original_row_.end()) {
          state = CSVState::EndOfUnquotedToken;
        } else if (original_row_[character_idx] == ',') {
          state = CSVState::EndOfUnquotedToken;
        } else if (original_row_[character_idx] == '"') {
          state = CSVState::ParsingError;
        } else {
          character_idx++;
        }
      } break;
      case CSVState::EndOfUnquotedToken: {
        state = CSVState::StartOfToken;
        word_boundaries_[bounds_idx].first = character_start_idx;
        // -1 because current idx is in position a comma
        word_boundaries_[bounds_idx].second = character_idx;
        character_idx++;
        character_start_idx = character_idx;
        bounds_idx++;
        word_boundaries_.emplace_back(0, 0);
        number_of_items_++;
      } break;
      case CSVState::ParsingError:
        parsing_error_ = true;
        break;
    }
  }
}
}  // namespace csv
