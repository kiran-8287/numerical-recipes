# MASTER PROJECT COMPLETION PROMPT — NUMERIX
## Complete Implementation, Refactoring, Testing, Documentation, Benchmarking, and Repository Cleanup

You are acting as a **senior C systems architect, numerical methods engineer, C11 library developer, software quality engineer, and technical documentation specialist**.

Your task is to take the existing repository and transform it into a fully implemented, reliable, professional numerical computation library named **NUMERIX**.

This is not a request for a superficial refactor, cosmetic cleanup, or a demonstration-only implementation.

You must inspect the existing codebase, understand its architecture, preserve working functionality, implement all missing features, repair incomplete functionality, improve numerical reliability, eliminate obsolete project remnants, verify the implementation, and deliver a professional final repository.

You must work directly in the existing project. Do not create an unrelated replacement project or discard existing functionality.

---

# 1. PROJECT IDENTITY AND OBJECTIVE

The project is named:

**NUMERIX — Intelligent Numerical Computation Library in C11**

NUMERIX is intended to be a modular numerical computation library written in standard C11.

Its purpose is to provide numerical algorithms together with an intelligent execution layer that can:

1. Accept mathematical functions through C callbacks or mathematical expressions.
2. Analyze the numerical problem before selecting an algorithm.
3. Recommend suitable numerical methods.
4. Execute numerical methods with appropriate tolerances and limits.
5. Detect numerical difficulties and potential failures.
6. Switch to alternative methods when appropriate.
7. Handle piecewise functions and discontinuities.
8. Provide detailed numerical diagnostics and execution information.
9. Compare numerical methods and measure their accuracy and performance.
10. Provide a usable command-line interface.
11. Work on both Windows and Linux.
12. Expose a clean, consistent public API through a single public header.

The final result must be a genuine working numerical library, not merely an architectural framework.

## Core technology requirements

- Language: C11.
- Frontend/interface: CLI.
- Public API: one primary public header, `include/numerix.h`.
- Internal implementation: modular `.c` files.
- Build systems: CMake and Make.
- Operating systems: Windows and Linux.
- Mathematical function input:
  - C callback functions.
  - Mathematical expressions supplied as strings.
- Static library output: `libnumerix.a` on Linux and the appropriate static library format on Windows.
- Testing: automated unit, integration, numerical accuracy, robustness, and portability tests.
- Documentation: complete technical documentation and usable examples.

Do not introduce unnecessary external dependencies. Any dependency must be justified, documented, optional where practical, and handled correctly by both build systems.

---

# 2. FIRST ACTION: COMPLETE REPOSITORY AUDIT

Before changing anything, inspect the entire repository.

Do not start rewriting files immediately.

Inspect:

- Every public header.
- Every internal header.
- Every source file.
- Every test.
- Every example.
- `CMakeLists.txt`.
- `Makefile`.
- `.gitignore`.
- `README.md`.
- All documentation.
- Existing benchmarking code.
- Platform-specific code.
- Existing legacy Numerical Recipes implementations.
- Existing build artifacts.
- Existing dependencies and external library integrations.

Create a clear internal understanding of:

1. Which features are fully implemented.
2. Which features are partially implemented.
3. Which features are only placeholders.
4. Which features are missing.
5. Which old files are still required.
6. Which old files are obsolete.
7. Which functions depend on legacy implementations.
8. Which public APIs are inconsistent.
9. Which tests are incomplete or do not test real behavior.
10. Which build configurations are broken.

Create or update:

`documentation/architecture/final_repository_audit.md`

Include a feature-by-feature implementation audit.

Use these statuses:

- COMPLETE
- PARTIALLY IMPLEMENTED
- PLACEHOLDER
- MISSING
- NEEDS REFACTORING
- VERIFIED

Do not mark anything COMPLETE or VERIFIED without actual evidence.

## Important existing repository observations

The repository has previously contained:

- `include/numerix.h`
- `include/numerical_recipes.h`
- Legacy source files:
  - `src/root_finding.c`
  - `src/differentiation.c`
  - `src/interpolation.c`
- NUMERIX wrappers:
  - `src/algorithms/root_finding.c`
  - `src/algorithms/differentiation.c`
  - `src/algorithms/interpolation.c`
- Expression parsing and evaluation.
- Piecewise function handling.
- Method recommendation and fallback.
- Parallel root-finding portfolio.
- Diagnostics.
- Execution limits.
- Step execution.
- Sensitivity analysis.
- Convergence monitoring.
- CLI scaffolding.
- Benchmarking scaffolding.
- Phase-based tests.

These observations describe the previously reviewed repository. Verify the current workspace before relying on them.

**Do not delete legacy algorithm files simply because their names are old.** The NUMERIX wrappers may still depend on them. Trace dependencies before removing, renaming, or consolidating any implementation.

---

# 3. DEVELOPMENT AND CHANGE MANAGEMENT RULES

Follow these rules throughout the entire project.

## 3.1 Protect the existing repository

Before modifying files:

1. Inspect Git status.
2. Identify uncommitted changes.
3. Create a backup or a dedicated Git branch.
4. Preserve the original repository history.
5. Never delete the `.git` directory.
6. Do not overwrite user work without checking it.

If Git is unavailable, create a separate backup copy before major refactoring.

## 3.2 Work incrementally

Implement the project in logical phases.

After each phase:

1. Compile the affected modules.
2. Run relevant tests.
3. Fix regressions.
4. Update documentation.
5. Record the changes.
6. Proceed only when the phase is stable.

