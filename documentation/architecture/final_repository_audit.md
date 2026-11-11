# NUMERIX repository audit

Audit date: 2026-11-11. The working tree already contained a broad, uncommitted NUMERIX migration; it was preserved and reviewed in place. The audit does not treat a passing smoke test as evidence of full feature completion.

## Evidence and limits

- `mingw32-make -B all` rebuilt the NUMERIX archive, CLI, three demos, and all test executables with `-Wall -Wextra -Wpedantic` under MinGW GCC and emitted no warnings.
- `mingw32-make test` passed 53 root-finding, 58 differentiation, 49 interpolation, 8 robustness, and 2 allocation-failure checks (170 counted checks), and ran all 18 NUMERIX phase executables successfully. Expression checks include power precedence/associativity, signed exponents, scientific notation, and rejection of malformed numeric literals.
- CLI smoke runs verified `--help`, `--version`, an expression-backed bisection root, expression-backed differentiation, and a clear error for an unknown command.
- These tests are mostly focused demonstrations and smoke checks. They do not establish full numerical correctness, memory safety, concurrency safety, or feature completeness.
- CMake initially failed because the allocation-failure test target did not include `include/`; after fixing that target, CMake configured, built, and ran all 24 CTest entries successfully with MinGW GCC on Windows. Hosted Linux GCC/Clang and MSVC jobs from run 42924291210 failed during CMake build; hosted job logs require repository admin rights here, and those toolchains remain unverified locally.

## Feature audit

| Area | Status | Current evidence and remaining work |
|---|---|---|
| Legacy root finding | VERIFIED | Bisection, Newton-Raphson, and secant in `src/root_finding.c`; 53 checks passed. |
| Legacy differentiation | VERIFIED | Forward, backward, and central finite differences in `src/differentiation.c`; 58 checks passed. |
| Legacy interpolation | VERIFIED | Linear, Lagrange, and Newton interpolation in `src/interpolation.c`; 49 checks plus allocation-failure checks passed. |
| NUMERIX API/core | PARTIALLY IMPLEMENTED | Public structures and initialization/status helpers exist. Phase 0/1 smoke tests pass; ownership, option validation, and consistent result-producing execution need broader coverage. |
| NUMERIX root wrappers | PARTIALLY IMPLEMENTED | Bisection, Newton-Raphson, and secant now evaluate the supplied callback/expression directly and no longer use shared global callback pointers. New phase tests cover callback roots and expression-backed bisection. Results remain scalar `double`s, and convergence metadata is not returned. |
| NUMERIX differentiation/interpolation adapters | PARTIALLY IMPLEMENTED | Differentiation directly evaluates callback/expression functions without shared global state; phase tests cover all three differences and expression-backed central difference. Interpolation wrappers still delegate to legacy implementations and need targeted new-API edge tests. |
| Linear algebra | PARTIALLY IMPLEMENTED | Vector dot/norm, matrix-vector multiply, partial-pivot Gaussian solve, and pivoted LU are in `src/algorithms/linear_algebra.c`. `test_advanced_algorithms.c` checks a pivoting solve, LU reconstruction, and singular detection. Dense-only; conditioning and broader scaling tests remain. |
| Integration | PARTIALLY IMPLEMENTED | Composite trapezoid and Simpson rules are in `src/algorithms/integration.c`. The advanced test checks `x^2` and Simpson panel validation. No adaptive error estimate or singularity handling. |
| Optimization | PARTIALLY IMPLEMENTED | Golden-section interval minimization is in `src/algorithms/optimization.c`. The advanced test checks a quadratic minimum and iteration exhaustion. Assumes unimodality and returns only the minimizer location. |
| ODE solving | PARTIALLY IMPLEMENTED | Fixed-step scalar RK4 is in `src/algorithms/ode.c`. The advanced test checks `y'=y`, backward-time integration, and step-limit handling. No adaptive step or error estimate. |
| Expression parser/evaluator | PARTIALLY IMPLEMENTED | Tokenizer, parser, AST, evaluator, and expression callback exist. Grammar supports standard unary/power precedence, right-associative powers, signed exponents, scientific notation, and a 1,024-character input cap. Phase 2 checks representative precedence and malformed literals. Deep resource exhaustion and broad function/domain/ownership testing remain. |
| Piecewise functions | PARTIALLY IMPLEMENTED | Segment storage/evaluation, breakpoint, validation, discontinuity and root-solver code exist. Basic evaluation passes phase 3; discontinuity classification and root solving have untested edge cases. |
| Method recommendation | PARTIALLY IMPLEMENTED | Rule-based recommendation exists and phase 4 smoke test passes; recommendations need eligibility and accuracy validation. |
| Fallback | PARTIALLY IMPLEMENTED | Fallback selection/history exists; phase 5 passes a basic scenario. |
| Parallel portfolio | NEEDS REFACTORING | Portfolio currently evaluates methods sequentially and records zero runtime. It is not parallel; the root wrappers' global callback state also prevents safe concurrent use. Phase 6 verifies only the sequential winner path. |
| Error detection/condition/sensitivity/convergence | PARTIALLY IMPLEMENTED | Helpers and smoke tests exist for each. Coverage does not yet establish the requested numerical diagnoses or robust thresholds. |
| Execution limits and step logging | PARTIALLY IMPLEMENTED | Data structures/helpers and smoke tests exist; limits are not consistently applied inside algorithm loops and cancellation is not generally wired into solvers. |
| CLI | PARTIALLY IMPLEMENTED | Interactive menu and command-line root, linear solve, interpolation, integration, differentiation, golden-section minimization, RK4 sample, piecewise evaluation, recommendation, sequential portfolio, comparison, accuracy, and sensitivity operations work. `--help` and `--version` work. Step traces are not connected to solver loops. |
| Benchmarking | PARTIALLY IMPLEMENTED | Root comparison now measures elapsed time and callback/expression evaluation counts; iteration counts are explicitly unavailable. Accuracy is measured only when a reference solution is supplied, otherwise residual is reported separately. GSL/BLAS/LAPACK and a separate legacy baseline remain unavailable; no standalone benchmark runner exists. |
| Build: Make | VERIFIED | The updated Makefile builds `lib/libnumerix.a`, CLI, demos, and tests; `make test` runs legacy, phase, and allocation-failure tests. Verified with MinGW GCC on Windows. |
| Build: CMake | VERIFIED | CMake defines the `numerix` static target, CLI, demos, and CTest entries. A MinGW GCC CMake build and all 24 CTest entries passed locally after adding the missing include path for the allocation-failure target. Linux GCC/Clang and MSVC hosted builds still need rerunning after the fix. |
| Portability/CI | PARTIALLY IMPLEMENTED | MinGW GCC build/test passed. Linux and MSVC builds were not run. A GitHub Actions workflow now configures GCC/Clang Linux and MSVC Windows jobs, but has not run in this workspace. |
| Documentation | PARTIALLY IMPLEMENTED | README now describes the supported subset and the major gaps; the audit records module evidence. API and algorithm references still need a full contract and accuracy review. |
| Repository cleanup | NEEDS REFACTORING | Legacy source/header remain required by the adapters and tests. Build artifacts are ignored; no source archive or generated-binary review has been completed. |

