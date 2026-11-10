# MASTER DEVELOPMENT PROMPT — NUMERIX
## Intelligent Numerical Computation Library in C

You are acting as the lead software architect, senior C developer, numerical methods engineer, testing engineer, and technical documenter for a project called **NUMERIX**.

Your task is to redesign, develop, test, and document NUMERIX as a modular, intelligent numerical computation library written in C.

This is not supposed to be a collection of unrelated numerical programs or a single large C file. It must be a reusable numerical library with a clean public API, independent algorithm implementations, a CLI demonstration interface, reliable numerical computation, intelligent method selection, error analysis, and comparative benchmarking against established numerical libraries.

Follow the requirements and phases below carefully.

---

# 1. PROJECT OBJECTIVE

Develop NUMERIX as an intelligent numerical computation library that provides traditional numerical methods along with additional capabilities that improve reliability, usability, accuracy analysis, and computational efficiency.

The library must support:

- Multiple numerical algorithms.
- Mathematical expression input.
- User-defined C callback functions.
- Piecewise functions and discontinuities.
- Intelligent numerical method recommendations.
- Automatic algorithm fallback.
- Parallel algorithm execution and comparison.
- Numerical error detection and explanation.
- Accuracy and performance comparisons.
- Sensitivity and condition analysis.
- Step-by-step execution.
- Numerical reliability reports.
- Computation limits and cancellation.
- Comprehensive testing and validation.

The project must work on both:

1. Windows
2. Linux

Use portable C11 as the primary implementation standard.

The primary demonstration interface must be a command-line interface (CLI).

Do not develop a GUI unless explicitly requested later.

---

# 2. FIRST TASK: INSPECT THE EXISTING PROJECT

Before writing or modifying code, inspect the complete existing repository.

The original project is a Numerical Recipes Library in C. It already contains some implementations that should be preserved and improved rather than discarded.

Inspect:

- All existing source files.
- Existing public headers.
- Existing numerical algorithms.
- Existing Makefile.
- Existing examples.
- Existing tests.
- Existing documentation.
- Existing memory management.
- Existing API design.
- Existing error handling.
- Existing compilation process.

The original assignment requires the following numerical methods:

### Root Finding
- Bisection Method.
- Newton-Raphson Method.
- Secant Method.

### Linear Algebra
- Gaussian Elimination.
- Gauss-Jordan Elimination.
- LU Decomposition.
- Matrix and vector operations.

### Interpolation
- Linear Interpolation.
- Lagrange Interpolation.
- Newton Interpolation.

### Integration
- Trapezoidal Rule.
- Simpson's Rule.

### Differentiation
- Numerical differentiation using finite differences.

### Optimization
- Gradient Descent.
- Golden-Section Search.

### Ordinary Differential Equations
- Euler Method.
- Fourth-order Runge-Kutta Method (RK4).

Check which algorithms are already implemented and which are missing.

Do not blindly recreate existing implementations.

Instead:

1. Identify reusable code.
2. Identify incomplete or incorrect implementations.
3. Identify duplicated code.
4. Identify weaknesses in the current API.
5. Identify memory-management issues.
6. Identify portability issues.
7. Identify missing numerical validation.
8. Prepare a migration plan.

Preserve working implementations where possible and improve them to fit the new architecture.

Do not delete existing code until its functionality has been understood and its replacement has been tested.

Create an initial repository audit document:

`documentation/architecture/initial_audit.md`

Include:

- Existing project structure.
- Existing algorithms.
- Existing test coverage.
- Existing build process.
- Missing requirements.
- Problems identified.
- Code worth preserving.
- Proposed migration steps.

Do not start implementing advanced features before understanding the existing code.

---

# 3. CORE TECHNOLOGY AND DESIGN DECISIONS

Follow these decisions throughout the project.

| Area | Decision |
|---|---|
| Programming language | C |
| C standard | C11 |
| Operating systems | Windows and Linux |
| User interface | CLI |
| Build systems | Makefile and CMake |
| Function input | C callbacks and mathematical expressions |
| Public API | One main public header |
| Implementation | Multiple modular `.c` files |
| Testing | Unit, integration, numerical and concurrency tests |
| Final comparison | GSL, BLAS, LAPACK and Numerical Recipes where appropriate |

## Important header-file decision

Use **one public header file:**

`include/numerix.h`

Do not create separate public headers for every module.

The single public header should expose:

- Public structures.
- Enumerations.
- Status codes.
- Configuration options.
- Function representations.
- Numerical method declarations.
- Piecewise function declarations.
- Expression input declarations.
- Recommendation APIs.
- Parallel execution APIs.
- Diagnostics APIs.
- Benchmarking APIs.
- Execution controls.
- Memory management functions.

Use include guards and proper C/C++ compatibility through `extern "C"`.

Internal implementation-only declarations may be placed in private headers under `src/internal/` when genuinely necessary. Do not expose implementation details unnecessarily.

**One public header does not mean one source file.** Keep all algorithm implementations modular and separate.

---

# 4. TARGET PROJECT STRUCTURE

Build toward the following repository structure.

Create folders and files incrementally when implementing each phase. Do not create hundreds of empty placeholder files.