Do not implement the entire library as one giant source file.

## 3.3 No fake implementations

Never:

- Add functions that return success without doing the numerical work.
- Return hardcoded numerical answers.
- Replace failed algorithms with arbitrary values.
- Create tests that only check whether a function exists.
- Mark unavailable functionality as implemented.
- Claim that an external benchmark was performed when it was not.
- Hide numerical errors to make tests pass.
- Silently ignore allocation failures.
- Use meaningless placeholder implementations.
- Claim Windows compatibility without an actual Windows build or clearly identified verification limitation.

Every public function must perform its documented task or explicitly return a suitable unsupported/error status.

## 3.4 Preserve compatibility where reasonable

The existing NUMERIX public API should be the primary API.

If existing legacy interfaces must be retained:

- Provide compatibility wrappers when useful.
- Clearly mark deprecated interfaces.
- Document the migration path.
- Avoid maintaining two independent implementations of the same algorithm.

The final public interface should be consistent and easy to use.

---

# 4. TARGET REPOSITORY ARCHITECTURE

Refactor toward the following structure.

Preserve useful existing files and adapt the structure where the current implementation justifies a better arrangement.

```text
NUMERIX/
│
├── CMakeLists.txt
├── Makefile
├── README.md
├── LICENSE
├── .gitignore
│
├── include/
│   └── numerix.h
│
├── src/
│   ├── core/
│   │   ├── problem.c
│   │   ├── result.c
│   │   ├── errors.c
│   │   ├── options.c
│   │   ├── memory.c
│   │   └── execution.c
│   │
│   ├── algorithms/
│   │   ├── root_finding.c
│   │   ├── linear_algebra.c
│   │   ├── differentiation.c
│   │   ├── interpolation.c
│   │   ├── integration.c
│   │   ├── optimization.c
│   │   └── ode.c
│   │
│   ├── expression/
│   │   ├── tokenizer.c
│   │   ├── parser.c
│   │   ├── ast.c
│   │   ├── evaluator.c
│   │   └── expression.c
│   │
│   ├── piecewise/
│   │   ├── intervals.c
│   │   ├── breakpoints.c
│   │   ├── discontinuity.c
│   │   └── piecewise_solver.c
│   │
│   ├── intelligence/
│   │   ├── problem_analysis.c
│   │   ├── method_recommendation.c
│   │   └── method_fallback.c
│   │
│   ├── execution/
│   │   ├── parallel_portfolio.c
│   │   ├── step_execution.c
│   │   └── execution_limits.c
│   │
│   ├── diagnostics/
│   │   ├── error_detection.c
│   │   ├── condition_analysis.c
│   │   ├── sensitivity.c
│   │   └── convergence_monitor.c
│   │
│   ├── benchmarking/
│   │   ├── accuracy_performance.c
│   │   ├── algorithm_comparison.c
│   │   ├── external_comparison.c
│   │   └── reliability_report.c
│   │
│   ├── testing/
│   │   └── validation.c
│   │
│   ├── cli/
│   │   ├── main.c
│   │   ├── cli.c
│   │   └── commands.c
│   │
│   ├── platform/
│   │   ├── platform.h
│   │   ├── timing.c
│   │   └── threading.c
│   │
│   └── internal/
│       └── numerix_internal.h
│
├── examples/
│   ├── root_finding/
│   ├── linear_algebra/
│   ├── differentiation/
│   ├── interpolation/
│   ├── integration/
│   ├── optimization/
│   ├── ode/
│   ├── expression/
│   └── advanced/
│
├── tests/
│   ├── unit/
│   ├── integration/
│   ├── numerical_accuracy/
│   ├── robustness/
│   ├── memory/
│   └── portability/
│
├── benchmarks/
│   ├── internal/
│   ├── external/
│   ├── datasets/
│   └── reports/
│
├── documentation/
│   ├── architecture/
│   ├── algorithms/
│   ├── api/
│   ├── examples/
│   ├── benchmarking/
│   ├── development/
│   └── reports/
│
└── scripts/
    ├── build.sh
    ├── build_windows.bat
    ├── run_tests.sh
    └── generate_report.sh
```

You do not need to create empty directories or meaningless files. Every directory and file must serve a real purpose.

The project must retain a single primary public header:

`include/numerix.h`

Internal headers must remain under `src/`.

---

# 5. PUBLIC API DESIGN

Review and complete `include/numerix.h`.

The header must provide a consistent public API for all implemented modules.

Organize the API into clear sections:

1. Version and configuration.
2. Status codes and errors.
3. Function callbacks.
4. Problem definitions.
5. Solver options.
6. Result structures.
7. Root finding.
8. Linear algebra.
9. Differentiation.
10. Interpolation.
11. Integration.
12. Optimization.
13. ODE solving.
14. Expression parsing and evaluation.
15. Piecewise functions.
16. Intelligent method selection.
17. Diagnostics.
18. Execution control.
19. Benchmarking.
20. Memory management.

## API design requirements

- Use meaningful and consistent function names.
- Use `const` where appropriate.
- Avoid exposing unnecessary internal structures.
- Document ownership of allocated memory.
- Document which functions allocate memory.
- Document how results are released.
- Provide clear status codes.
- Avoid ambiguous return values.
- Use appropriate types.
- Validate pointers and dimensions.
- Handle invalid inputs safely.
- Make the API usable from ordinary C11 programs.
- Use `extern "C"` guards for C++ consumers.
- Avoid global mutable state where possible.

Use a consistent naming convention such as:

