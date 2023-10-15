#pragma once

#include <fmt/format.h>
#include <iostream>

namespace hydrie {

#define UNHANDLED_CASE(type)                                                   \
    {                                                                          \
        fmt::print(stderr, "Error@{}:{}: Unhandled case for {} = %{}\n",       \
                   __FILE__, __LINE__, #type, to_string(type));                \
        exit(1);                                                               \
    }

#define ERROR(msg)                                                             \
    {                                                                          \
        fmt::print(stderr, "Error@{}:{}: {}\n", __FILE__, __LINE__, msg);      \
        exit(1);                                                               \
    }

#define TODO(msg)                                                              \
    {                                                                          \
        fmt::print(stderr, "TODO@{}:{}: {}\n", __FILE__, __LINE__, msg);       \
        exit(1);                                                               \
    }

} // namespace hydrie
