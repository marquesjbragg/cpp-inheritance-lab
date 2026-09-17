#pragma once
#include "serializable.hpp"
#include "exceptions.hpp"
#include <string>
#include <regex>

class Person : public Serializable {
protected:
  int id_;
  std::string name_;
  std::string email_;

  static void validate_email(const std::string& e) {
    // intentionally simple for the lab; throws on bad input
    static const std::regex re(R"(^[^@\s]+@[^@\s]+\.[^@\s]+$)");
    if (!std::regex_match(e, re)) throw ValidationError("Invalid email: " + e);
  }

public:
  Person(int id, std::string name, std::string email)
    : id_(id), name_(std::move(name)), email_(std::move(email)) {
    if (id_ <= 0) throw ValidationError("id must be positive");
    if (name_.empty()) throw ValidationError("name is required");
    validate_email(email_);
  }

  virtual std::string role() const = 0;

  // Accessors for tests
  int id() const { return id_; }
  const std::string& name() const { return name_; }
  const std::string& email() const { return email_; }
};