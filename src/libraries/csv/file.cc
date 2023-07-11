#include "file.h"

#include <filesystem>

namespace csv {
void File::Open(const std::string& filepath,
                std::ios_base::openmode open_mode) {
  ResetFile();
  file_.open(filepath, open_mode);

  if (!file_.is_open()) {
    status_ = Status::kOpenError;
    return;
  }

  file_.seekg(std::ios::end);
  if (file_.tellg() == 0) {
    status_ = Status::kEmptyFile;
  } else {
    file_.seekg(std::ios::beg);
  }

  status_ = Status::kOpenSuccess;
}

void File::SetStream(std::fstream stream) { file_ = std::move(stream); }

std::fstream File::GetStream() noexcept { return std::move(file_); }

const std::string& File::ReadLine() {
  std::getline(file_, current_line_);

  if (file_.eof()) {
    status_ = Status::kEndOfFile;
    return current_line_;
  } else if (!file_) {
    status_ = Status::kReadError;
    return current_line_;
  } else {
    status_ = Status::kReadSuccess;
    return current_line_;
  }
}

const std::string& File::PeekLine() {
  SaveCursorPosition();
  ReadLine();
  RestoreCursorPosition();
  return current_line_;
}

void File::SkipLine() {
  file_.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  if (file_.eof()) {
    status_ = Status::kEndOfFile;
  } else if (!file_) {
    status_ = Status::kReadError;
  }
}

void File::WriteLine(const std::string& string) {
  if (string.length() > std::numeric_limits<std::streamsize>::max()) {
    status_ = Status::kWriteStringSizeOverflow;
    return;
  }
  file_.write(string.c_str(), static_cast<std::streamsize>(string.length()));
  if (!file_) {
    status_ = Status::kWriteError;
  } else {
    status_ = Status::kWriteSuccess;
  }
}

Status File::GetStatus() const noexcept { return status_; }

void File::SaveCursorPosition() noexcept { position_ = file_.tellg(); }

void File::RestoreCursorPosition() noexcept { file_.seekg(position_); }
void File::ResetFile() {
  file_.close();
  status_ = Status::kClosed;
  current_line_.clear();
}

}  // namespace csv