```c
numerix_root_solve(...)
numerix_matrix_create(...)
numerix_integrate(...)
numerix_differentiate(...)
numerix_interpolate(...)
numerix_optimize(...)
numerix_ode_solve(...)
numerix_expression_parse(...)
numerix_result_free(...)
```

Do not force these exact signatures if the existing design has a better, well-justified API. The important requirement is consistency.

---

# 6. IMPLEMENT ALL CORE NUMERICAL MODULES

The following modules must be genuinely implemented.

## 6.1 Root finding

Implement and validate:

- Bisection method.
- Regula Falsi.
- Secant method.
- Newton-Raphson.
- Safeguarded Newton method.
- Brent-style bracketed method.
- Appropriate convergence and failure detection.

Requirements:

- Check whether a valid sign-changing bracket exists when required.
- Detect invalid intervals.
- Detect division by zero.
- Detect near-zero derivatives.
- Detect stagnation.
- Support absolute and relative tolerances.
- Support maximum iterations.
- Return iteration counts.
- Return residuals.
- Return convergence status.
- Record useful diagnostics.

Use robust algorithms where possible. Do not blindly continue when numerical conditions are invalid.

## 6.2 Linear algebra

Implement:

### Vector operations
- Addition.
- Subtraction.
- Scalar multiplication.
- Dot product.
- Euclidean norm.
- Infinity norm.
- Vector normalization where valid.

### Matrix operations
- Creation and destruction.
- Addition.
- Subtraction.
- Multiplication.
- Transpose.
- Matrix-vector multiplication.
- Identity matrix.
- Matrix norm.

### Matrix solvers
- Gaussian elimination with partial pivoting.
- Gauss-Jordan elimination.
- LU decomposition with pivoting.
- Forward substitution.
- Back substitution.
- Linear system solving.

### Numerical diagnostics
- Singular matrix detection.
- Near-singular matrix warnings.
- Pivot diagnostics.
- Residual calculation.
- Condition estimation where implemented.
- Appropriate failure statuses.

Do not claim advanced matrix factorization methods such as QR or SVD are available unless they are actually implemented and tested.

## 6.3 Differentiation

Implement:

- Forward finite differences.
- Backward finite differences.
- Central finite differences.
- Higher-order finite difference formulas.
- Richardson extrapolation.
- Appropriate second derivative support.

Requirements:

- Handle small step sizes.
- Detect invalid function evaluations.
- Avoid unnecessary cancellation.
- Provide sensible step-size guidance.
- Report the chosen step size.
- Document expected accuracy and limitations.

## 6.4 Interpolation

Implement:

- Linear interpolation.
- Lagrange polynomial interpolation.
- Newton divided differences.
- Polynomial evaluation.
- Appropriate interpolation derivative support if included in the public API.

Requirements:

- Validate input dimensions.
- Handle duplicate x-values.
- Define behavior outside the data range.
- Avoid unnecessary recomputation where possible.
- Document numerical limitations of high-degree polynomial interpolation.

## 6.5 Numerical integration

Implement:

- Composite trapezoidal rule.
- Composite Simpson's 1/3 rule.
- Adaptive Simpson integration.
- Adaptive quadrature or another justified adaptive method.
- Error estimation.
- Integration diagnostics.

Requirements:

- Handle invalid integration bounds.
- Handle reversed integration limits correctly.
- Validate Simpson's rule requirements.
- Support absolute and relative error tolerances.
- Detect non-finite evaluations.
- Handle integrable singularities only where the selected method supports them.
- Respect breakpoints and discontinuities.
- Report estimated error and evaluation count.

Do not imply that a method can handle arbitrary singularities.

## 6.6 Optimization

Implement:

- Golden-section search.
- Bracketed one-dimensional minimization.
- Gradient descent for suitable multidimensional problems.
- Backtracking or another documented line-search strategy.
- Convergence checks.
- Objective evaluation limits.

Requirements:

- Distinguish minimization from maximization.
- Validate dimensions and initial conditions.
- Detect stagnation.
- Detect non-finite objective values.
- Return final objective value.
- Return iteration and evaluation counts.
- Explain assumptions about differentiability and convexity.

Do not promise global optimality for methods that only find local optima.

## 6.7 Ordinary differential equations

Implement:

- Euler method.
- Improved Euler / Heun method.
- Classical fourth-order Runge-Kutta (RK4).
- A justified adaptive ODE method if included in the API.

Requirements:

- Support initial-value problems.
- Support vector-valued systems where the API allows it.
- Validate initial conditions.
- Handle integration direction.
- Detect non-finite derivatives.
- Respect step limits and execution limits.
- Return the solution trajectory.
- Document step size and accuracy characteristics.

---

# 7. MATHEMATICAL EXPRESSION ENGINE

Complete the expression engine.

It must accept mathematical expressions as strings and evaluate them safely.

Examples:

```text
x^2 - 4
sin(x)
cos(x) + x
exp(-x) - 0.5
sqrt(x) - 3
log(x)
abs(x)
```

Support a clearly documented set of:

- Numeric constants.
- Variables.
- Parentheses.
- Arithmetic operators.
- Exponentiation.
- Unary operators.
- Mathematical functions.
- Appropriate constants such as pi and e.

Implement a proper tokenizer and parser.

Use a clearly defined grammar and operator precedence.

An AST-based design is preferred if it fits the existing architecture.

Requirements:

