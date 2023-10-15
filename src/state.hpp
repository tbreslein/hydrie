#pragma once

#include "config.hpp"
#include "defines.hpp"
#include <limits.h>

namespace hydrie {

template <auto Start, auto End, auto Inc, class F>
constexpr void constexpr_for(F &&f) {
    if constexpr (Start < End) {
        f(std::integral_constant<decltype(Start), Start>());
        constexpr_for<Start + Inc, End, Inc>(f);
    }
}

template <PhysicsType P>
struct State {
    constexpr static const std::size_t j_rho{[]() -> auto {
        switch (P) {
            case PhysicsEulerIsothermal:
            case PhysicsEulerAdiabatic:
                return 0;
        }
    }()};
    constexpr static const std::size_t j_xi{[]() -> auto {
        switch (P) {
            case PhysicsEulerIsothermal:
            case PhysicsEulerAdiabatic:
                return 1;
        }
    }()};
    constexpr static const std::size_t j_eta{[]() -> auto {
        switch (P) {
            case PhysicsEulerIsothermal:
            case PhysicsEulerAdiabatic:
                return 2;
        }
    }()};
    constexpr static const std::size_t j_p{[]() -> auto {
        switch (P) {
            case PhysicsEulerIsothermal:
                return std::numeric_limits<size_t>::max();
            case PhysicsEulerAdiabatic:
                return 3;
        }
    }()};
    constexpr static const std::size_t j_all{[]() -> auto {
        switch (P) {
            case PhysicsEulerIsothermal:
                return 3;
            case PhysicsEulerAdiabatic:
                return 4;
        }
    }()};
    constexpr static const std::size_t j_max{j_all - 1};
    constexpr static const std::size_t j_eigen_min{0};
    constexpr static const std::size_t j_eigen_max{j_max};

    const double gamma;
    constexpr static const bool is_isothermal{[]() -> auto {
        switch (P) {
            case PhysicsEulerIsothermal:
                return true;
            case PhysicsEulerAdiabatic:
                return false;
        }
    }()};
    constexpr static const bool is_adiabatic{
        []() -> auto { return !is_isothermal; }()};

    Tensor2<NUM_DIR, N_ALL> csound;
    Tensor3<NUM_DIR, j_all, N_ALL> p;
    Tensor3<NUM_DIR, j_all, N_ALL> c;
    Tensor3<NUM_DIR, j_all, N_ALL> eigen;
    Tensor3<NUM_DIR, j_all, N_ALL> flux;

    template <size_t D = Cent, typename Prim, typename Cons>
    auto update_prim(Prim &p, const Cons &c) const noexcept -> void {
        row<j_rho>(p[D]) = row<j_rho>(c[D]);
        row<j_xi>(p[D]) = row<j_xi>(c[D]) / row<j_rho>(c[D]);
        row<j_eta>(p[D]) = row<j_eta>(c[D]) / row<j_rho>(c[D]);
        if constexpr (P == PhysicsEulerAdiabatic) {
            const double gamma_m = this->gamma - 1.0;
            row<j_p>(p[D]) =
                gamma_m *
                (row<j_p>(c[D]) - 0.5 * (row<j_xi>(p[D]) * row<j_xi>(c[D]) +
                                         row<j_eta>(p[D]) * row<j_eta>(c[D])));
        }
    }

    template <size_t D = Cent, typename Cons, typename Prim>
    auto update_cons(Cons &c, const Prim &p) const noexcept -> void {
        row<j_rho>(c[D]) = row<j_rho>(p[D]);
        row<j_xi>(c[D]) = row<j_xi>(p[D]) * row<j_rho>(p[D]);
        row<j_eta>(c[D]) = row<j_eta>(p[D]) * row<j_rho>(p[D]);
        if constexpr (P == PhysicsEulerAdiabatic) {
            const double a = 1.0 / (this->gamma - 1.0);
            row<j_p>(p[D]) = a * row<j_p>(c[D]) +
                             0.5 * (row<j_xi>(p[D]) * row<j_xi>(c[D]) +
                                    row<j_eta>(p[D]) * row<j_eta>(c[D]));
        }
    }

    template <size_t D = Cent, typename CSound, typename Prim>
    auto update_csound(CSound &csound, const Prim &p) const noexcept -> void {
        if constexpr (is_adiabatic) {
            csound[D] =
                blaze::sqrt(this->gamma * row<j_p>(p[D]) / row<j_rho>(p[D]));
        }
        // no op for isothermal
    }

    template <size_t D = Cent, typename Eigen, typename VXi, typename CSound>
    auto update_eigen(Eigen &eigen, const VXi &vxi,
                      const CSound &cs) const noexcept -> void {
        row<j_eigen_min>(eigen[D]) = vxi - cs;
        row<j_eigen_max>(eigen[D]) = vxi + cs;
        constexpr_for<1, j_eigen_max - 1>(
            [&](auto j) { row<j>(eigen[D]) = vxi; });
    }

    template <size_t D = Cent, typename Flux, typename Cons, typename Prim,
              typename CS>
    auto update_flux(Flux &flux, const Cons &c, const Prim &p,
                     const CS &cs) const noexcept -> void {
        row<j_rho>(flux[D]) = row<j_rho>(c[D]);
        row<j_eta>(flux[D]) = row<j_xi>(p[D]) * row<j_eta>(c[D]);

        if constexpr (P == PhysicsEulerAdiabatic) {
            row<j_xi>(flux[D]) =
                row<j_xi>(c[D]) * row<j_xi>(p[D]) + row<j_p>(p[D]);
            row<j_p>(flux[D]) =
                (row<j_p>(p[D]) + row<j_p>(c[D])) * row<j_xi>(p[D]);
        } else if constexpr (P == PhysicsEulerIsothermal) {
            row<j_xi>(flux[D]) =
                row<j_rho>(c[D]) *
                (row<j_xi>(p[D]) * row<j_xi>(p[D]) + cs[D] * cs[D]);
        }
    }
};

template <PhysicsType P>
auto init_state(const StateConfig &state_config) noexcept -> State<P> {
    return State<P>{
        .gamma = state_config.gamma,
        .csound = Tensor2<NUM_DIR, N_ALL>(),
        .p = Tensor3<NUM_DIR, State<P>::j_all, N_ALL>(),
        .c = Tensor3<NUM_DIR, State<P>::j_all, N_ALL>(),
        .eigen = Tensor3<NUM_DIR, State<P>::j_all, N_ALL>(),
        .flux = Tensor3<NUM_DIR, State<P>::j_all, N_ALL>(),
    };
}

} // namespace hydrie
