#include "student.hpp"
#include "instructor.hpp"
#include <iostream>
#include <memory>
#include <vector>

int main() {
  try {
    std::vector<std::unique_ptr<Person>> people;
    people.emplace_back(std::make_unique<Student>(1,"Ada","ada@uni.edu",2026, std::vector<std::string>{"CS101","MATH200"}));
    people.emplace_back(std::make_unique<Instructor>(2,"Grace","grace@uni.edu","Room 314", std::vector<std::string>{"CS101"}));

    // Serialize to JSON array
    nlohmann::json j = nlohmann::json::array();
    for (const auto& p : people) j.push_back(p->to_json());
    std::cout << "JSON:\n" << j.dump(2) << "\n\n";

    // Serialize to YAML sequence
    YAML::Node y;
    for (const auto& p : people) y.push_back(p->to_yaml());
    std::cout << "YAML:\n" << YAML::Dump(y) << "\n";

    // CSV (per concrete type; headers differ)
    std::cout << "\nCSV (Student):\n";
    Student tmpS(99,"Tmp","tmp@uni.edu",2030);
    std::cout << tmpS.csv_header() << "\n";
    std::cout << static_cast<Student&>(*people[0]).csv_row() << "\n";

    std::cout << "\nCSV (Instructor):\n";
    Instructor tmpI(98,"TmpI","tmpI@uni.edu","Room 1");
    std::cout << tmpI.csv_header() << "\n";
    std::cout << static_cast<Instructor&>(*people[1]).csv_row() << "\n";
  } catch (const ValidationError& e) {
    std::cerr << "ValidationError: " << e.what() << "\n";
    return 2;
  } catch (const SerializationError& e) {
    std::cerr << "SerializationError: " << e.what() << "\n";
    return 3;
  } catch (const std::exception& e) {
    std::cerr << "Unhandled std::exception: " << e.what() << "\n";
    return 1;
  }
  return 0;
}