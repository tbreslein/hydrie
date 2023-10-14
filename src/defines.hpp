#pragma once

#include "errorhandling.hpp"
#include <Fastor/Fastor.h>
#include <string>
#include <variant>

namespace hydrie {

template <size_t N>
using Tensor1 = Fastor::Tensor<double, N>;

template <size_t M, size_t N>
using Tensor2 = Fastor::Tensor<double, M, N>;

template <size_t O, size_t M, size_t N>
using Tensor3 = Fastor::Tensor<double, O, M, N>;

enum MeshType { MeshCartesian };

std::string to_string(const MeshType x) {
    switch (x) {
        case MeshCartesian:
            return "MeshCartesian";
        default:
            UNHANDLED_CASE(x);
    }
}

enum PhysicsType {
    PhysicsEulerIsothermal,
    PhysicsEulerAdiabatic,
};

std::string to_string(const PhysicsType x) {
    switch (x) {
        case PhysicsEulerIsothermal:
            return "PhysicsEulerIsothermal";
        case PhysicsEulerAdiabatic:
            return "PhysicsEulerAdiabatic";
        default:
            UNHANDLED_CASE(x);
    }
}

enum LimiterType {
    LimiterNone,
    LimiterMinMod,
    LimiterMonocent,
    LimiterSuperbee,
    LimiterVanLeer,
};

std::string to_string(const LimiterType x) {
    switch (x) {
        case LimiterNone:
            return "LimiterNone";
        case LimiterMinMod:
            return "LimiterMinMod";
        case LimiterMonocent:
            return "LimiterMonocent";
        case LimiterSuperbee:
            return "LimiterSuperbee";
        case LimiterVanLeer:
            return "LimiterVanLeer";
        default:
            UNHANDLED_CASE(x);
    }
}

enum Direction {
    West = 0UL,
    East,
    Cent,
};

std::string to_string(const Direction x) {
    switch (x) {
        case West:
            return "West";
        case East:
            return "East";
        case Cent:
            return "Cent";
        default:
            UNHANDLED_CASE(x);
    }
}

#define NUM_DIR 3UL

// these two enums count up in powers of 2 starting from 2, because then we can
// combine them with directions West and East through | to build unique numbers.
// NOTE: Never | these with Cent!
enum BoundaryType {
    BoundaryNoGradients = 2UL,
};

std::string to_string(const BoundaryType x) {
    switch (x) {
        case BoundaryNoGradients:
            return "BoundaryNoGradients";
        default:
            UNHANDLED_CASE(x);
    }
}

enum BoundaryCustomType {
    BoundaryCustomNoGradients = 2UL,
    BoundaryCustomReflect = 4UL,
    BoundaryCustomNearZero = 8UL,
};

std::string to_string(const BoundaryCustomType x) {
    switch (x) {
        case BoundaryCustomNoGradients:
            return "BoundaryCustomNoGradients";
        case BoundaryCustomReflect:
            return "BoundaryCustomReflect";
        case BoundaryCustomNearZero:
            return "BoundaryCustomNearZero";
        default:
            UNHANDLED_CASE(x);
    }
}

#ifndef N_ALL
#define N_ALL 200UL
#endif

#ifndef N_GC
#define N_GC 2UL
#endif

#ifndef N_COMP
#define N_COMP (N_ALL - 2UL * N_GC)
#endif

#ifndef I_MIN
#define I_MIN 0UL
#endif

#ifndef I_MAX
#define I_MAX (I_MIN + N_ALL - 1UL)
#endif

#ifndef I_COMP_MIN
#define I_COMP_MIN (I_MIN + N_GC)
#endif

#ifndef I_COMP_MAX
#define I_COMP_MAX (I_MAX - N_GC)
#endif

#if PHYSICS_TYPE == PhysicsEulerIsothermal

#define J_ALL 3UL
#define J_RHO 0UL
#define J_XI 1UL
#define J_ETA 2UL
#define J_P 0UL

#elif PHYSICS_TYPE == PhysicsEulerAdiabatic

#define J_ALL 4UL
#define J_RHO 0UL
#define J_XI 1UL
#define J_ETA 2UL
#define J_P 3UL

#else
#error "PHYSICS_TYPE has invalid value!"
#endif

#define J_MAX J_ALL - 1UL
#define J_EIGEN_MIN 0UL
#define J_EIGEN_MAX J_MAX

using Boundaries =
    std::variant<BoundaryType, std::array<BoundaryCustomType, J_ALL>>;

} // namespace hydrie
