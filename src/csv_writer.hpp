#pragma once
#include <span>
#include <sstream>
#include <vector>
#include <string>
#include "person.hpp"
#include "csv.hpp"

inline std::string csv_join(const std::vector<std::string>& v) {
  std::string joined;
  for (size_t i = 0; i < v.size(); ++i) {
    if (i) joined += ';';
    joined += v[i];
  }
  return joined;
}

inline std::string write_people_csv(std::span<const Person*> people) {
  static const std::vector<std::string> columns = {
    "role","id","name","email","grad_year","office","courses","teaches"
  };

  std::ostringstream out;
  for (size_t i = 0; i < columns.size(); ++i) {
    if (i) out << ',';
    out << columns[i];
  }
  out << '\n';

  for (const Person* p : people) {
    nlohmann::json j = p->to_json();

    auto field = [&](const std::string& key) -> std::string {
      if (!j.contains(key)) return "";
      if (j.at(key).is_array())
        return csv_join(j.at(key).get<std::vector<std::string>>());
      if (j.at(key).is_number_integer())
        return std::to_string(j.at(key).get<int>());
      return j.at(key).get<std::string>();
    };

    for (size_t i = 0; i < columns.size(); ++i) {
      if (i) out << ',';
      out << csv_escape(field(columns[i]));
    }
    out << '\n';
  }
  return out.str();
}