```text
NUMERIX/
│
├── include/
│   └── numerix.h
│
├── src/
│   │
│   ├── internal/
│   │   ├── numerix_internal.h
│   │   ├── function_internal.h
│   │   ├── execution_internal.h
│   │   └── validation_internal.h
│   │
│   ├── core/
│   │   ├── problem.c
│   │   ├── result.c
│   │   ├── options.c
│   │   ├── errors.c
│   │   └── memory.c
│   │
│   ├── algorithms/
│   │   │
│   │   ├── root_finding/
│   │   │   ├── bisection.c
│   │   │   ├── newton_raphson.c
│   │   │   └── secant.c
│   │   │
│   │   ├── linear_algebra/
│   │   │   ├── gaussian_elimination.c
│   │   │   ├── gauss_jordan.c
│   │   │   ├── lu_decomposition.c
│   │   │   └── matrix_operations.c
│   │   │
│   │   ├── interpolation/
│   │   │   ├── linear.c
│   │   │   ├── lagrange.c
│   │   │   └── newton.c
│   │   │
│   │   ├── integration/
│   │   │   ├── trapezoidal.c
│   │   │   └── simpson.c
│   │   │
│   │   ├── differentiation/
│   │   │   └── finite_differences.c
│   │   │
│   │   ├── optimization/
│   │   │   ├── gradient_descent.c
│   │   │   └── golden_section.c
│   │   │
│   │   └── ode/
│   │       ├── euler.c
│   │       └── runge_kutta.c
│   │
│   ├── expression/
│   │   ├── tokenizer.c
│   │   ├── parser.c
│   │   ├── ast.c
│   │   └── evaluator.c
│   │
│   ├── piecewise/
│   │   ├── intervals.c
│   │   ├── breakpoints.c
│   │   ├── discontinuity.c
│   │   └── piecewise_solver.c
│   │
│   ├── intelligence/
│   │   ├── method_recommendation.c
│   │   ├── method_fallback.c
│   │   └── problem_analysis.c
│   │
│   ├── execution/
│   │   ├── parallel_portfolio.c
│   │   ├── cancellation.c
│   │   ├── step_execution.c
│   │   └── execution_limits.c
│   │
│   ├── diagnostics/
│   │   ├── error_detection.c
│   │   ├── convergence_monitor.c
│   │   ├── sensitivity.c
│   │   └── condition_analysis.c
│   │
│   └── benchmarking/
│       ├── algorithm_comparison.c
│       ├── accuracy_performance.c
│       └── reliability_report.c
│
├── platform/
│   ├── windows/
│   │   ├── threading.c
│   │   └── timing.c
│   │
│   └── linux/
│       ├── threading.c
│       └── timing.c
│
├── cli/
│   ├── main.c
│   ├── commands.c
│   ├── input.c
│   └── output.c
│
├── examples/
│   ├── root_finding/
│   ├── linear_algebra/
│   ├── interpolation/
│   ├── integration/
│   ├── differentiation/
│   ├── optimization/
│   └── ode/
│
├── tests/
│   ├── unit/
│   ├── integration/
│   ├── edge_cases/
│   ├── numerical_stability/
│   ├── concurrency/
│   ├── error_handling/
│   └── validation/
│
├── benchmarks/
│   ├── standard_problems/
│   ├── accuracy/
│   ├── performance/
│   ├── numerical_recipes/
│   ├── gsl/
│   ├── lapack/
│   └── blas/
│
├── documentation/
│   ├── architecture/
│   ├── algorithms/
│   ├── api/
│   ├── user_guide/
│   ├── testing/
│   └── experimental_evaluation/
│
├── build/
│
├── Makefile
├── CMakeLists.txt
├── README.md
├── LICENSE
└── .gitignore
```

The exact number of implementation files can change if a better modular arrangement is justified. However, preserve the responsibilities and separation described here.

Do not create empty files just to match the tree.

---

# 5. ARCHITECTURE: HOW THE MODULES CONNECT

This is one of the most important requirements.

The library must have a coherent flow from user input to numerical execution and final result.

The overall architecture should be:

```text
                 USER
                  |
          +-------+-------+
          |               |
       CLI Input       C API Input
          |               |
          v               v
    Expression Parser   C Callback
          |               |
          +-------+-------+
                  |
                  v
          Common Function Model
                  |
                  v
           Problem Representation
                  |
          +-------+--------+
          |                |
          v                v
    Problem Analysis   Piecewise Engine
          |                |
          +-------+--------+
                  |
                  v
       Method Recommendation
                  |
                  v
          Execution Controller
                  |
          +-------+--------+
          |                |
          v                v
    Numerical Method   Parallel Portfolio
          |                |
          +-------+--------+
                  |
                  v
       Independent Verification
                  |
                  v
          Diagnostics Engine
                  |
                  v
           Structured Result
                  |
          +-------+--------+
          |                |
          v                v
     CLI Report       Benchmark Data
```

## 5.1 Common function representation

Both supported function-input methods must eventually produce a common internal function representation.

### Route A: Mathematical expressions

Example user input:

`sin(x) + x^2 - 4`

Processing:

1. Tokenize the expression.
2. Parse tokens.
3. Construct an Abstract Syntax Tree (AST).
4. Validate syntax and mathematical operations.
5. Evaluate the expression for a supplied value of x.
6. Expose it through the common function interface.

### Route B: C callback functions

Example:

```c
double my_function(double x, void *user_data)
{
    return x * x - 4.0;
}
```

The callback should be wrapped in the same common function representation.

Do not force callback users to convert their functions into strings.

### Common function model

Design a function abstraction capable of storing:

- Evaluation callback.
- Optional derivative callback.
- User data pointer.
- Function input type.
- Domain information, if known.
- Continuity information, if supplied or established.
- Differentiability information, if supplied or established.
- Ownership information for dynamically allocated resources.

