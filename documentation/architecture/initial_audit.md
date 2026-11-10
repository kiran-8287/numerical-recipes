# Initial Repository Audit: NUMERIX Migration

## 1. Repository inspection summary

The repository currently implements a compact numerical library in C11 with the following structure:

- Public header: [include/numerical_recipes.h](../../include/numerical_recipes.h)
- Library source: [src/root_finding.c](../../src/root_finding.c), [src/differentiation.c](../../src/differentiation.c), [src/interpolation.c](../../src/interpolation.c)
- Build system: [Makefile](../../Makefile)
- Demonstrations: [examples](../../examples)
- Tests: [tests](../../tests)
- Documentation: [documentation](../../documentation)

This project is not yet the NUMERIX architecture described in the master prompt. It is a smaller, working numerical library for root finding, finite-difference differentiation, and interpolation.

## 2. Current implementation footprint

### Algorithms already implemented

The current codebase contains:

- Root finding
  - `bisection`
  - `newton_raphson`
  - `secant`
- Differentiation
  - `forward_difference`
  - `backward_difference`
  - `central_difference`
- Interpolation
  - `linear_interpolation`
  - `lagrange_interpolation`
  - `newton_interpolation`

All implementations are implemented in separate source files, but they are not yet arranged into the NUMERIX modular layout described in the master prompt.

### Current API characteristics

The public API uses a single header, [include/numerical_recipes.h](../../include/numerical_recipes.h), and exposes routines as scalar `double` functions returning `NAN` on detected failures. This is a clean and stable API for a lightweight library but it does not yet support:

- structured results;
- method recommendation;
- diagnostics metadata;
- execution contexts;
- expression parsing;
- piecewise functions;
- parallel task execution;
- benchmarking/reporting layers.

### Current test coverage

The repository includes:

- [tests/test_root_finding.c](../../tests/test_root_finding.c)
- [tests/test_differentiation.c](../../tests/test_differentiation.c)
- [tests/test_interpolation.c](../../tests/test_interpolation.c)
- [tests/test_robustness.c](../../tests/test_robustness.c)
- [tests/test_allocation_failure.c](../../tests/test_allocation_failure.c)

This is a solid foundation for numerical correctness validation. The tests check:

- standard numerical examples;
- invalid inputs;
- non-finite values;
- duplicate or invalid interpolation nodes;
- repeated calls;
- allocation-failure conditions.

### Build and validation workflow

The current build system is a GNU Makefile that compiles the static library and runs separate test binaries. The working flow is:

- `make library`
- `make all`
- `make test`
- `make test-memory`
- `make clean`

This is a good base for cross-platform compilation because it keeps outputs, tests, and demos separate from production library code.

## 3. What can be reused safely

The following current artifacts are worth preserving and adapting:

1. The project structure and separation of library code from demos and tests.
2. The static-library build pattern and separate test executables.
3. The public-header pattern and the concept of a single public API.
4. The root-finding implementations in [src/root_finding.c](../../src/root_finding.c).
5. The differentiations in [src/differentiation.c](../../src/differentiation.c).
6. The interpolation routines in [src/interpolation.c](../../src/interpolation.c).
7. The validation style: input checks, `stderr` diagnostics, and `NAN` failure return values.
8. The test discipline and numerical examples.

These components are a strong base for the NUMERIX foundation and should be retained as working numerical building blocks during the migration.

## 4. What needs modification

The current codebase does not yet meet the NUMERIX specification. The changes required include:

### API and naming migration

- Rename the public interface from `numerical_recipes.h` to the NUMERIX header `numerix.h`.
- Replace scalar-only return conventions with richer `NR_Result` / `NR_Problem` / `NR_Options` style objects.
- Add explicit status codes, diagnostics, and ownership documentation.

### Architectural migration

- Move from a flat library to a modular architecture under `src/core`, `src/algorithms`, `src/execution`, `src/diagnostics`, `src/intelligence`, and `src/expression`.
- Add common function abstraction for callback-based and expression-based inputs.
- Introduce problem representation and execution context.
- Add piecewise function infrastructure and solver coordination.

### Numerical requirements not present yet

The current project is missing major required phases:

