#pragma once

#include "defines.hpp"
#include <string>

namespace hydrie {

struct MeshConfig {
    const MeshType type;
    const double xi_in;
    const double xi_out;
};

struct StateConfig {
    const double gamma;
};

struct NumfluxConfig {
    const LimiterType limiter_type;
};

struct Config {
    const std::string sim_name;
    const bool print_banner;
    const MeshConfig mesh_config;
    const StateConfig state_config;
    const NumfluxConfig numflux_config;
    const Boundaries boundaries;
};

} // namespace hydrie