The internal evaluator should provide a consistent way for algorithms to evaluate both expression-based and callback-based functions.

Never claim that arbitrary C callbacks can be automatically analyzed for continuity or differentiability. Such properties must come from user-supplied metadata, explicit domain information, or cautious numerical heuristics.

---

# 6. PHASE 0 — ARCHITECTURE AND CORE API

This phase must be completed before expanding numerical algorithms.

## Objectives

Design the common architecture and public API.

Implement:

- Core data structures.
- Function representation.
- Problem representation.
- Structured result representation.
- Numerical options.
- Status and error codes.
- Memory management conventions.
- Timing abstraction.
- Execution context.
- Common validation utilities.

### Public API design

Keep the public API in:

`include/numerix.h`

The API should be designed around reusable types such as:

- `NR_Function`
- `NR_Problem`
- `NR_Result`
- `NR_Options`
- `NR_Status`
- `NR_Method`
- `NR_Error`
- `NR_ExecutionContext`
- `NR_PiecewiseFunction`

These are suggested names. You may use the NUMERIX prefix consistently instead of NR if preferred. Do not mix naming conventions.

### Structured result

Avoid returning only a `double` from every algorithm.

Design a common result structure that can report:

- Computed value.
- Residual, where applicable.
- Estimated error, where available.
- Iteration count.
- Function evaluation count.
- Execution time.
- Convergence status.
- Method used.
- Warnings.
- Diagnostic information.

For methods where a particular field is not meaningful, explicitly mark it unavailable rather than inventing a value.

### Options

Provide user-configurable settings:

- Absolute tolerance.
- Relative tolerance.
- Maximum iterations.
- Maximum execution time.
- Maximum memory or allocation limits where practical.
- Derivative step size.
- Convergence criteria.
- Cancellation token.
- Diagnostic level.
- Step-by-step execution mode.

### Memory management

Document:

- Who allocates each object.
- Who owns each object.
- Who frees each object.
- Whether returned objects must be released by the caller.
- How allocation failures are reported.

Provide consistent creation and destruction functions.

### Deliverables

Create:

- Initial public API.
- Core implementation.
- Internal interfaces.
- API documentation.
- Small examples.
- Core unit tests.

### Phase 0 completion criteria

Do not proceed until:

- The project compiles on Linux.
- The project compiles on Windows.
- The common function abstraction works.
- The common result structure works.
- Memory ownership is documented.
- Basic tests pass.
- Existing code migration strategy is documented.

---

# 7. PHASE 1 — COMPLETE THE ORIGINAL NUMERICAL LIBRARY

Implement all required numerical methods.

Keep every algorithm in its own implementation file.

Do not put every algorithm in one large source file.

## 7.1 Root finding

Implement:

- Bisection.
- Newton-Raphson.
- Secant.

Each method should support:

- Absolute and relative tolerances.
- Maximum iteration limit.
- Nonfinite value detection.
- Residual calculation.
- Convergence detection.
- Nonconvergence reporting.
- Function evaluation counting.

Newton-Raphson should support an optional user-supplied derivative.

If a derivative is unavailable, the method may use a supported numerical derivative strategy, provided it is configured and reported.

## 7.2 Linear algebra

Implement:

- Gaussian elimination.
- Gauss-Jordan elimination.
- LU decomposition.
- Matrix multiplication.
- Matrix addition and subtraction.
- Vector operations.
- Matrix and vector memory management.

Use pivoting where appropriate.

Detect:

- Singular matrices.
- Nearly singular matrices.
- Invalid dimensions.
- Nonfinite inputs.
- Ill-conditioning warnings.

Design matrix structures with clear dimensions, storage and ownership rules.

## 7.3 Interpolation

Implement:

- Linear interpolation.
- Lagrange interpolation.
- Newton divided differences.

Validate:

- Duplicate x-values.
- Invalid input sizes.
- Nonfinite data.
- Extrapolation outside the data interval.

Provide clear warnings for potentially unreliable extrapolation.

## 7.4 Integration

Implement:

- Trapezoidal Rule.
- Simpson's Rule.

Support:

- Configurable subdivisions.
- Tolerance-based refinement where appropriate.
- Evaluation counting.
- Nonfinite value detection.
- Error estimates only when mathematically justified.

Handle Simpson's Rule subdivision requirements correctly.

## 7.5 Differentiation

Implement finite differences:

- Forward difference.
- Backward difference.
- Central difference.

Include appropriate step-size validation and document numerical limitations.

## 7.6 Optimization

Implement:

- Gradient Descent.
- Golden-Section Search.

Support convergence criteria, iteration limits, and reporting.

Clearly distinguish methods requiring derivatives from derivative-free methods.

## 7.7 ODE solving

Implement:

- Euler Method.
- Fourth-order Runge-Kutta.

Support:

- Initial conditions.
- Step sizes.
- Integration direction where supported.
- Maximum steps.
- Nonfinite detection.
- Execution reporting.

### Phase 1 deliverables

- All required algorithms.
- Examples for every module.
- Unit tests for every algorithm.
- API documentation.
- Numerical correctness tests against analytical solutions.

Do not add advanced intelligence until the basic numerical foundation is stable.

---

# 8. PHASE 2 — MATHEMATICAL EXPRESSION ENGINE

Implement the expression parser so users can provide mathematical functions directly through the CLI and API.

Files:

`src/expression/`

Implement:

1. Tokenizer.
2. Parser.
3. AST construction.
4. AST evaluator.

Support at least:

- Addition.
- Subtraction.
- Multiplication.
- Division.
- Exponentiation.
- Parentheses.
- Unary minus.
- Variables such as x.
- Constants such as pi and e.
- sin.
- cos.
- tan.
- asin.
- acos.
- atan.
- exp.
- log.
- log10.
- sqrt.
- abs.

Add domain checking and meaningful syntax errors.

Examples:

`x^2 - 4`

`sin(x) + cos(x)`

`exp(-x^2)`

`log(x)`

The expression engine must integrate with the common function representation.

The algorithms must not need to know whether a function originated from an expression or a C callback.

### Future-proofing

Keep the expression architecture suitable for later automatic differentiation and symbolic derivative support.

Do not attempt to implement a full computer algebra system in this phase.

### Tests

Test:

- Operator precedence.
- Parentheses.
- Nested functions.
- Invalid expressions.
- Division by zero.
- Invalid logarithm inputs.
- Invalid square root inputs.
- Nonfinite results.

---

# 9. PHASE 3 — PIECEWISE AND DISCONTINUITY-AWARE ENGINE

This is a major NUMERIX feature.

Implement it in:

`src/piecewise/`

Files:

- `intervals.c`
- `breakpoints.c`
- `discontinuity.c`
- `piecewise_solver.c`

## 9.1 Purpose

Allow users to define functions over multiple intervals, with different mathematical expressions or callbacks in different regions.

The piecewise engine must connect directly to the common function representation.

It must not be an isolated module containing unrelated hardcoded functions.

## 9.2 Example

The user enters:

```text
f(x) =

x^2       for 0 <= x < 2
3*x + 1   for 2 <= x <= 4
```

Internally:

- Segment 1 stores the function x² and its interval.
- Segment 2 stores the function 3x + 1 and its interval.
- The breakpoint is x = 2.

Each segment must reference or own a valid common function representation according to documented ownership rules.

## 9.3 Interval management

Support:

- Lower and upper bounds.
- Open and closed endpoints.
- Adjacent intervals.
- Disjoint intervals.
- Gaps.
- Overlapping intervals.
- Boundary ownership.
- Invalid interval detection.

Define clear policies for gaps and overlaps.

Do not silently choose a function when two intervals overlap ambiguously.

## 9.4 Breakpoint management

Identify and store known breakpoints where the user-defined function changes.

Support explicitly supplied breakpoints.

Do not claim arbitrary black-box discontinuities can always be detected automatically.

Heuristic detection may be added later and must be labeled as heuristic.

## 9.5 Discontinuity handling

Support classification where possible:

- Jump discontinuity.
- Removable discontinuity.
- Infinite discontinuity.
- Undefined point.

Do not claim mathematically certain classification based only on a few numerical samples.

Distinguish:

- Known discontinuities from user-provided piecewise definitions.
- Numerically suspected discontinuities.
- Unknown behaviour.

## 9.6 Integration example

For:

```text
x^2       for 0 <= x < 2
3*x + 1   for 2 <= x <= 4
```

And:

`integrate(f, 0, 4)`

The piecewise engine must:

1. Receive the piecewise function.
2. Identify the relevant intervals.
3. Find the breakpoint x = 2.
4. Split the integration range.
5. Call the existing integration algorithm for each valid segment.
6. Combine the segment results.
7. Report the result and segment-level diagnostics.

Mathematically:

`integral(0,4) = integral(0,2) + integral(2,4)`

The piecewise engine must NOT reimplement Simpson's Rule or Trapezoidal Rule.

It must coordinate existing algorithms.

## 9.7 Reuse across numerical methods

The same piecewise architecture must support:

- Root finding within valid intervals.
- Integration split at breakpoints.
- Differentiation with appropriate one-sided handling near boundaries.
- Interpolation over separate data regions where appropriate.
- ODE solving when the governing function changes between intervals.
- State transitions at known boundaries.

For ODE transitions, define how the state is carried from one interval to the next and whether integration restarts.

Do not evaluate across invalid gaps.

### Phase 3 tests

Test:

- Continuous adjacent segments.
- Jump discontinuities.
- Removable discontinuities.
- Gaps.
- Overlapping intervals.
- Exact breakpoint evaluation.
- Integration across breakpoints.
- Root finding on multiple segments.
- Differentiation near boundaries.
- ODE state transitions.

---

# 10. PHASE 4 — INTELLIGENT METHOD RECOMMENDATION

Implement:

`src/intelligence/`

Files:

- `method_recommendation.c`
- `problem_analysis.c`
- `method_fallback.c`

## 10.1 Purpose

NUMERIX should recommend suitable numerical methods instead of requiring users to understand every algorithm.

Analyze available problem information, including:

- Continuity.
- Differentiability.
- Derivative availability.
- Function domain.
- Discontinuities.
- Matrix dimensions.
- Matrix sparsity.
- Matrix properties where established.
- Conditioning.
- ODE stiffness information where available.
- Constraints.
- User tolerances.
- Computational limits.

## 10.2 Recommendations

Examples:

- Recommend Bisection when a valid sign-changing bracket is available and reliability is prioritized.
- Recommend Newton-Raphson when derivative information and suitable starting conditions are available.
- Recommend Secant when a derivative is unavailable but two starting points are available.
- Recommend LU decomposition for suitable linear systems.
- Recommend Gaussian elimination with pivoting when appropriate.
- Recommend Golden-Section Search for suitable derivative-free one-dimensional optimization.
- Recommend RK4 for suitable non-stiff initial-value ODE problems.

Do not recommend methods based on unsupported assumptions.