## Source and dependency map

- Public interfaces: `include/numerix.h` and compatibility interface `include/numerical_recipes.h`.
- Legacy algorithms: `src/root_finding.c`, `src/differentiation.c`, `src/interpolation.c`.
- NUMERIX adapters: `src/algorithms/`.
- Core, expression, piecewise, intelligence, execution, diagnostics, benchmarking, testing, and CLI code are grouped under the matching `src/` directories.
- Platform timing implementations are in `platform/windows/timing.c` and `platform/linux/timing.c`.
- The current implementation uses the C standard library and math library. GSL, BLAS, and LAPACK are optional CMake switches, but no adapter implementation invokes those libraries yet.
- Legacy files are retained because NUMERIX adapters currently depend on the legacy algorithms and existing public compatibility/tests depend on that header.

## Immediate priorities

1. Rerun Linux GCC/Clang and Windows MSVC CI after the CMake include-path fix.
2. Add stress, scale, conditioning, and failure-injection tests for new linear algebra, integration, optimization, and ODE implementations.
3. Replace placeholder metrics in comparison, accuracy, timing, and reliability reports with measurements from actual solver executions.
4. Wire resource limits, cancellation, piecewise solving, and step traces into algorithm execution.
5. Expand expression, ownership, discontinuity, and numerical edge-case validation; finish API and algorithm documentation for supported contracts.

## Cleanup performed in this audit pass

- Removed the tracked `build/interpolation.o` artifact when verifying `make clean`; it is a compiler object, not a source dependency. The current Makefile ignores generated build products, and a clean `mingw32-make all` rebuilt the project without it.
- No source or documentation files were deleted. Build outputs under `build/`, `bin/`, and `lib/` remain generated and ignored after verification; a distributable source archive has not been created.
