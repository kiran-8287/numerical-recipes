# NUMERIX architecture

## Public interfaces

`include/numerix.h` is the primary public C API. It defines callback/expression functions, problems, options, results, status values, and the implemented solver and analysis interfaces. `include/numerical_recipes.h` remains as a compatibility interface for the original scalar routines.

The compatibility API and NUMERIX API are compiled into one static archive, `libnumerix.a`. The legacy algorithms are retained because the compatibility interface and some NUMERIX interpolation wrappers still depend on them.

## Source layout

```text
include/                 Public headers
src/core/                Function, problem, option, result, and status helpers
src/algorithms/          Root finding, differences, interpolation, algebra, integration, optimization, ODE
src/expression/          Tokenizer, parser, AST, and evaluator
src/piecewise/           Piecewise segment and breakpoint support
src/intelligence/        Recommendation and fallback helpers
src/execution/           Portfolio, step log, and resource-limit helpers
src/diagnostics/         Error, sensitivity, condition, and convergence helpers
src/benchmarking/        Comparison and reporting helpers
src/cli/                 CLI dispatch and executable entry point
src/testing/             Lightweight validation-suite helpers
src/                     Legacy scalar algorithms
platform/                Operating-system timing implementations
tests/                   Legacy tests and NUMERIX phase tests
examples/                Legacy API demonstrations
```

## Execution and ownership

The callback function context remains caller-owned. Parsed expressions allocate an AST; `numerix_function_destroy` releases it. Piecewise segments borrow their function objects and contexts. Destroy the piecewise container with `numerix_piecewise_destroy`, and keep any borrowed callbacks and their contexts alive for the duration of evaluation.

The scalar NUMERIX root, difference, integration, and optimization routines evaluate callbacks directly. Root and scalar objective routines return `NAN` for detected invalid inputs, non-finite evaluations, or failure to converge. RK4 and linear algebra return status codes. These routines do not currently return a unified `NUMERIX_Result`, and problem options/limits are not consistently connected to solver loops.

## Build and test arrangement

GNU Make and CMake compile the legacy and NUMERIX modules into the `numerix` static library. The CLI and examples link against it. The test programs remain separate executables. `make test` runs every regular test and the isolated allocation-failure test; CTest discovers the same test sources when built with CMake.

## Current boundaries

The first dense linear algebra, composite integration, golden-section, and scalar RK4 routines are callable and have analytical unit cases, but they have not had scale, conditioning, sanitizer, or cross-platform validation. The portfolio is sequential. Comparison reports measure elapsed time and function calls, but do not instrument iterations. Limits and cancellation are not consistently enforced by solver loops. External library adapters and a standalone benchmark runner remain unavailable. Track verified functionality in [the final repository audit](architecture/final_repository_audit.md).