1. Reject malformed expressions.
2. Detect invalid tokens.
3. Detect missing parentheses.
4. Detect invalid function arguments.
5. Detect division by zero.
6. Detect domain errors.
7. Detect overflow and non-finite results.
8. Provide useful error positions where possible.
9. Avoid unsafe evaluation methods.
10. Avoid executing arbitrary C code or shell commands.

Expression parsing and numerical evaluation must be separate concerns.

Provide APIs to:

- Parse an expression.
- Evaluate an expression at a point.
- Evaluate it over suitable inputs.
- Inspect parse errors.
- Release expression resources.

Make expression ownership and memory management explicit.

---

# 8. C CALLBACK FUNCTION SUPPORT

In addition to expression strings, support C callback functions.

For example:

```c
double my_function(double x, void *user_data);
```

The API should allow users to pass custom context through `user_data`.

Where appropriate, support callbacks for:

- Scalar functions.
- Multivariable functions.
- Objective functions.
- ODE derivative functions.

Provide examples showing both callback-based and expression-based usage.

The same numerical algorithms should be usable through both input methods where practical. Avoid duplicating every algorithm just to support expressions.

---

# 9. PIECEWISE FUNCTIONS AND DISCONTINUITIES

Complete the piecewise numerical engine.

It must support functions that have different definitions across different intervals.

Examples:

```text
f(x) = x^2       for x < 0
f(x) = x + 1     for x >= 0
```

Implement:

- Interval representation.
- Breakpoint storage.
- Interval lookup.
- Piecewise evaluation.
- Breakpoint validation.
- Discontinuity classification where possible.
- Integration across multiple intervals.
- Root-finding interval splitting.
- Appropriate handling of derivative discontinuities.

The engine must not assume that every function is continuous.

When an algorithm requires continuity, differentiability, or a sign-changing bracket, validate the assumption and report when it is violated.

Do not silently cross a known discontinuity during an operation that requires a continuous interval.

---

# 10. INTELLIGENT PROBLEM ANALYSIS AND METHOD RECOMMENDATION

NUMERIX must provide an intelligent numerical execution layer.

This layer should analyze a problem and recommend suitable numerical methods.

For example:

- A bracketed root problem may be suited to bisection or Brent-style methods.
- A smooth function with a reliable derivative may be suited to Newton-Raphson.
- A function without an available derivative may be suited to secant or bracketed methods.
- A function with known discontinuities should be split into suitable intervals.
- A linear system may be suited to LU decomposition with pivoting.
- A smooth bounded one-dimensional optimization problem may be suited to golden-section search.

Implement:

1. Problem classification.
2. Basic function-behavior analysis where possible.
3. Method eligibility checks.
4. Recommendation scoring or a clearly defined rule system.
5. Method selection.
6. Fallback selection.
7. Execution diagnostics.

Important:

This must be a transparent numerical decision system, not a vague claim that the software uses artificial intelligence.

Explain why a method was recommended.

Expose recommendation details through the API or CLI.

Do not claim that finite sampling can prove global smoothness, continuity, monotonicity, or convexity. Distinguish observed evidence from mathematical guarantees.

---

# 11. METHOD FALLBACK AND PARALLEL ROOT-FINDING PORTFOLIO

Complete the fallback engine.

If a selected method fails or is unsuitable:

1. Record the reason.
2. Determine whether another method is eligible.
3. Select a suitable alternative.
4. Execute it within the remaining resource limits.
5. Record the complete execution history.
6. Return the best valid result or a meaningful failure status.

Do not retry indefinitely.

Implement a parallel root-solving portfolio only where the platform and build configuration support it.

Requirements:

- Clearly defined worker behavior.
- Thread-safe independent solver state.
- No unsafe sharing of mutable result structures.
- Proper synchronization.
- Resource limits.
- Cancellation behavior.
- Deterministic result selection rules.
- Correct cleanup after failures.

Do not introduce threading merely for appearance.

If parallel execution is unavailable or disabled, provide a correct sequential fallback.

Document whether the library requires a threading dependency or uses platform-native facilities.

---

# 12. DIAGNOSTICS AND NUMERICAL RELIABILITY

Complete the diagnostic modules.

## Error detection

Detect and report relevant conditions:

- Invalid inputs.
- Invalid intervals.
- Division by zero.
- Non-finite values.
- Overflow and underflow where detectable.
- Near-zero denominators.
- Singular or near-singular matrices.
- Invalid derivatives.
- Stagnation.
- Non-convergence.
- Iteration exhaustion.
- Function evaluation exhaustion.
- Memory allocation failure.
- Cancellation.
- Unsupported method assumptions.

## Condition analysis

Provide suitable condition and stability information where mathematically meaningful.

Do not report an invented condition number.

Document estimation methods and limitations.

## Sensitivity analysis

Implement sensitivity analysis for supported problems.

Examples may include:

- Root sensitivity to input perturbations.
- Solution sensitivity to parameter changes.
- Numerical perturbation analysis.

Distinguish estimated sensitivity from exact mathematical derivatives.

## Convergence monitoring

Track:

- Iteration count.
- Residual.
- Error estimate.
- Step size.
- Relative change.
- Convergence status.
- Stagnation indicators.

## Reliability reporting

Produce meaningful reports containing:

- Selected method.
- Alternative methods considered.
- Execution attempts.
- Convergence details.
- Numerical warnings.
- Estimated error.
- Residual.
- Evaluation counts.
- Execution time.
- Final status.

Fix any unsafe string handling, overlapping buffers, and possible truncation in diagnostic messages.

---

# 13. EXECUTION CONTROL AND RESOURCE LIMITS

Complete execution limits and cancellation functionality.

