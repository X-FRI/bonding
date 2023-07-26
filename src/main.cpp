#include "spdlog/spdlog.h"
#include "structopt/app.hpp"

#include "include/cli.h"
#include "include/config.h"
#include "include/container.h"

using namespace bonding;

int
main(int argc, char ** argv)
{
  auto app = structopt::app("bonding");

  try
    {
      auto options = app.parse<cli::Command_Line_Args>(argc, argv);

      if (options.debug.value())
        {
          spdlog::set_level(spdlog::level::debug);
          spdlog::debug("Activate debug mode!");
        }

      container::Container::start(options.to_args());
    }
  catch (structopt::exception & e)
    {
      spdlog::error(e.what());
      spdlog::error(e.help());
    }
  catch (std::bad_optional_access & _)
    {
      spdlog::error(app.help());
    }
  catch (std::exception & e)
    {
      spdlog::error(e.what());
    }
  return 0;
}