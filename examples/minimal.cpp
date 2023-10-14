// #define N_ALL 10UL
// #define PHYSICS_TYPE PHYSICS_EULER_ISOTHERMAL

// #include "config.hpp"
// #include "defines.hpp"
#include "hydrie.hpp"

int main(void) {

    // const Config config = (Config){
    //     .sim_name = "minimal",
    //     .print_banner = true,
    //     .mesh_config = {.type = MESH_CARTESIAN, .xi_in = 1.0, .xi_out = 2.0},
    //     .state_config = {.type = PHYSICS_TYPE},
    //     .numflux_config = {.numflux_type = NUMFLUX_KT,
    //                     .recon_type = RECON_VAN_LEER},
    // };
    // return run_hydrie_with_config(&config);
    return run();
}
