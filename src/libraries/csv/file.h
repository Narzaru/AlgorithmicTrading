#ifndef CSV_FILE_H_
#define CSV_FILE_H_

#include <fstream>
#include <string>
#include <vector>

#include "row.h"
#include "status.h"
#include "utils.h"

namespace csv {

class File {
 public:
  void Open(const std::string &filepath,
            std::ios_base::openmode open_mode = std::ios::in);
  void SetStream(std::fstream stream);
  std::fstream GetStream() noexcept;
  Status GetStatus() const noexcept;

  const std::string &ReadLine();
  const std::string &PeekLine();
  void SkipLine();

  void WriteLine(const std::string &string);

 protected:
  void ResetFile();

  // file cursor position
  void SaveCursorPosition() noexcept;
  void RestoreCursorPosition() noexcept;

  std::fstream file_;
  Status status_;
  std::string current_line_;
  std::streampos position_;
};

}  // namespace csv

#endif  // CSV_FILE_H_
