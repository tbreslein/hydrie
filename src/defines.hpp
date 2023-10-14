#pragma once

enum MeshType { MeshCartesian };

enum PhysicsType {
    PhysicsEulerIsothermal,
    PhysicsEulerAdiabatic,
};

enum LimiterType {
    LimiterNone,
    LimiterMinMod,
    LimiterMonocent,
    LimiterSuperbee,
    LimiterVanLeer,
};

enum Direction {
    West = 0UL,
    East,
    Cent,
};

#define NUM_DIR 3UL

// these two enums count up in powers of 2 starting from 2, because then we can
// combine them with directions West and East through | to build unique numbers.
// NOTE: Never | these with Cent!
enum BoundaryType {
    BoundaryCustom = 2UL,
    BoundaryNoGradients = 4UL,
};

enum BoundaryCustomType {
    BoundaryCustomNoGradients = 2UL,
    BoundaryCustomReflect = 4UL,
    BoundaryCustomNearZero = 8UL,
};

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