- linear algebra routines;
- integration algorithms;
- optimization methods;
- ODE solvers;
- expression parser/evaluator;
- piecewise discontinuity-aware engine;
- method recommendation and fallback logic;
- parallel portfolio execution;
- benchmarking and reliability reporting;
- sensitivity and condition analysis;
- step-by-step execution support;
- advanced diagnostics and convergence monitoring.

### Compatibility and portability issues

- The current naming and header convention is project-specific and not aligned with the NUMERIX master prompt.
- The code uses a simple callback model and returns scalar results, which is insufficient for the richer execution and benchmarking layers.
- The current documentation is strong for the original library but does not cover the NUMERIX architecture, execution contracts, or benchmarking methodology.

## 5. Problems identified and risk factors

1. Naming and API mismatch: existing library name and header do not align with NUMERIX.
2. Missing architecture layers: no core result model, no execution or diagnostics engine.
3. Limited numerical scope: root finding/differentiation/interpolation are implemented, but many required methods are absent.
4. No expression input layer: a major NUMERIX requirement is still missing.
5. No piecewise/discontinuity-aware handling.
6. No method recommendation or fallback logic.
7. No parallel execution model or thread abstraction.
8. No benchmarking or reliability-report framework.
9. Current tests validate original routines but not the larger NUMERIX architecture.

## 6. Code worth preserving

High-value code to retain and adapt:

- validation patterns;
- finite-difference numerical guards;
- interpolation robustness checks;
- `Makefile` structure;
- documentation rationale about modular separation and ownership;
- public API discipline with a single header;
- existing comprehensive numeric test style.

The original implementation is not a replacement target; it is the first stable numerical foundation for the new architecture.

## 7. Proposed migration plan

### Phase 0: architecture and API alignment

1. Define the NUMERIX public API in `include/numerix.h`.
2. Introduce the common function abstraction and internal problem/result types.
3. Document memory ownership and error/status conventions.
4. Establish the new module layout and compile targets.
5. Add compatibility shims or adapters around the current numerical functions.

### Phase 1: preserve and migrate the current library

1. Keep the original algorithms working as a trusted first layer.
2. Re-export or adapt them through the new public API.
3. Add tests to validate that the new API preserves numerical behavior.
4. Avoid deleting existing code until the adapter layer is verified.

### Phase 2: add missing required methods

Implement the remaining algorithms by module:

- linear algebra;
- integration;
- optimization;
- ODE solving;
- expression parsing/evaluation.

### Phase 3: add intelligent layers

- method recommendation;
- fallback logic;
- diagnostics and convergence monitoring;
- step-by-step execution;
- sensitivity/condition analysis.

### Phase 4: portfolio execution and benchmarking

- thread-safe parallel portfolio solver;
- algorithm comparison;
- accuracy/performance analysis;
- GSL/LAPACK/BLAS benchmarking integration.

### Phase 5: CLI and final validation

- CLI usage and demos;
- final documentation and evidence generation;
- cross-platform verification.

## 8. Initial architectural direction

The target architecture should remain consistent with the master prompt and should be designed as a layered, modular library:

```text
CLI / C API
   |
   v
Common function abstraction
   |
   v
Problem + options + result models
   |
   +--> algorithm modules
   +--> execution controller
   +--> diagnostics / convergence / sensitivity
   +--> benchmarking / reporting
```

This structure preserves the original library’s numerical correctness while introducing the richer NUMERIX execution and analysis layers.

## 9. Architectural conflicts or missing information

The master prompt is comprehensive and mostly actionable, but a few items need explicit project decisions before implementation expands:

- Whether the project should immediately rename the header and library names, or maintain a compatibility wrapper during a migration period.
- Whether the current original library should continue to exist as a legacy compatibility layer while NUMERIX grows.
- Whether the phase order should be strictly preserved, or whether the existing working algorithms should be carried forward as part of a parallel migration.
- Whether cross-platform CI/build validation will be performed in Windows and Linux environments via the same Makefile or via CMake in addition to Make.

These items are manageable and do not block the migration, but they should be acknowledged before implementation expands beyond the audit stage.

## 10. Recommendation

Proceed with Phase 0 using the current library as the trusted numerical foundation, while introducing the NUMERIX API and architecture incrementally. The current repository is a suitable starting point, but it must be adapted to the broader NUMERIX design rather than treated as a final state.
