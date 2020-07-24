#include "path.h"
#include <windows.h>
using namespace std;

string path::name(string const& path) {
  int pos = path.length();
  while (pos > 0 && path[pos - 1] != '/' && path[pos - 1] != '\\') {
    --pos;
  }
  return path.substr(pos);
}
string path::title(string const& path) {
  int pos = path.length();
  int dot = path.length();
  while (pos > 0 && path[pos - 1] != '/' && path[pos - 1] != '\\') {
    --pos;
    if (path[pos] == '.' && dot == path.length()) {
      dot = pos;
    }
  }
  if (dot == pos) {
    return path.substr(pos);
  } else {
    return path.substr(pos, dot - pos);
  }
}
string path::path(string const& path) {
  int pos = path.length();
  while (pos > 0 && path[pos - 1] != '/' && path[pos - 1] != '\\') {
    --pos;
  }
  return path.substr(0, pos ? pos - 1 : 0);
}
string path::ext(string const& path) {
  int pos = path.length();
  int dot = path.length();
  while (pos > 0 && path[pos - 1] != '/' && path[pos - 1] != '\\') {
    --pos;
    if (path[pos] == '.' && dot == path.length()) {
      dot = pos;
    }
  }
  if (dot == pos) {
    return "";
  } else {
    return path.substr(dot);
  }
}

void path::create(std::string const& path) {
  std::string buf;
  for (char chr : path) {
    if (chr == '/' || chr == '\\') chr = sep;
    buf.push_back(chr);
    if (chr == sep) {
      CreateDirectory(buf.c_str(), nullptr);
    }
  }
  if (!buf.empty() && buf.back() != sep) {
    CreateDirectory(buf.c_str(), nullptr);
  }
}

#include <windows.h>
std::string path::root() {
  static std::string root_;
  if (root_.empty()) {
    char buffer[512];
    GetModuleFileName(GetModuleHandle(NULL), buffer, sizeof buffer);
    root_ = path(buffer);
  }
  return root_;
  //char buffer[512];
  //GetCurrentDirectory(sizeof buffer, buffer);
  //return buffer;
}

string operator / (string const& lhs, string const& rhs) {
  if (lhs.empty() || rhs.empty()) return lhs + rhs;
  bool left = (lhs.back() == '\\' || lhs.back() == '/');
  bool right = (rhs.front() == '\\' || rhs.front() == '/');
  if (left && right) {
    string res = lhs;
    res.pop_back();
    return res + rhs;
  } else if (!left && !right) {
    return lhs + path::sep + rhs;
  } else {
    return lhs + rhs;
  }
}
