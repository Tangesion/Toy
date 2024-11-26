
#pragma once
#include <stdexcept>
#include <iostream>

namespace inference_frame::common
{

    [[noreturn]] inline void throwRuntimeError(char const *const file, int const line, std::string const &info = "")
    {
        throw std::runtime_error{file + std::string(":") + std::to_string(line) + " " + info};
    }

#define LIKELY(x) __builtin_expect((x), 1)
#define UNLIKELY(x) __builtin_expect((x), 0)

#define CHECK_WITH_INFO(val, info)                                              \
    do                                                                          \
    {                                                                           \
        LIKELY(static_cast<bool>(val))                                          \
        ? static_cast<void>(0)                                                  \
        : inference_frame::common::throwRuntimeError(__FILE__, __LINE__, info); \
    } while (0)

#define JUST_THROW(info)                                                      \
    \  
    do                                                                        \
    {                                                                         \
        inference_frame::common::throwRuntimeError(__FILE__, __LINE__, info); \
    }                                                                         \
    while (0)
}
