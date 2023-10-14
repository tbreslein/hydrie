#define N_ALL 10UL
#define PHYSICS_TYPE PhysicsEulerIsothermal

#include "hydrie.hpp"

int main(void) {
    const auto config = hydrie::Config{
        .sim_name = "minimal",
        .print_banner = true,
        .mesh_config = {.type = hydrie::MeshCartesian,
                        .xi_in = 1.0,
                        .xi_out = 2.0},
        .state_config = {.gamma = 1.0},
        .numflux_config = {.limiter_type = hydrie::LimiterVanLeer},
        .boundaries = {hydrie::BoundaryNoGradients},
    };
    return hydrie::Hydrie::init(config).run();
}
