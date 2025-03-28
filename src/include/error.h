/** Copyright (C) 2023 Muqiu Han <muqiu-han@outlook.com> */

#ifndef BONDING_ERROR_H
#define BONDING_ERROR_H

#include <cerrno>
#include <cstdint>
#include <exception>
#include <map>
#include <string>
#include <utility>
#include <expected>
#include "logging.h"

namespace bonding::error
{
  enum class Code
  {
    Undefined,
    Socket,
    ChildProcess,
    Container,
    Hostname,
    Mounts,
    Namespace,
    Systemcall,
    Cgroups,
    Exec,
    Capabilities,
    Unix,
    Environment,
    Cli,
    Configfile,
  };

  inline const std::map<Code, std::string> CODE_TO_STRING = {
    {Code::Undefined, "Unknown Error"},
    {Code::Socket, "Socket Error"},
    {Code::ChildProcess, "Child Process Error"},
    {Code::Container, "Container Error"},
    {Code::Hostname, "Hostname Error"},
    {Code::Mounts, "Mounts Error"},
    {Code::Namespace, "Namespace Error"},
    {Code::Systemcall, "System Call Error"},
    {Code::Cgroups, "Cgroups Error"},
    {Code::Exec, "Execute Error"},
    {Code::Cli, "Cli Error"},
    {Code::Environment, "Envronment Error"},
    {Code::Capabilities, "Capabilities Error"},
    {Code::Unix, "Unix Error"},
    {Code::Configfile, "Config File Error"},
  };

  class Err
  {
  public:
    Err()
    {
      // Err object is an unrecoverable error, so we terminate the program.
      std::terminate();
    }

    Err(
      const Code     code,
      std::string    custom,
      const uint32_t line,
      std::string    file,
      std::string    function)
      : m_code(code)
      , m_errno(errno)
      , m_line(line)
      , m_custom(std::move(custom))
      , m_function(std::move(function))
      , m_file(std::move(file))
    {
      LOG_ERROR << to_string() << ":" << function;
      LOG_INFO << "In the " << function << " function on line " << line << " of the file "
               << file << " (" << file << ":" << line << ")";
    }

    [[nodiscard]] static int32_t to_exit_code() noexcept;
    [[nodiscard]] std::string    to_string() const noexcept;

  private:
    Code m_code;

    int      m_errno;
    uint32_t m_line;

    std::string m_custom;
    std::string m_function;
    std::string m_file;
  };

} // namespace bonding::error

/** Some macros to create an Err object. */
#define ERR(CODE) bonding::error::Err(CODE, "", __LINE__, __FILE__, __FUNCTION__)

#define ERR_MSG(CODE, MSG)                                                               \
  bonding::error::Err(CODE, MSG, __LINE__, __FILE__, __FUNCTION__)

#endif /* BONDING_ERROR_H */
