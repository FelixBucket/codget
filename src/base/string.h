#pragma once

#include <string>
#include <cctype>
#include <vector>

struct ci_char_traits : public std::char_traits < char > {
  static bool eq(char c1, char c2) { return std::toupper(c1) == std::toupper(c2); }
  static bool ne(char c1, char c2) { return std::toupper(c1) != std::toupper(c2); }
  static bool lt(char c1, char c2) { return std::toupper(c1) < std::toupper(c2); }
  static int compare(char const* s1, char const* s2, size_t n) {
    while (n--) {
      char c1 = std::toupper(*s1++);
      char c2 = std::toupper(*s2++);
      if (c1 != c2) return (c1 < c2 ? -1 : 1);
    }
    return 0;
  }
  static char const* find(char const* s, int n, char a) {
    a = std::toupper(a);
    while (n-- && std::toupper(*s) != a) {
      ++s;
    }
    return (n >= 0 ? s : nullptr);
  }
};

class istring : public std::basic_string<char, ci_char_traits> {
public:
  typedef std::basic_string<char, ci_char_traits> _Base;
  istring() {}
  istring(istring const& str) : _Base(str) {}
  istring(std::string const& str) : _Base(str.c_str()) {}
  istring(char const* str) : _Base(str) {}
  istring(istring&& str) : _Base(str) {}
  istring(char const* str, size_t n) : _Base(str) {}
  template<class Iter>
  istring(Iter begin, Iter end) : _Base(begin, end) {}

  istring& operator=(std::string const& str) {
    assign(str.c_str());
    return *this;
  }
  operator std::string() const {
    return std::string(c_str());
  }
};
std::string strlower(std::string const& src);

template<class T>
inline int basic_compare(T const& lhs, T const& rhs) {
  if (lhs < rhs) return -1;
  if (lhs > rhs) return 1;
  return 0;
}

std::vector<std::string> split(std::string const& str, char sep = ' ');
std::vector<std::string> split(std::string const& str, char const* sep);
std::vector<std::string> split_multiple(std::string const& str, char const* sep);
std::string join(std::vector<std::string> const& list, char sep = ' ');
std::string join(std::vector<std::string> const& list, std::string const& sep);
template<class Iter>
inline std::string join(Iter left, Iter right) {
  std::string res;
  while (left != right) {
    if (!res.empty()) res.push_back(' ');
    res.append(*left++);
  }
  return res;
}

std::string fmtstring(char const* fmt, ...);
std::string varfmtstring(char const* fmt, va_list list);

std::wstring utf8_to_utf16(std::string const& str);
std::string utf16_to_utf8(std::wstring const& str);
std::string trim(std::string const& str);