Support appropriate limits for:

- Maximum iterations.
- Maximum function evaluations.
- Maximum execution time.
- Numerical tolerance.
- Maximum memory usage where it can be reliably enforced.
- Cancellation requests.

Requirements:

- Validate all limit settings.
- Check limits at appropriate points during execution.
- Return a specific status when a limit is reached.
- Preserve useful partial results where appropriate.
- Clean up resources after cancellation or failure.
- Do not claim a hard memory or time guarantee if the implementation cannot enforce it.

Implement step-by-step execution where supported.

A user should be able to inspect execution progress rather than only receiving a final answer.

---

# 14. REAL COMMAND-LINE INTERFACE

Complete the CLI.

The existing CLI must not merely display a menu and print a selected operation.

It must actually execute supported numerical operations.

Provide a clean interface with commands or a menu for:

1. Root finding.
2. Linear algebra.
3. Differentiation.
4. Interpolation.
5. Integration.
6. Optimization.
7. ODE solving.
8. Expression evaluation.
9. Piecewise functions.
10. Method recommendation.
11. Benchmarking.
12. Diagnostics.
13. Exit.

Provide sensible command-line argument parsing.

Example usage:

```bash
numerix root --method bisection --expr "x^2 - 4" --a 0 --b 3
```

```bash
numerix integrate --method simpson --expr "sin(x)" --a 0 --b 3.141592653589793
```

```bash
numerix differentiate --expr "x^3" --x 2
```

```bash
numerix expression --expr "sin(x) + x^2" --x 1
```

```bash
numerix matrix-solve --input matrix.txt
```

The exact syntax may be adjusted to suit the implementation.

Requirements:

- Validate command-line inputs.
- Print clear numerical results.
- Print errors and warnings.
- Print method and convergence information.
- Provide `--help`.
- Provide version information.
- Use nonzero exit codes for failed commands.
- Avoid claiming an operation is supported if it is not implemented.

Where useful, provide an interactive mode and a non-interactive mode.

Do not make the CLI a separate implementation of the numerical algorithms. It must call the public NUMERIX API.

---

# 15. BENCHMARKING AND EXTERNAL LIBRARY COMPARISON

Complete the benchmarking framework.

First implement reliable internal benchmarking.

Compare suitable algorithms using identical mathematical problems and controlled conditions.

Measure:

- Execution time.
- Accuracy.
- Absolute error.
- Relative error.
- Residual.
- Iteration count.
- Function evaluation count.
- Convergence success rate.
- Failure rate.
- Memory use where reliably measurable.

Use multiple test functions and problem sizes.

Separate:

1. Accuracy benchmarks.
2. Performance benchmarks.
3. Reliability benchmarks.
4. Algorithm-to-algorithm comparisons.

## External comparison

Prepare optional comparison adapters for:

- GSL.
- BLAS.
- LAPACK.
- Numerical Recipes, subject to legal access and licensing.

Important distinctions:

- GSL provides numerical algorithms that can be compared directly for suitable tasks.
- BLAS and LAPACK provide lower-level linear algebra functionality; compare appropriate operations rather than unrelated numerical algorithms.
- Numerical Recipes code must not be copied or redistributed without the required rights.
- Do not assume a reference implementation is installed.

External comparisons must:

1. Detect whether the dependency is available.
2. Record its version.
3. Record compiler and build configuration.
4. Record benchmark conditions.
5. Use equivalent problem definitions.
6. Clearly identify unavailable comparisons.
7. Avoid comparing different accuracy targets without explanation.

If a dependency is unavailable, skip the relevant comparison and report it honestly.

Never fill external benchmark tables with fabricated numbers.

The benchmark report must distinguish:

- NUMERIX measurements actually obtained.
- External measurements actually obtained.
- Comparisons not yet performed.

Generate reproducible benchmark reports.

---

# 16. MEMORY MANAGEMENT AND ERROR HANDLING

Audit every allocation and deallocation.

Requirements:

- Check allocation results.
- Prevent memory leaks.
- Prevent double frees.
- Prevent use-after-free.
- Prevent buffer overflows.
- Validate array sizes and multiplication overflow before allocation.
- Define clear ownership rules.
- Release partially allocated structures on failure.
- Provide consistent cleanup functions.
- Ensure all error paths clean up correctly.

Use a consistent result and error model.

Do not expose internal allocation details unnecessarily.

Where practical, add tests for allocation failures.

Use sanitizers when supported.

---

# 17. PORTABILITY: WINDOWS AND LINUX

The project must build on both Windows and Linux.

Inspect all platform-dependent code.

Requirements:

- Keep platform-specific functionality isolated.
- Avoid assuming POSIX APIs exist on Windows.
- Avoid assuming Windows APIs exist on Linux.
- Use portable C11 functionality where possible.
- Use appropriate conditional compilation only where necessary.
- Handle timing functions correctly.
- Handle threading consistently.
- Handle file paths and command-line input portably.
- Avoid compiler-specific extensions unless guarded and documented.

For Linux, verify with GCC and, if available, Clang.

For Windows, verify using an available Windows compiler/toolchain such as MSVC or MinGW.

Fix the Linux CMake timing issue associated with `clock_gettime` and `CLOCK_MONOTONIC` using a correct portable implementation and appropriate feature-test configuration where necessary.

Do not simply disable timing functionality to make the build pass.

---

# 18. BUILD SYSTEMS

Both CMake and Make must build the same library functionality.

## CMake

Provide:

- Correct C11 configuration.
- Static library target named `numerix`.
- Correct public include directories.
- Private internal include directories.
- Platform-specific source selection.
- Optional CLI executable.
- Optional examples.
- Optional benchmarks.
- Optional tests.
- Optional external library integrations.
- Installation rules.
- Exported headers and library.
- CTest integration.

The CMake project should support commands such as:

```bash
cmake -S . -B build
cmake --build build --config Release
ctest --test-dir build --output-on-failure
```

Ensure tests are actually registered with `add_test()`.

## Make

Provide equivalent functionality:

```bash
make
make test
make clean
make examples
make benchmarks
```

Add additional targets if useful.

Ensure Make does not accidentally omit newly implemented source files.

The static library should be named consistently:

- Linux: `libnumerix.a`
- Windows: the appropriate static library filename.

Do not retain `libnumerical_recipes.a` as the primary output name.

---

# 19. COMPILER WARNINGS AND CODE QUALITY

Compile with strict warning settings.

At minimum, use appropriate equivalents of:

```bash
-std=c11
-Wall
-Wextra
-Wpedantic
```

Use stronger warnings where practical.

Fix all genuine warnings.

Pay particular attention to:

- `strncpy` truncation warnings.
- `snprintf` truncation warnings.
- Overlapping source and destination buffers.
- Unsafe string concatenation.
- Signed/unsigned conversion.
- Integer overflow.
- Uninitialized variables.
- Unused variables.
- Incorrect format specifiers.
- Pointer conversion issues.
- Incorrect array indexing.
- Undefined behavior.

Do not suppress warnings globally to achieve a clean build.

If a warning must be suppressed, explain exactly why and limit the suppression to the smallest relevant scope.

Use readable, maintainable C code.

Avoid unnecessarily complicated abstractions.

---

# 20. TESTING: REAL NUMERICAL VALIDATION

Create a complete automated testing system.

The existing phase tests must be reviewed. Do not assume that a successful smoke test proves a feature works.

Organize tests into:

## Unit tests

Test individual functions and modules.

## Integration tests

Test workflows that combine multiple modules.

Examples:

- Expression parser → root solver.
- Expression parser → numerical integration.
- Piecewise engine → root solver.
- Method recommendation → execution → fallback.
- Matrix creation → LU decomposition → solution validation.

## Numerical accuracy tests

Use functions with known analytical solutions.

Examples:

- \(x^2 - 4 = 0\).
- \(\sin(x)\).
- \(e^x\).
- \(x^3\).
- Polynomial interpolation.
- Integrals with known exact results.
- Linear systems with known solutions.
- ODEs with known analytical solutions.

Use appropriate tolerances based on the algorithm.

## Edge cases

Test:

- Zero.
- Negative values.
- Very small values.
- Large values.
- Reversed bounds.
- Invalid bounds.
- Singular matrices.
- Near-singular matrices.
- Duplicate interpolation points.
- Non-finite function outputs.
- Discontinuities.
- Domain errors.
- Allocation failures.
- Maximum iteration exhaustion.
- Cancellation.
- Invalid expressions.
- Empty inputs.
- Invalid dimensions.

## Robustness tests

Test invalid pointers, invalid dimensions, extreme input sizes, and error cleanup paths where appropriate.

Never deliberately dereference invalid memory as a normal test.

## Test coverage

Make sure every public API function has at least one meaningful test.

For important numerical algorithms, include multiple test cases and independent result validation.

The standard test command must execute all relevant test suites, not merely compile them.

Report the number of tests and checks executed.

---

# 21. DOCUMENTATION

Rewrite and complete all documentation.

The README must describe NUMERIX, not the old Numerical Recipes project.

## README.md

Include:

1. Project overview.
2. Key features.
3. Implemented numerical methods.
4. Supported input methods.
5. Architecture overview.
6. Requirements.
7. Linux installation.
8. Windows installation.
9. Build instructions.
10. Test instructions.
11. CLI usage.
12. C API examples.
13. Expression examples.
14. Error handling.
15. Memory management.
16. Benchmarking.
17. Limitations.
18. Current feature status.
19. Roadmap.
20. License information.

Do not claim incomplete features are available.

## API reference

Document every public API function.

For each function include:

- Purpose.
- Parameters.
- Return value.
- Status codes.
- Memory ownership.
- Preconditions.
- Error conditions.
- Numerical limitations.
- Example usage.

## Algorithm documentation

For every implemented numerical algorithm, explain:

- Mathematical principle.
- Formula.
- Preconditions.
- Convergence properties.
- Expected accuracy.
- Computational cost.
- Failure conditions.
- Suitable use cases.
- Limitations.

## Architecture documentation

Document:

- Module responsibilities.
- Public/internal boundaries.
- Data structures.
- Memory ownership.
- Execution flow.
- Method recommendation.
- Fallback behavior.
- Piecewise handling.
- Expression processing.
- Platform abstraction.
- Benchmark architecture.

## Other documentation

Review and update:

- `documentation/algorithms.md`
- `documentation/api_reference.md`
- `documentation/architecture.md`
- `documentation/data_structures.md`
- `documentation/memory_management.md`
- `documentation/testing.md`
- `documentation/references.md`
- `documentation/future_extensions.md`
- `documentation/submission_checklist.md`
- `documentation/team_contributions.md`
- Existing experimental evaluation reports.
- Existing audit and phase-plan documents.

Archive or update obsolete audit documents. Do not leave contradictory documentation in the final repository.

Keep development prompts and internal planning notes out of the public documentation unless they are intentionally required. If retained, move them to a clearly named development directory.

