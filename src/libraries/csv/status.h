#ifndef CSV_STATUS_H_
#define CSV_STATUS_H_

namespace csv {

enum class Status {
  kOpenSuccess,
  kCorrectFile,
  kEmptyFile,
  kOpenError,
  kReadError,
  kReadSuccess,
  kWriteStringSizeOverflow,
  kWriteError,
  kWriteSuccess,
  kEndOfFile,
  kClosed,
};

}

#endif  // CSV_STATUS_H_
