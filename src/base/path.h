#pragma once

#include <string>

namespace path {
  std::string name(std::string const& path);
  std::string title(std::string const& path);
  std::string path(std::string const& path);
  std::string ext(std::string const& path);
  void create(std::string const& path);

  std::string root();

  static const char sep = '\\';
}

std::string operator / (std::string const& lhs, std::string const& rhs);