---

# 22. EXAMPLES

Create working examples for every major implemented module.

Examples should compile and run using the public API.

Provide:

- Root-finding example.
- Linear algebra example.
- Differentiation example.
- Interpolation example.
- Integration example.
- Optimization example.
- ODE example.
- Expression evaluation example.
- Piecewise function example.
- Intelligent method recommendation example.
- Diagnostics example.

Each example must:

1. Include `numerix.h`.
2. Use the public API.
3. Check errors.
4. Release allocated resources.
5. Print understandable results.
6. Avoid depending on private internal headers.

---

# 23. REMOVE LEGACY PROJECT REMNANTS SAFELY

The original project was called Numerical Recipes Library.

The final project identity must be NUMERIX.

Review every reference to the old project name.

In particular, inspect:

- `include/numerical_recipes.h`
- `src/root_finding.c`
- `src/differentiation.c`
- `src/interpolation.c`
- Makefile library names.
- README references.
- Example includes.
- Test includes.
- Documentation references.
- Build comments.
- Generated library names.

## Important migration rules

1. Make `include/numerix.h` the only primary public header.
2. Replace old public-header usage throughout examples, tests, and documentation.
3. Rename the main library output to `libnumerix.a`.
4. Preserve legacy implementations if current wrappers still depend on them.
5. If legacy implementations are migrated into NUMERIX modules, ensure there is only one authoritative implementation.
6. Delete obsolete compatibility code only after proving it is unused.
7. If backward compatibility is valuable, retain a clearly marked deprecated compatibility header temporarily.
8. Remove outdated comments and old project naming from active code.

Do not delete a source file merely because it looks old.

Search for all references before deleting or moving it.

---

# 24. CLEAN THE REPOSITORY

Remove generated build artifacts from the final source submission.

Review:

- `bin/`
- `build/`
- `lib/*.a`
- Object files.
- Executables.
- Temporary benchmark outputs.
- Temporary reports.
- Debug logs.
- Unnecessary duplicate files.

Do not delete files that are part of the source or required documentation.

Keep `.git` in the working repository. Do not include Git internals in a source ZIP intended for submission.

Update `.gitignore` appropriately.

Ensure a clean checkout can rebuild everything from source.

The final source archive should contain source files, headers, build scripts, tests, examples, and documentation—not machine-specific generated binaries.

---

# 25. CONTINUOUS INTEGRATION AND AUTOMATED VERIFICATION

If the repository supports GitHub Actions, add CI workflows for:

- Linux GCC build.
- Linux Clang build where available.
- Windows build.
- Unit tests.
- Integration tests.
- Numerical accuracy tests.
- CMake build.
- Make build.

A CI job must fail if compilation or tests fail.

Do not create a workflow that only prints success.

Use a reasonable test matrix.

If Windows CI cannot be configured in the current environment, document the exact commands needed to verify the Windows build.

---

# 26. PERFORMANCE AND NUMERICAL QUALITY REVIEW

After implementation, inspect algorithms for:

- Unnecessary allocations.
- Repeated calculations.
- Inefficient matrix loops.
- Unnecessary function evaluations.
- Poor numerical stability.
- Cancellation errors.
- Overflow risks.
- Underflow risks.
- Incorrect convergence conditions.
- Incorrect error estimates.

Optimize only after correctness is established.

Do not sacrifice numerical reliability for a small performance improvement.

Use reproducible benchmarks to support performance claims.

---

# 27. FINAL IMPLEMENTATION PHASES

Complete the project in the following order.

## Phase 0 — Repository protection and audit

- Create backup/branch.
- Inspect repository.
- Record current status.
- Identify missing functionality.
- Identify dependencies.
- Establish baseline builds and tests.

## Phase 1 — Architecture and public API

- Finalize module boundaries.
- Complete `numerix.h`.
- Establish consistent error handling.
- Establish memory ownership rules.
- Define public data structures.

## Phase 2 — Core numerical algorithms

- Complete root finding.
- Complete linear algebra.
- Complete differentiation.
- Complete interpolation.
- Implement integration.
- Implement optimization.
- Implement ODE solving.

## Phase 3 — Expression and piecewise engines

- Complete parser and evaluator.
- Validate expression grammar.
- Complete piecewise representation.
- Complete breakpoint and discontinuity handling.
- Integrate these capabilities with numerical methods.

## Phase 4 — Intelligent execution

- Complete problem analysis.
- Complete method recommendation.
- Complete method eligibility checks.
- Complete fallback engine.
- Complete parallel portfolio where supported.

## Phase 5 — Diagnostics and reliability

- Complete error detection.
- Complete condition analysis where supported.
- Complete sensitivity analysis.
- Complete convergence monitoring.
- Complete reliability reporting.
- Complete execution limits and cancellation.

## Phase 6 — CLI and examples

- Implement functional CLI operations.
- Implement argument parsing.
- Add help and version commands.
- Add examples for all numerical modules.

## Phase 7 — Benchmarking

- Complete internal benchmarks.
- Add optional external integrations.
- Verify GSL comparison where available.
- Verify appropriate BLAS/LAPACK comparisons.
- Handle Numerical Recipes references legally.
- Generate reproducible reports.

## Phase 8 — Build and portability

- Complete CMake.
- Complete Make.
- Register all tests with CTest.
- Fix Linux build.
- Verify Windows build.
- Fix compiler warnings.
- Verify clean installation.

## Phase 9 — Testing and validation