## 10.3 Explainable decisions

Every recommendation should include:

- Recommended method.
- Reason for recommendation.
- Required assumptions.
- Advantages.
- Limitations.
- Potential risks.
- Alternative methods.

Example:

```text
Recommended method: Bisection

Reason:
A valid sign-changing interval was found.

Advantages:
Reliable bracket-preserving convergence under continuity.

Limitations:
Usually slower than Newton-Raphson near a well-behaved root.

Alternative:
Secant Method.
```

If properties cannot be determined, clearly state that.

---

# 11. PHASE 5 — AUTOMATIC ALGORITHM FALLBACK

Implement automatic recovery when a numerical method fails.

Example:

1. User requests Newton-Raphson.
2. The derivative is zero or unusable.
3. The algorithm detects the failure.
4. The diagnostics module reports the reason.
5. The fallback module checks available alternatives.
6. If a valid bracket exists, it may select Bisection.
7. Otherwise, it may consider Secant if valid initial points are available.
8. The alternative method is executed.
9. The result records the method-switching history.

The system must:

- Preserve the original failure information.
- Record every fallback attempt.
- Avoid infinite fallback loops.
- Respect user limits and cancellation.
- Verify the final result independently.
- Report if all available methods fail.

Never silently switch algorithms.

---

# 12. PHASE 6 — PARALLEL ALGORITHM RACING / PORTFOLIO SOLVER

Implement:

`src/execution/parallel_portfolio.c`

Initially focus on root finding.

## 12.1 Objective

Run multiple numerical methods concurrently on the same problem.

For example:

- Thread 1: Bisection.
- Thread 2: Newton-Raphson.
- Thread 3: Secant.

Each method receives its own execution state and configuration.

## 12.2 Execution flow

1. Validate the problem.
2. Prepare independent solver contexts.
3. Start the methods concurrently.
4. Monitor completion.
5. Independently verify each candidate result.
6. Accept the first sufficiently verified result.
7. Request cooperative cancellation of remaining methods.
8. Join all worker threads safely.
9. Collect timing and convergence information.
10. Report the winner and the comparison.

Do not accept a result merely because a thread finished first.

A candidate must satisfy the configured correctness and residual checks.

## 12.3 Thread safety

Use a platform abstraction.

Linux:
- pthread-based implementation.

Windows:
- Windows threading implementation or a suitable portable threading backend.

Do not scatter platform-specific calls throughout numerical algorithms.

Use cooperative cancellation.

Never forcibly terminate a thread while it is using library memory.

Protect shared state using appropriate synchronization.

Do not allow one worker to corrupt another worker's function or execution context.

## 12.4 Performance comparison

Measure:

- Parallel execution time.
- Individual execution times.
- Total function evaluations.
- Iterations.
- Accuracy.
- Thread startup overhead.

Demonstrate that parallel execution is not automatically faster for every problem.

---

# 13. PHASE 7 — ADVANCED ERROR DETECTION AND EXPLANATION

Implement:

`src/diagnostics/error_detection.c`

Detect and report:

- Invalid input.
- Invalid interval.
- Division by zero.
- Domain violations.
- Singular matrix.
- Nearly singular matrix.
- Overflow.
- Underflow where detectable.
- NaN and infinity.
- Divergence.
- Nonconvergence.
- Stagnation.
- Oscillation.
- Ill-conditioning.
- Numerical instability.
- Resource limit reached.
- Cancellation.

The error system must explain:

1. What happened?
2. Where it happened?
3. Why it may have happened?
4. What the user can do to correct it?
5. Whether an alternative method is available?

Example:

```text
Status: DERIVATIVE_ZERO

Method: Newton-Raphson

Problem:
The derivative evaluated to approximately zero.

Possible reason:
The current iteration is at or near a stationary point.

Suggested action:
Try a different initial guess or use a bracket-based method.
```

Do not return only generic messages such as "Error occurred."

Keep machine-readable status codes alongside human-readable explanations.

---

# 14. PHASE 8 — ALGORITHM COMPARISON ENGINE

Implement:

`src/benchmarking/algorithm_comparison.c`

Allow multiple methods to solve the same problem.

Compare:

- Numerical accuracy.
- Residual.
- Estimated error.
- Execution time.
- Iterations.
- Function evaluations.
- Memory consumption where measurable.
- Convergence behaviour.
- Reliability.
- Failure rate.

Use identical problem definitions and comparable configurations.

The comparison engine should produce structured data that the CLI can display as tables.

Do not mix benchmarking code into the numerical algorithms themselves.

---

# 15. PHASE 9 — ACCURACY VS PERFORMANCE ANALYSIS

Implement:

`src/benchmarking/accuracy_performance.c`

Allow the user to vary tolerances and study how numerical accuracy changes against computational cost.

For example:

Test tolerances:

- 1e-2
- 1e-4
- 1e-6
- 1e-8
- 1e-10

Measure:

- Actual error against known reference results.
- Execution time.
- Iterations.
- Function evaluations.
- Memory usage where relevant.

Generate CSV outputs suitable for plotting.

Provide data for graphs such as:

- Tolerance vs execution time.
- Tolerance vs actual error.
- Iterations vs actual error.
- Accuracy vs computational cost.

Do not claim a method is more accurate solely because it uses a smaller requested tolerance.

Measure actual error against known reference values.

---

# 16. PHASE 10 — NUMERICAL RECIPES AND STANDARD LIBRARY COMPARISON

This is a mandatory final project milestone.

At the end, compare NUMERIX with established numerical libraries.

