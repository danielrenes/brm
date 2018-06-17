#ifndef SYSTEM_H
#define SYSTEM_H

#include "typedefs.h"

namespace Brm
{

#if _WIN32
  const PLATFORM OS = PLATFORM::WINDOWS;
#elif __linux__
  const PLATFORM OS = PLATFORM::LINUX;
#elif __APPLE__
  const PLATFORM OS = PLATFORM::MAC;
#else
  const PLATFORM OS = PLATFORM::UNKNOWN;
#endif

}

#endif