- Run all unit tests.
- Run all integration tests.
- Run numerical accuracy tests.
- Run memory tests.
- Run robustness tests.
- Run portability tests.
- Run sanitizers where available.

## Phase 10 — Documentation and cleanup

- Rewrite README.
- Update API reference.
- Update algorithm documentation.
- Update architecture documentation.
- Remove obsolete project references.
- Remove generated artifacts from submission.
- Verify `.gitignore`.
- Prepare final source archive.

## Phase 11 — Final acceptance audit

Perform a final audit of every requirement in this prompt.

Do not declare the project complete until the acceptance criteria below have been checked.

---

# 28. FINAL ACCEPTANCE CRITERIA

The project is complete only when all applicable requirements are satisfied.

## A. Implementation

- [ ] All required numerical modules have genuine implementations.
- [ ] Public API is complete and consistent.
- [ ] Expression parsing works.
- [ ] C callbacks work.
- [ ] Piecewise functions work.
- [ ] Discontinuity-aware execution works within documented limitations.
- [ ] Method recommendation works.
- [ ] Fallback works.
- [ ] Parallel execution works where supported, with a valid sequential fallback.
- [ ] Diagnostics work.
- [ ] Execution limits work.
- [ ] CLI performs real numerical operations.
- [ ] Examples work.

## B. Numerical correctness

- [ ] Algorithms are tested against known solutions.
- [ ] Tolerances are meaningful.
- [ ] Residuals are checked.
- [ ] Invalid numerical conditions are handled.
- [ ] Edge cases are covered.
- [ ] Numerical limitations are documented.
- [ ] No fabricated numerical outputs exist.

## C. Build and portability

- [ ] CMake configures.
- [ ] CMake builds.
- [ ] CTest discovers and executes tests.
- [ ] Make builds.
- [ ] Make test executes the complete test suite.
- [ ] Linux build is verified.
- [ ] Windows build is verified or its outstanding verification is explicitly documented.
- [ ] Compiler warnings are fixed or individually justified.
- [ ] Static library is correctly named `numerix`.

## D. Quality and memory

- [ ] Allocation failures are handled.
- [ ] Cleanup paths are tested.
- [ ] No known memory leaks remain.
- [ ] No known undefined behavior remains.
- [ ] No known buffer overflows remain.
- [ ] No unsafe overlapping string operations remain.
- [ ] Sanitizer checks are run where available.

## E. Benchmarking

- [ ] Internal benchmarking works.
- [ ] Accuracy and performance are reported separately.
- [ ] External library detection works.
- [ ] External comparisons are performed only when dependencies are available.
- [ ] Unavailable comparisons are honestly reported.
- [ ] No fabricated benchmark data exists.

## F. Repository and documentation

- [ ] README describes NUMERIX accurately.
- [ ] API documentation is complete.
- [ ] Algorithm documentation is complete.
- [ ] Architecture documentation is accurate.
- [ ] Examples use the public NUMERIX API.
- [ ] Old project references are removed from active documentation.
- [ ] Legacy code is retained only when justified.
- [ ] Generated binaries and build artifacts are excluded from the source submission.
- [ ] A clean checkout can rebuild the project.
- [ ] Final repository structure is clean.

---

# 29. FINAL REPORT REQUIRED FROM YOU

When implementation is finished, produce:

## 1. Project completion summary

Explain what NUMERIX now does.

## 2. Final repository structure

Show the complete final directory tree.

## 3. Implementation report

For every module, state:

- What was implemented.
- Which algorithms are available.
- Which files contain the implementation.
- Which tests verify the module.

## 4. API report

Summarize the final public API and show small usage examples.

## 5. Build report

Provide the actual commands used and their results for:

- CMake.
- Make.
- Linux.
- Windows, if verified.

## 6. Testing report

Report:

- Number of tests.
- Number of checks.
- Passed tests.
- Failed tests.
- Numerical accuracy results.
- Memory validation results.
- Outstanding limitations.

## 7. Benchmark report

State which benchmarks were actually performed.

Identify unavailable external libraries and comparisons that remain unperformed.

## 8. Cleanup report

List:

- Files created.
- Files modified.
- Files renamed.
- Files deleted.
- Legacy code retained.
- Generated files excluded from the final submission.

Explain why each significant deletion or migration was safe.

## 9. Remaining limitations

List anything that is not fully implemented or could not be verified.

Do not hide limitations.

## 10. Final commands

Provide exact commands for a new user to:

- Build the project.
- Run the tests.
- Run the CLI.
- Execute examples.
- Run benchmarks.

---

# 30. MOST IMPORTANT FINAL INSTRUCTION

Do not confuse the completion of the development phases with the completion of the software.

The goal is not to create more source files, more directories, more documentation, or more test names.

The goal is to create a **working numerical computation library** whose implementations, numerical results, APIs, error handling, tests, build system, CLI, and documentation agree with each other.

Prioritize:

1. Mathematical correctness.
2. Real implementations.
3. Numerical reliability.
4. Memory safety.
5. API consistency.
6. Cross-platform compatibility.
7. Meaningful testing.
8. Maintainability.
9. Documentation accuracy.
10. Professional repository cleanliness.

If a requirement is impossible or unsuitable, explain why and propose a technically justified alternative. Do not silently omit it.

Work autonomously through all phases, inspect your own changes, fix problems you introduce, and continue until the acceptance criteria are satisfied or a genuine external limitation prevents further progress.

**The final deliverable must be a complete, clean, buildable, tested, documented, and genuinely functional NUMERIX C11 numerical computation library.**