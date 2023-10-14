#pragma once

#include <format>
#include <iostream>

namespace hydrie {

#define UNHANDLED_CASE(type)                                                   \
    {                                                                          \
        std::cerr << std::format("Error@{}:{}: Unhandled case for {} = %{}\n", \
                                 __FILE__, __LINE__, #type, to_string(type));  \
        exit(1);                                                               \
    }

#define ERROR(msg)                                                             \
    {                                                                          \
        std::cerr << std::format("Error@{}:{}: {}\n", __FILE__, __LINE__,      \
                                 msg);                                         \
        exit(1);                                                               \
    }

#define TODO(msg)                                                              \
    {                                                                          \
        std::cerr << std::format("TODO@{}:{}: {}\n", __FILE__, __LINE__, msg); \
        exit(1);                                                               \
    }

} // namespace hydrie