Include:

1. GNU Scientific Library (GSL).
2. BLAS.
3. LAPACK.
4. Numerical Recipes, where a suitable and legally usable reference implementation is available.

## 16.1 GSL

Use GSL as the primary external numerical library comparison.

Compare corresponding algorithms for:

- Root finding.
- Integration.
- Differentiation where suitable equivalents exist.
- Linear algebra.
- Interpolation.
- ODE solving.
- Optimization.

Only compare equivalent or reasonably corresponding methods.

## 16.2 BLAS and LAPACK

Use BLAS and LAPACK for relevant matrix/vector and linear algebra comparisons.

Do not claim that BLAS or LAPACK directly provide counterparts for every NUMERIX algorithm.

Compare only appropriate operations.

## 16.3 Numerical Recipes

Establish a specific reference edition and source before performing a direct implementation comparison.

Respect its licensing and copyright.

Do not copy or redistribute restricted Numerical Recipes source code without permission.

If a usable reference implementation is unavailable:

- Compare the documented algorithms and mathematical approaches.
- Clearly label the comparison as an academic/reference comparison.
- Do not claim that a direct source-code benchmark was performed.

## 16.4 Benchmark methodology

Use:

- Identical test problems.
- Equivalent algorithms.
- Equivalent numerical tolerances.
- Comparable compiler optimization settings.
- The same machine.
- The same hardware configuration.
- Repeated runs.
- Documented measurement methodology.

Compare:

- Accuracy.
- Runtime.
- Memory usage.
- Iteration counts.
- Function evaluations.
- Convergence.
- Numerical stability.
- Error handling.
- Reliability.

Document any differences in API overhead or algorithm implementation that affect fairness.

## 16.5 Final evaluation

Create:

`documentation/experimental_evaluation/`

Include:

- Benchmark methodology.
- Test datasets and problems.
- Compiler and hardware details.
- Results tables.
- Graphs.
- Accuracy analysis.
- Performance analysis.
- Reliability analysis.
- Strengths and weaknesses.
- Limitations.
- Final conclusions.

Never fabricate benchmark results.

Only publish measurements actually obtained.

---

# 17. PHASE 11 — NUMERICAL RELIABILITY REPORTS

Every numerical computation should be capable of producing a structured reliability report.

Include:

- Input problem.
- Selected method.
- Recommended method, if applicable.
- Actual method used.
- Computed result.
- Residual.
- Estimated error, when available.
- Iteration count.
- Function evaluation count.
- Execution time.
- Convergence status.
- Warnings.
- Errors.
- Fallback history.
- Relevant numerical diagnostics.

The CLI should present this information clearly.

For methods where some metrics are unavailable, indicate that they were not measured or are not applicable.

---

# 18. PHASE 12 — STEP-BY-STEP EXECUTION MODE

Implement:

`src/execution/step_execution.c`

Provide an optional mode to inspect the numerical computation process.

Examples:

### Bisection

Show:

- Initial interval.
- Midpoint.
- Function value.
- Updated interval.
- Error or convergence estimate.
- Iteration number.

### Newton-Raphson

Show:

- Current estimate.
- Function value.
- Derivative value.
- Next estimate.
- Convergence information.

### Gaussian Elimination

Show:

- Pivot selection.
- Row swaps.
- Elimination operations.
- Intermediate matrices.
- Back substitution.

### Integration

Show:

- Subinterval boundaries.
- Sampled function values.
- Contributions to the integral.
- Running total.

### ODE

Show:

- Current step.
- Current state.
- Intermediate RK4 evaluations.
- Next state.

Do not force step-by-step storage when disabled.

Avoid excessive memory usage by allowing configurable history limits.

---

# 19. PHASE 13 — SENSITIVITY AND CONDITION ANALYSIS

Implement:

`src/diagnostics/sensitivity.c`

And:

`src/diagnostics/condition_analysis.c`

Study how small changes in inputs affect numerical outputs.

Include:

- Input perturbation experiments.
- Relative and absolute sensitivity.
- Matrix condition numbers where supported.
- Ill-conditioning warnings.
- Sensitivity reports.

For example, slightly perturb the input parameters and observe how much the result changes.

Distinguish sensitivity analysis from rigorous mathematical condition-number calculations.

Clearly label empirical estimates.

---

# 20. PHASE 14 — COMPUTATION LIMITS AND CANCELLATION

Implement:

`src/execution/execution_limits.c`

Support:

- Maximum iteration count.
- Maximum execution time.
- Maximum allocation limits where practical.
- User-defined tolerances.
- Cooperative cancellation.
- Resource usage monitoring where supported.

Every potentially long-running numerical algorithm should check its execution context at safe points.

Cancellation must return a clear status and preserve a consistent result structure.

Do not use unsafe forced thread termination.

---

# 21. PHASE 15 — INTELLIGENT CONVERGENCE MONITORING

Monitor numerical execution for:

- Convergence.
- Divergence.
- Stagnation.
- Oscillation.
- Slow convergence.
- Residual behaviour.
- Error behaviour.

Provide iteration history when requested.

The system should be able to warn when progress appears unreliable.

Do not automatically declare divergence based on a single iteration or arbitrary thresholds.

Make monitoring criteria configurable and document them.

---

# 22. PHASE 16 — ADVANCED TESTING AND VALIDATION

Build a comprehensive test framework.

The project must not be considered complete merely because it compiles.

## 22.1 Unit tests

Test every numerical algorithm independently.

## 22.2 Analytical validation

Use functions and problems with known analytical solutions.

