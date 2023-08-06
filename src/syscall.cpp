#include "include/syscall.h"
#include <algorithm>
#include <array>
#include <asm-generic/errno-base.h>
#include <libseccomp/seccomp.h>

namespace bonding::syscall
{
  Result<Unit, error::Err>
  Syscall::refuse_if_comp() noexcept
  {
    for (const auto [syscall, ind, biteq] : default_refuse_if_comp_syscalls)
      {
        const scmp_arg_cmp cmp = { ind, SCMP_CMP_MASKED_EQ, biteq, biteq };

        if (0 != seccomp_rule_add_array(ctx, SCMP_ACT_ERRNO(EPERM), syscall, 1, &cmp))
          return Err(
            error::Err(error::Code::SystemcallError, "seccomp_rule_add_array error"));
      }

    return Ok(Unit());
  }

  Result<Unit, error::Err>
  Syscall::refuse_syscall() noexcept
  {
    for (const int syscall : default_refuse_syscalls)
      if (0 != seccomp_rule_add(ctx, SCMP_ACT_ERRNO(EPERM), syscall, 0))
        return Err(error::Err(error::Code::SystemcallError, "seccomp_rule_add error"));

    return Ok(Unit());
  }

  Result<Unit, error::Err>
  Syscall::setup() noexcept
  {
    spdlog::debug("Refusing / Filtering unwanted syscalls");

    /* Initialize seccomp profile with all syscalls allowed by default */
    ctx = seccomp_init(SCMP_ACT_ALLOW);
    if (NULL == ctx)
      return Err(error::Err(error::Code::SystemcallError, "seccomp_init error"));

    refuse_syscall().unwrap();
    refuse_if_comp().unwrap();

    if (0 != seccomp_load(ctx))
      return Err(error::Err(error::Code::SystemcallError, "seccomp_load error"));

    return Ok(Unit());
  }

  Result<Unit, error::Err>
  Syscall::clean() noexcept
  {
    seccomp_release(ctx);

    return Ok(Unit());
  }
}
