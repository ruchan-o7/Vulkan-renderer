#pragma once
#include <cassert>

#include "../log.h"

#define VK_CHECK(call)                        \
  do {                                        \
    VkResult result_ = call;                  \
    CORE_WARNING("Vk call did not succeded"); \
    assert(result_ == VK_SUCCESS);            \
  } while (0)
