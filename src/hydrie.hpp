#pragma once

#include "config.hpp"
#include "mesh.hpp"
#include <fmt/format.h>
#include <iostream>

namespace hydrie {

struct Hydrie {
    const Mesh mesh;

    static auto init(const Config &config) noexcept -> Hydrie {
        return Hydrie{.mesh = init_mesh(config.mesh_config)};
    }

    int run(void) {
        std::cout << fmt::format("dxi = {}\n", this->mesh.dxi);
        return 0;
    }
};

} // namespace hydrie
