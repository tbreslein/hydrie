#pragma once

#include "config.hpp"
#include "mesh.hpp"
#include <Fastor/Fastor.h>
#include <format>
#include <iostream>

namespace hydrie {

struct Hydrie {
    const Mesh mesh;

    static auto init(const Config &config) noexcept -> Hydrie {
        return Hydrie{.mesh = init_mesh(config.mesh_config)};
    }

    int run(void) {
        std::cout << std::format("dxi = {}\n", this->mesh.dxi);
        return 0;
    }
};

} // namespace hydrie
