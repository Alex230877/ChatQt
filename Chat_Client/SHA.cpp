#include "SHA.h"
#include <memory>
#include "sha1.hpp"

std::string sha_1::hash(const std::string& value)
{
    auto checksum = std::make_shared<SHA1>();
    checksum->update(value);
    return checksum->final();
}
