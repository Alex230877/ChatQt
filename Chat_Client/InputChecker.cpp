#include "InputChecker.h"

namespace {
  const uint8_t MIN_LENGTH_USER_INPUT = 3;
}

uint8_t InputChecker::getMinLength()
{
  return MIN_LENGTH_USER_INPUT;
}

bool InputChecker::isTooShort(const std::string& input)
{
  if (input.length() < MIN_LENGTH_USER_INPUT){
    return true;
  }
  return false;
}

bool InputChecker::isContainsForbidenChars(const std::string& input)
{
  const std::string permissionedChars =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  size_t pos = input.find_first_not_of(permissionedChars);
  if (pos != std::string::npos) {
    return true;
  }
  return false;
}

bool InputChecker::isStartsByNumber(const std::string& input)
{
  const std::string numbers = "0123456789";
  size_t pos = input.find_first_of(numbers);
  if (pos == 0) {
    return true;
  }
  return false;
}
