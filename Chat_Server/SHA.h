#ifndef SHA_H
#define SHA_H

#include <string>

namespace sha_1 {
/**
  Calculate hash sha_1
  \param[in] value Input
  \return Hash
  */
std::string hash(const std::string& value);
}

#endif
