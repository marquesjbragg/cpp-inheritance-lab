#pragma once
#include "person.hpp"
#include "csv.hpp"
#include <vector>

class Student : public Person {
  int grad_year_;
  std::vector<std::string> courses_;
public:
  Student(int id, std::string name, std::string email, int grad_year,
          std::vector<std::string> courses = {})
    : Person(id, std::move(name), std::move(email)),
      grad_year_(grad_year), courses_(std::move(courses)) {
    if (grad_year_ < 2000) throw ValidationError("grad_year too small");
  }

  std::string role() const override { return "Student"; }

  nlohmann::json to_json() const override {
    return {
      {"role", role()}, {"id", id_}, {"name", name_}, {"email", email_},
      {"grad_year", grad_year_}, {"courses", courses_}
    };
  }

  YAML::Node to_yaml() const override {
    YAML::Node n;
    n["role"] = role(); n["id"] = id_; n["name"] = name_; n["email"] = email_;
    n["grad_year"] = grad_year_;
    for (const auto& c : courses_) n["courses"].push_back(c);
    return n;
  }

  std::string csv_header() const override {
    return "role,id,name,email,grad_year,courses"; // courses joined by ';'
  }

  std::string csv_row() const override {
    std::string joined;
    for (size_t i=0;i<courses_.size();++i) { if (i) joined+=';'; joined += courses_[i]; }
    return csv_escape(role()) + "," + std::to_string(id_) + "," +
           csv_escape(name_) + "," + csv_escape(email_) + "," +
           std::to_string(grad_year_) + "," + csv_escape(joined);
  }
};