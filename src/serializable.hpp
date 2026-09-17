#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include <yaml-cpp/yaml.h>

struct Serializable {
  virtual ~Serializable() = default;
  virtual nlohmann::json to_json() const = 0;
  virtual YAML::Node     to_yaml() const = 0;
  virtual std::string    csv_header() const = 0;
  virtual std::string    csv_row() const = 0;
};