#include <catch2/catch_test_macros.hpp>
#include "student.hpp"
#include "instructor.hpp"
#include "csv_writer.hpp"
#include "course.hpp"

TEST_CASE("Student JSON serialization") {
  Student s(10,"Alice","alice@uni.edu",2027, {"CS101","HIST110"});
  auto j = s.to_json();
  CHECK(j.at("role") == "Student");
  CHECK(j.at("name") == "Alice");
  CHECK(j.at("grad_year") == 2027);
}

TEST_CASE("Instructor YAML serialization") {
  Instructor i(11,"Bob","bob@uni.edu","C-210", {"CS101"});
  auto y = i.to_yaml();
  CHECK(y["role"].as<std::string>() == "Instructor");
  CHECK(y["office"].as<std::string>() == "C-210");
}

TEST_CASE("CSV escaping handles commas and quotes") {
  Student s(12,"Eve, \"The Great\"","eve@uni.edu",2028, {"CS,101","AI\"Lab"});
  auto row = s.csv_row();
  REQUIRE(row.find("\"Eve, \"\"The Great\"\"\"") != std::string::npos);
}

TEST_CASE("Validation throws on bad email") {
  REQUIRE_THROWS_AS(Student(13,"Zed","not-an-email",2026), ValidationError);
}

TEST_CASE("Polymorphic CSV writer emits superset header with stable columns") {
  Student s(20,"Sam","sam@uni.edu",2025, {"CS101"});
  Instructor ins(21,"Rita","rita@uni.edu","Room 9", {"CS101","CS102"});
  std::vector<const Person*> people = {&s, &ins};
  std::string csv = write_people_csv(people);

  REQUIRE(csv.find("role,id,name,email,grad_year,office,courses,teaches\n") == 0);
  REQUIRE(csv.find("Student,20,Sam,sam@uni.edu,2025,,CS101,\n") != std::string::npos);
  REQUIRE(csv.find("Instructor,21,Rita,rita@uni.edu,,Room 9,,CS101;CS102\n") != std::string::npos);
}

TEST_CASE("Course serializes via interface only (LSP)") {
  Student s1(30,"Mia","mia@uni.edu",2026, {"CS101"});
  Student s2(31,"Ravi","ravi@uni.edu",2027, {"CS102","MATH201"});
  Instructor prof(32,"Dr. Lee","lee@uni.edu","Room 5", {"CS101"});

  Course c("Intro to CS", {s1, s2}, prof);

  auto j = c.to_json();
  CHECK(j.at("title") == "Intro to CS");
  CHECK(j.at("instructor").at("name") == "Dr. Lee");
  CHECK(j.at("roster").size() == 2);

  auto row = c.csv_row();
  REQUIRE(row.find("Mia;Ravi") != std::string::npos);
}

TEST_CASE("Course throws on empty title") {
  Instructor prof(33,"Dr. X","x@uni.edu","Room 1");
  REQUIRE_THROWS_AS(Course("", {}, prof), ValidationError);
}