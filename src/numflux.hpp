#pragma once

#include "config.hpp"
#include "defines.hpp"
#include "mesh.hpp"
#include "state.hpp"
#include <limits.h>

namespace hydrie {

template <PhysicsType P>
struct Numflux {
    const LimiterType limiter_type;
    Tensor2<NUM_DIR, N_ALL> a_plus;
    Tensor2<NUM_DIR, N_ALL> a_minus;
    Tensor2<NUM_DIR, N_ALL> b;
    Tensor2<NUM_DIR, N_ALL> c;
    Tensor3<NUM_DIR, State<P>::j_all, N_ALL> flux_num;
    Tensor3<NUM_DIR, State<P>::j_all, N_ALL> flux;

  private:
    auto reconstruct(State<P> &u, const Mesh &mesh) const;

    template <Direction D>
    auto prep_u(State<P> &u) const {
        u.update_cons<D>(u.c[D], u.p[D]);
        u.update_csound<D>(u.csound[D], u.p[D]);
        u.update_eigen<D>(u.eigen[D], row<u.j_xi>(u.p[D]), u.csound[D]);
        u.update_flux<D>(u.flux[D], u.c[D], u.p[D], u.csound[D]);
    }

  public:
    auto update_flux(State<P> &u, const Mesh &mesh) {
        this->reconstruct(u, mesh);
        this->prep_u<West>(u);
        this->prep_u<East>(u);

        // calc a_plus, a_minus, b, c
        // calc flux_num
        // calc flux
    }
};

template <PhysicsType P>
auto init_numflux(const NumfluxConfig &numflux_config) noexcept -> Numflux<P> {
    return Numflux<P>{
        .limiter_type = numflux_config.limiter_type,
        .a_plus = Tensor2<NUM_DIR, N_ALL>(),
        .a_minus = Tensor2<NUM_DIR, N_ALL>(),
        .b = Tensor2<NUM_DIR, N_ALL>(),
        .c = Tensor2<NUM_DIR, N_ALL>(),
        .flux_num = Tensor3<NUM_DIR, State<P>::j_all, N_ALL>(),
        .flux = Tensor3<NUM_DIR, State<P>::j_all, N_ALL>(),
    };
}
} // namespace hydrie
