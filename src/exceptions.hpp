#pragma once
#include <stdexcept>
struct ValidationError    : std::runtime_error { using std::runtime_error::runtime_error; };
struct SerializationError : std::runtime_error { using std::runtime_error::runtime_error; };