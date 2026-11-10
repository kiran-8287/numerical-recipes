# Testing and verification

## Test organization

The test programs are standalone executables that link to `libnumerix.a`, except `test_allocation_failure`, which links an isolated build of the legacy interpolation source compiled with a `malloc` substitution.

| Test file or group | Coverage |
|---|---|
| `test_root_finding.c` | Legacy bisection, Newton-Raphson, secant, invalid inputs, and non-finite behavior |
| `test_differentiation.c` | Legacy forward, backward, and central differences, invalid inputs, and non-finite behavior |
| `test_interpolation.c` | Legacy linear, Lagrange, and Newton interpolation and boundary/error cases |
| `test_robustness.c` | Larger inputs, unchanged arrays, repeated calls, allocation-size overflow, and duplicate nodes |
| `test_allocation_failure.c` | Deterministic allocation failure and successful retry |
| `test_advanced_algorithms.c` | Analytical checks for dense vectors/matrices, partial pivoting, LU reconstruction, composite integrals, golden-section minimization, and forward/backward RK4 |
| `test_phase*.c` | NUMERIX core, adapters, expressions, piecewise helpers, recommendation/fallback, portfolio, diagnostics, reports, limits, validation, and CLI |

Phase tests are integration/smoke tests, not full verification of every behavior in their module. In particular they do not establish the correctness of the accuracy, timing, reliability, or numerical condition metrics.

## Current recorded run

Environment: Windows workspace, MinGW GCC (`gcc`), `mingw32-make`. The exact output from the current `mingw32-make test` run is saved in [`evidence/current_make_test.txt`](evidence/current_make_test.txt).

| Group | Passed | Failed |
|---|---:|---:|
| Legacy root finding | 53 | 0 |
| Legacy differentiation | 58 | 0 |
| Legacy interpolation | 49 | 0 |
| Legacy robustness | 8 | 0 |
| Allocation-failure injection | 2 | 0 |
| **Counted assertions** | **170** | **0** |
| Advanced algorithm suite | 1 successful executable | 0 failed exits |
| NUMERIX phase executables | 18 successful exits | 0 failed exits |

`mingw32-make -B all` rebuilt the static library, CLI, demos, and tests with `-Wall -Wextra -Wpedantic` and no warnings. CLI runs of help, version, expression-backed root finding, expression-backed differentiation, and an unsupported command were also checked manually.

## Reproduce

Windows with MinGW:

```sh
mingw32-make all
mingw32-make test
mingw32-make test-memory
```

Linux with GNU Make:

```sh
make all
make test
make test-memory
```

CMake/CTest:

```sh
cmake -S . -B build-cmake
cmake --build build-cmake --parallel
ctest --test-dir build-cmake --output-on-failure
```

CMake/CTest could not be executed in the current workspace because CMake is not installed. Linux and MSVC builds have not been run locally. A GitHub Actions workflow was added to run GCC and Clang on Linux and MSVC on Windows; that workflow remains unverified until CI executes.

## Sanitizers

Sanitizer validation is outstanding for the current NUMERIX modules. Historical documentation mentioned sanitizer runs on the legacy code, but no current sanitizer log was available for this audit; no current sanitizer pass is claimed.