Examples:

- Polynomial roots.
- Known integrals.
- Known derivatives.
- Linear systems with known solutions.
- ODEs with known analytical solutions.

Compare computed results against reference values.

## 22.3 Edge-case testing

Include:

- Zero inputs.
- Negative inputs where valid.
- Very large values.
- Very small values.
- Invalid values.
- NaN.
- Infinity.
- Singular matrices.
- Nearly singular matrices.
- Duplicate interpolation points.
- Discontinuous functions.
- Invalid intervals.
- Very strict tolerances.
- Maximum iteration limits.

## 22.4 Metamorphic testing

Use mathematical properties to test algorithms even when exact answers are not readily available.

Examples:

- Integrating a function over two adjacent intervals should agree with integrating over their union when the integral exists.
- Multiplying both sides of a linear system by an appropriate nonzero scalar should preserve the solution.
- Differentiating a scaled function should scale the derivative accordingly.
- Repeated runs with identical deterministic inputs should produce consistent results within the documented numerical limits.

## 22.5 Failure injection

Test:

- Allocation failures.
- Invalid callback behaviour.
- Parser errors.
- Cancellation.
- Thread startup failure.
- Worker failures.
- Resource limits.

## 22.6 Concurrency tests

Test:

- Multiple simultaneous solvers.
- Safe cancellation.
- Shared immutable functions.
- Worker isolation.
- Thread joining.
- Resource cleanup.
- Data races where tooling is available.

## 22.7 Numerical stability tests

Test:

- Poorly conditioned matrices.
- Catastrophic cancellation.
- Floating-point rounding effects.
- Overflow and underflow.
- Sensitivity to step size.
- Convergence under different tolerances.

Run tests on both Windows and Linux.

Use sanitizers and memory-checking tools where available.

Document tests that cannot be run on a particular platform.

---

# 23. PHASE 17 — CLI DEVELOPMENT

The CLI is the primary demonstration interface.

It should be clear and easy to use.

Example interface:

```text
========================================
             NUMERIX
 Intelligent Numerical Computation
========================================

1. Root Finding
2. Linear Algebra
3. Interpolation
4. Integration
5. Differentiation
6. Optimization
7. ODE Solving
8. Piecewise Functions
9. Method Recommendation
10. Parallel Solver
11. Algorithm Comparison
12. Accuracy vs Performance
13. Sensitivity Analysis
14. Step-by-Step Execution
15. Exit
```

Support mathematical expression input.

Example:

```text
Select operation: Integration

Enter function:
sin(x) + x^2

Lower limit: 0
Upper limit: 5

Method:
1. Trapezoidal
2. Simpson
3. Automatic recommendation

Selection: 3
```

Also provide a way to use C callbacks through the public API.

The CLI should support:

- Input validation.
- Clear error messages.
- Method selection.
- Automatic recommendations.
- Step-by-step mode.
- Comparison mode.
- Reliability reports.
- Cancellation where feasible.
- CSV export for experiments.

Do not put numerical algorithms directly into CLI source files.

CLI code must call the public library API.

---

# 24. PHASE 18 — BUILD SYSTEM AND CROSS-PLATFORM SUPPORT

Provide:

- Makefile.
- CMakeLists.txt.

Support Linux and Windows builds.

Keep platform-specific implementation isolated in `platform/`.

Provide build configurations for:

- Debug.
- Release.
- Testing.
- Benchmarking.

Support suitable compilers such as GCC, Clang, and MSVC where practical.

Use consistent compiler warning levels.

Do not silently suppress warnings.

Provide clear instructions for optional external dependencies such as GSL, BLAS and LAPACK.

The core NUMERIX library should not require these external libraries just to perform its own numerical methods.

They are required only for the relevant external benchmark integrations.

---

# 25. PHASE 19 — ADDITIONAL FEATURES AND FUTURE EXTENSIONS

Design the architecture so the following can be added without major restructuring.

These are future extensions and should not delay completion of the core requirements.

## 25.1 Automatic differentiation

Use dual numbers or a similar approach to calculate derivatives automatically.

## 25.2 Sparse matrix support

Add sparse matrix storage and suitable sparse algorithms.

## 25.3 Verified interval arithmetic

Support interval-based numerical verification where feasible.

## 25.4 Input uncertainty propagation

Study how uncertainty in input parameters affects numerical results.

## 25.5 Deterministic parallel computation

Use controlled reduction ordering where reproducibility is required.

## 25.6 Checkpoint and resume

Allow selected long-running numerical computations to save and restore execution state.

## 25.7 Automatic breakpoint detection

Add heuristic methods for detecting possible discontinuities in sampled functions.

Clearly distinguish heuristic detection from mathematical proof.

## 25.8 Numerical problem visualization

Allow the CLI to export suitable data for external plotting tools.

## 25.9 Reproducible experiment runner

Allow users to repeat benchmark experiments using saved configurations.

---

# 26. CODING RULES AND QUALITY REQUIREMENTS

These rules apply throughout development.

1. Use standard C11.
2. Write readable, maintainable code.
3. Keep functions reasonably small and focused.
4. Avoid unnecessary global variables.
5. Avoid duplicated algorithm implementations.
6. Validate inputs before numerical execution.
7. Check allocation failures.
8. Check all relevant floating-point results for NaN and infinity.
9. Document public functions.
10. Document numerical assumptions.
11. Use consistent naming conventions.
12. Keep implementation details private.
13. Do not mix CLI logic with library algorithms.
14. Do not mix benchmarking logic with algorithm implementations.
15. Avoid unnecessary external dependencies.
16. Do not introduce C++ into the C library.
17. Do not use unsafe thread cancellation.
18. Do not silently ignore errors.
19. Do not invent numerical error estimates.
20. Do not claim arbitrary functions can be perfectly analyzed.
21. Do not fabricate test or benchmark results.
22. Do not create empty placeholder files.
23. Do not delete working original code without a tested replacement.
24. Keep the build reproducible.
25. Maintain Windows and Linux compatibility.

