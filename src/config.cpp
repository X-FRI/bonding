#include "include/config.h"

#include <iterator>
#include <sstream>

namespace bonding::config
{
  Result<std::vector<std::string>, Unit>
  Container_Options::parse_argv(const std::string & argv) noexcept
  {
    std::istringstream argv_stream(argv);
    return Ok(std::vector<std::string>((std::istream_iterator<std::string>(argv_stream)),
                                       std::istream_iterator<std::string>()));
  }
}