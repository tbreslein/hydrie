#pragma once

#include "config.hpp"

namespace hydrie {

struct Mesh {
    MeshType type;
    double dxi;
    double deta;
    double dphi;
    double inv_dxi;
    Tensor1<N_ALL> sqrt_g;
    Tensor1<N_ALL> volume;
    Tensor1<N_ALL> deta_dphi_div_volume;
    Tensor2<2, N_ALL> face_dist;
    Tensor2<2, N_ALL> darea_xi_deta_dphi;
    Tensor2<NUM_DIR, N_ALL> xi;
    Tensor2<NUM_DIR, N_ALL> h_xi;
    Tensor2<NUM_DIR, N_ALL> h_eta;
    Tensor2<NUM_DIR, N_ALL> h_phi;
};

auto init_mesh(const MeshConfig &mesh_config) noexcept -> Mesh {
    auto dxi = 1.0;
    auto deta = 1.0;
    auto dphi = 1.0;
    switch (mesh_config.type) {
        case MeshCartesian:
            dxi = (mesh_config.xi_out - mesh_config.xi_in) / N_ALL;
            break;
        default:
            UNHANDLED_CASE(mesh_config.type);
    }

    Tensor2<NUM_DIR, N_ALL> xi(0.0);
    for (size_t i = 0; i < N_ALL; i++) {
        xi(West, i) = mesh_config.xi_in +
                      dxi * static_cast<double>(i - static_cast<int>(2));
    }
    xi(Cent, Fastor::all) = xi(West, Fastor::all) + 0.5 * dxi;
    xi(East, Fastor::all) = xi(West, Fastor::all) + dxi;

    Tensor2<NUM_DIR, N_ALL> h_xi(1.0);
    Tensor2<NUM_DIR, N_ALL> h_eta(1.0);
    Tensor2<NUM_DIR, N_ALL> h_phi(1.0);
    switch (mesh_config.type) {
        case MeshCartesian:
            break;
        default:
            UNHANDLED_CASE(mesh_config.type);
    }

    Tensor2<2, N_ALL> face_dist, darea_xi_deta_dphi;
    for (int k = 0; k < Cent; k++)
        for (int i = 0; i < static_cast<int>(N_ALL); i++) {
            face_dist(k, i) = (xi(k, i) - xi(Cent, i));
        }

    Tensor1<N_ALL> sqrt_g, volume, deta_dphi_div_volume;

    return Mesh{
        .type = mesh_config.type,
        .dxi = dxi,
        .deta = deta,
        .dphi = dphi,
        .inv_dxi = 1.0 / dxi,
        .sqrt_g = std::move(sqrt_g),
        .volume = std::move(volume),
        .deta_dphi_div_volume = std::move(deta_dphi_div_volume),
        .face_dist = std::move(face_dist),
        .darea_xi_deta_dphi = std::move(darea_xi_deta_dphi),
        .xi = std::move(xi),
        .h_xi = std::move(h_xi),
        .h_eta = std::move(h_eta),
        .h_phi = std::move(h_phi),
    };
}

} // namespace hydrie