Use `double` as the default numerical type unless a specific algorithm or experiment justifies another precision.

Document floating-point limitations.

---

# 27. DOCUMENTATION REQUIREMENTS

Maintain documentation as the project develops.

Create:

### Architecture

- Overall architecture.
- Module responsibilities.
- Data flow.
- Function representation.
- Piecewise engine design.
- Execution and threading design.

### Algorithms

For every algorithm document:

- Mathematical explanation.
- Formula.
- Required inputs.
- Assumptions.
- Convergence characteristics.
- Limitations.
- Error handling.

### API

Document:

- Every public type.
- Every public function.
- Parameters.
- Return values.
- Memory ownership.
- Error statuses.
- Usage examples.

### User guide

Explain:

- Building NUMERIX.
- Using the CLI.
- Using C callbacks.
- Entering mathematical expressions.
- Solving piecewise problems.
- Reading reliability reports.
- Running comparisons.
- Exporting benchmark results.

### Testing

Document:

- Test strategy.
- Test problems.
- Analytical reference values.
- Platform coverage.
- Known limitations.

### Experimental evaluation

Document the final comparison against GSL, BLAS, LAPACK and Numerical Recipes where appropriate.

---

# 28. DEVELOPMENT WORKFLOW — STRICT PHASE-BY-PHASE EXECUTION

Do not attempt to implement all phases in one operation.

Follow this workflow:

## At the beginning of each phase

1. Read the architecture and existing documentation.
2. Inspect the relevant source files.
3. Identify dependencies on previously completed phases.
4. Explain what will be implemented.
5. List the files that will be created or modified.
6. Identify possible compatibility risks.

## During implementation

1. Implement only the current phase.
2. Reuse existing code where appropriate.
3. Keep modules independent.
4. Write tests alongside the implementation.
5. Compile frequently.
6. Fix warnings and errors.
7. Update documentation.
8. Do not leave broken intermediate code.

## At the end of each phase

Provide a report containing:

- Features implemented.
- Files created.
- Files modified.
- Existing code reused.
- Tests added.
- Tests executed.
- Test results.
- Build results.
- Known issues.
- Remaining work.
- Whether the phase acceptance criteria were met.

Do not claim a phase is complete if its required tests have not passed.

If a feature cannot be implemented reliably, explain the limitation rather than creating a misleading placeholder.

Do not move to the next phase until the current phase is stable.

If you discover that an architectural change is necessary, explain why and obtain approval before changing the agreed architecture.

---

# 29. FINAL PROJECT ACCEPTANCE CRITERIA

NUMERIX will be considered complete only when the following requirements are satisfied.

## Core numerical library

- All original required algorithms are implemented.
- All methods are accessible through the public API.
- Existing working code has been migrated or retained appropriately.
- Examples demonstrate the modules.

## Function input

- Mathematical expressions work.
- C callbacks work.
- Both use the same internal function abstraction.

## Piecewise engine

- Multiple intervals work.
- Breakpoints are supported.
- Open and closed boundaries work.
- Gaps and overlaps are handled explicitly.
- Numerical algorithms can operate across valid segments.
- Discontinuity handling is tested.

## Intelligence

- Method recommendations work.
- Recommendations explain their reasoning.
- Automatic fallback works.
- Parallel solver execution works safely.

## Reliability

- Structured results are available.
- Error detection works.
- Convergence monitoring works.
- Step-by-step execution works.
- Sensitivity and condition analysis are available.
- Execution limits and cancellation work.

## Benchmarking

- Algorithm comparison works.
- Accuracy versus performance experiments work.
- GSL comparison is completed for relevant algorithms.
- BLAS and LAPACK comparisons are completed for relevant operations.
- Numerical Recipes comparison is completed where a suitable reference is available.
- Final evaluation includes actual measurements and documented methodology.

## Platform and build

- Linux build works.
- Windows build works.
- Makefile works.
- CMake works.
- Tests pass on supported platforms.

## Documentation

- Architecture documentation is complete.
- API documentation is complete.
- User guide is complete.
- Algorithm documentation is complete.
- Testing documentation is complete.
- Final experimental evaluation is complete.

---

# 30. FIRST ACTION TO TAKE

Start with **Phase 0: Repository Inspection and Architecture Preparation**.

Do not immediately start writing the complete library.

Your first response should:

1. Confirm the repository has been inspected.
2. Summarize the current implementation.
3. Identify what can be reused.
4. Identify what needs modification.
5. Present the proposed migration plan.
6. Identify any architectural conflicts or missing information.

Then prepare the initial architecture and API design.

Wait for approval before proceeding with substantial implementation.

**Final instruction:** NUMERIX must be developed as one coherent, modular numerical computation library. Every module must connect to the common API and function representation. The piecewise engine must coordinate existing numerical algorithms rather than duplicate them. The intelligence, diagnostics, benchmarking and execution layers must build on the numerical foundation, not operate as disconnected demonstrations.

Prioritize numerical correctness, reliability, maintainability, portability and measurable performance over simply producing a large amount of code.