#pragma once
#include "serializable.hpp"
#include "exceptions.hpp"
#include "student.hpp"
#include "instructor.hpp"
#include "csv.hpp"
#include <string>
#include <vector>

class Course : public Serializable {
  std::string title_;
  std::vector<Student> roster_;
  Instructor instructor_;
public:
  Course(std::string title, std::vector<Student> roster, Instructor instructor)
    : title_(std::move(title)), roster_(std::move(roster)), instructor_(std::move(instructor)) {
    if (title_.empty()) throw ValidationError("course title is required");
  }

  nlohmann::json to_json() const override {
    nlohmann::json roster_json = nlohmann::json::array();
    for (const auto& s : roster_) roster_json.push_back(s.to_json());
    return {
      {"title", title_},
      {"instructor", instructor_.to_json()},
      {"roster", roster_json}
    };
  }

  YAML::Node to_yaml() const override {
    YAML::Node n;
    n["title"] = title_;
    n["instructor"] = instructor_.to_yaml();
    for (const auto& s : roster_) n["roster"].push_back(s.to_yaml());
    return n;
  }

  std::string csv_header() const override {
    return "title,instructor_name,instructor_office,student_count,student_names";
  }

  std::string csv_row() const override {
    std::string names;
    for (size_t i = 0; i < roster_.size(); ++i) {
      if (i) names += ';';
      names += roster_[i].name();
    }
    std::string office = instructor_.to_json().at("office").get<std::string>();
    return csv_escape(title_) + "," +
           csv_escape(instructor_.name()) + "," +
           csv_escape(office) + "," +
           std::to_string(roster_.size()) + "," +
           csv_escape(names);
  }
};