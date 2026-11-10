# NUMERIX

NUMERIX is a C11 numerical library in active development. The repository keeps its original `numerical_recipes.h` API while adding a structured API in `include/numerix.h`.

## What works today

- Root finding: bisection, Newton-Raphson, and secant methods.
- Numerical differentiation: forward, backward, and central finite differences.
- Interpolation: linear, Lagrange, and Newton divided-difference interpolation.
- Dense linear algebra: vector dot product and norm, matrix-vector multiplication, Gaussian elimination with partial pivoting, and pivoted LU factorization.
- Numerical integration: composite trapezoid and composite Simpson rules.
- Scalar optimization: golden-section minimization on a finite interval.
- Initial value ODE solving: fixed-step fourth-order Runge-Kutta (RK4).
- Callback and expression-backed scalar functions, with basic piecewise evaluation.
- Early-stage recommendation, fallback, comparison, diagnostics, and execution helper APIs.
- Three legacy API demonstrations and a NUMERIX command-line interface.

The expression engine supports arithmetic, powers, and a limited set of scalar functions. The structured recommendation, comparison, diagnostics, and piecewise layers have basic implementations and smoke tests; they are not yet a complete intelligent execution system. The CLI executes the implemented scalar algorithms and sample linear/ODE operations; it reports unsupported step tracing. The portfolio runs sequentially, and external-library comparison does not invoke GSL, BLAS, or LAPACK. New dense linear algebra and ODE routines need broader scale and stability testing beyond their analytical unit cases.

See [the repository audit](documentation/architecture/final_repository_audit.md) for evidence and module-by-module limitations.

## Build

Requirements: a C11 compiler and GNU Make. The build has been exercised with the MinGW GCC toolchain available in this workspace. CMake support is provided but still needs verification in an environment with CMake installed.

Windows with MinGW:

```sh
mingw32-make all
mingw32-make test
mingw32-make test-memory
```

Linux with GCC or Clang and GNU Make:

```sh
make all
make test
make test-memory
```

The primary archive is `lib/libnumerix.a`; executables are written under `bin/`. `make test` runs the legacy tests, NUMERIX phase tests, and allocation-failure test. `make clean` removes generated build, binary, and library outputs.

CMake workflow:

```sh
cmake -S . -B build-cmake
cmake --build build-cmake
ctest --test-dir build-cmake --output-on-failure
```

## Command-line use

```sh
bin/numerix_cli --help
bin/numerix_cli root --expr "x^2 - 2" --a 1 --b 2 --tol 1e-10
bin/numerix_cli differentiate --expr "x^3" --x 2
bin/numerix_cli integrate --expr "x^2" --a 0 --b 1
bin/numerix_cli minimize --expr "(x-1)^2" --a 0 --b 2
```

The CLI also accepts `linear`, `ode`, `piecewise`, `interpolate`, `recommend`, `portfolio`, `compare`, `accuracy`, and `sensitivity`. Step tracing is not yet connected to solver loops.

## Use the NUMERIX API

```c
#include <math.h>
#include "numerix.h"

static double f(double x, void *context)
{
    (void)context;
    return x * x - 2.0;
}

int main(void)
{
    NUMERIX_Function function;
    if (numerix_function_init(&function, f, NULL, NULL) != NUMERIX_STATUS_OK)
        return 1;

    double root = numerix_bisection(&function, 0.0, 2.0, 1e-8);
    return isfinite(root) ? 0 : 1;
}
```

Compile and link with GCC:

```sh
gcc -std=c11 -Iinclude example.c lib/libnumerix.a -lm -o example
```

Numerical routines returning `double` use `NAN` for detected invalid input or failure. Check results with `isfinite()`. A finite approximation alone does not certify an error bound. Callback context remains caller-owned. Expression functions own their parsed expression data and should be released with `numerix_function_destroy()`.

## Legacy API

`include/numerical_recipes.h` and the three original demos remain supported during migration. They provide the same root finding, differentiation, and interpolation methods using scalar callbacks and `NAN` failure returns.

## Repository map

- `include/` — public NUMERIX and legacy headers.
- `src/` — legacy algorithms and NUMERIX modules.
- `platform/` — Windows and Linux timing implementations.
- `tests/` — legacy robustness/correctness tests and NUMERIX phase tests.
- `examples/` — legacy API demonstrations.
- `documentation/` — API, algorithm, test, and architecture notes.

## Verification record

The available MinGW GCC run passed 168 legacy correctness/robustness checks, two allocation-failure checks, the advanced-algorithm analytical suite, and all 18 phase test executables. The phase executables include smoke tests and are not a substitute for full numerical coverage. CMake/CTest, Linux builds, sanitizers, large-matrix conditioning, and a real parallel portfolio have not yet been verified. See the audit for detail.
