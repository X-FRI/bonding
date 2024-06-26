/** Copyright (C) 2023 Muqiu Han <muqiu-han@outlook.com> */

#include "include/capabilities.h"
#include "include/log.hpp"
#include "include/unix.h"
#include <linux/prctl.h>
#include <sys/prctl.h>

namespace bonding::capabilities
{
  Result<Void, error::Err> Capabilities::setup() noexcept
  {
    for (const int drop_caps : DROP)
      if (-1 == prctl(PR_CAPBSET_DROP, drop_caps, 0, 0, 0))
        return ERR(error::Code::Capabilities);

    unix::Capabilities::Get_proc()
      .and_then([](cap_t cap) {
      unix::Capabilities::Set_flag(
        cap, CAP_INHERITABLE, static_cast<int>(DROP.size()), &DROP[0], CAP_CLEAR)
        .and_then([&](const Void _) { return unix::Capabilities::Set_proc(cap); })
        .and_then([&](const Void _) {
        return unix::Capabilities::Free(cap);
      }).or_else([&](const error::Err & err) {
        if (nullptr != cap)
          unix::Capabilities::Free(cap).unwrap();

        return ERR_MSG(error::Code::Capabilities, err.to_string());
      });

      return Ok(cap);
    }).unwrap();

    LOG_INFO << "Clearing unwanted capabilities...✓";
    return Ok(Void());
  }
} // namespace bonding::capabilities