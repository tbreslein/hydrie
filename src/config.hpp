#pragma once
#include "defines.hpp"
#include <string>

namespace hydrie {

struct MeshConfig {
    MeshType type;
    double xi_in;
    double xi_out;
};

struct StateConfig {
    double gamma;
};

struct NumfluxConfig {
    LimiterType limiter_type;
};

struct Config {
    std::string sim_name;
    bool print_banner;
    MeshConfig mesh_config;
    StateConfig state_config;
    NumfluxConfig numflux_config;
    Boundaries boundaries;
};

} // namespace hydrie
