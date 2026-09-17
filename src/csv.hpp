#pragma once
#include <string>
#include <algorithm>

inline std::string csv_escape(std::string s) {
  bool need_quotes = s.find_first_of(",\"\n") != std::string::npos;
  if (need_quotes) {
    std::string t; t.reserve(s.size()+2);
    for (char c : s) { t += (c=='"') ? std::string("\"\"") : std::string(1,c); }
    return "\"" + t + "\"";
  }
  return s;
}