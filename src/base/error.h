#pragma once

#include <string>
#include <sstream>

class Exception {
public:
  Exception(char const* fmt, ...);
  Exception(Exception const& e)
    : buf_(e.buf_.str())
  {}

  virtual char const* what() const throw() {
    str_ = buf_.str();
    return str_.c_str();
  }

  template<class T>
  inline void append(T const& t) {
    buf_ << t;
  }
private:
  mutable std::string str_;
  std::stringstream buf_;
};

template<class T>
static inline Exception&& operator<<(Exception&& e, T const& t) {
  e.append(t);
  return std::forward<Exception>(e);
}
