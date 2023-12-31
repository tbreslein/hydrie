#pragma once

#include "errorhandling.hpp"
#include <blaze/math/StaticMatrix.h>
#include <blaze/math/StaticVector.h>
#include <string>
#include <variant>

namespace hydrie {

using blaze::column;
using blaze::columns;
using blaze::row;
using blaze::rows;

template <size_t N>
using Tensor1 = blaze::StaticVector<double, N>;

template <size_t M, size_t N>
using Tensor2 = blaze::StaticMatrix<double, M, N>;

template <size_t O, size_t M, size_t N>
using Tensor3 = std::array<Tensor2<M, N>, O>;

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
constexpr const size_t NUM_DIR = 3;

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

using Boundaries = std::variant<BoundaryType, std::vector<BoundaryCustomType>>;

} // namespace hydrie
