# NUMERIX Phase-wise Implementation Plan

## Phase 0 — Architecture and API foundation
Status: Implemented in the current working branch.

Objectives:
- establish the NUMERIX public API and common data model;
- define function, problem, result, options, and execution context types;
- document the memory ownership and diagnostic conventions;
- keep the working numerical library intact while adding the new architecture layer.

Files introduced or updated:
- [include/numerix.h](../../include/numerix.h)
- [src/core/problem.c](../../src/core/problem.c)
- [src/core/result.c](../../src/core/result.c)
- [src/core/options.c](../../src/core/options.c)
- [src/core/execution.c](../../src/core/execution.c)
- [src/core/memory.c](../../src/core/memory.c)
- [src/core/errors.c](../../src/core/errors.c)
- [src/internal/numerix_internal.h](../../src/internal/numerix_internal.h)

## Phase 1 — Complete the original numerical library
Planned scope:
- migrate the working algorithms into the NUMERIX API layout;
- add wrappers or adapters for root finding, interpolation, and differentiation;
- ensure numerical validation and diagnostic behavior remain stable.

Key methods:
- root finding: bisection, Newton-Raphson, secant;
- differentiation: forward, backward, central finite differences;
- interpolation: linear, Lagrange, Newton divided differences.

## Phase 2 — Expression engine
Planned scope:
- tokenizer, parser, AST, and evaluator;
- support variable expressions with trigonometric, exponential, logarithmic, and algebraic operations;
- integrate expression functions into the common function model.

## Phase 3 — Piecewise and discontinuity-aware engine
Planned scope:
- interval segmentation and breakpoint handling;
- overlapping/gap validation;
- integration and root-finding across valid segments;
- coordination with existing algorithms rather than duplicating them.

## Phase 4 — Intelligent method recommendation
Planned scope:
- analyze continuity, differentiability, conditioning, and domain information;
- produce explainable recommendations with alternative methods and risk notes.

## Phase 5 — Automatic fallback and recovery
Planned scope:
- catch invalid derivative or convergence failures;
- switch to alternative methods when appropriate;
- preserve failure history and verification logs.

## Phase 6 — Parallel portfolio execution
Planned scope:
- thread-safe multi-method root-solving evaluations;
- cooperative cancellation and result verification;
- timing and accuracy comparisons.

## Phase 7 — Error detection and explanation
Planned scope:
- invalid input, domain, divergence, nearly singular matrices, cancellation, and resource-limit detection;
- machine-readable status codes plus human-readable explanations.

## Phase 8 — Algorithm comparison engine
Planned scope:
- compare multiple solver strategies on the same problem;
- produce structured benchmark output for CLI users.

## Phase 9 — Accuracy vs performance analysis
Planned scope:
- tolerance sweep experiments and output suitable for plotting;
- compare observed error against actual computation cost.

## Phase 10 — GSL, BLAS, LAPACK, and Numerical Recipes comparison
Planned scope:
- benchmark equivalent tasks against external numerical libraries;
- document methodology and limitations clearly.

## Phase 11 — Reliability reports
Planned scope:
- structured reports on result, diagnostics, residual, warnings, and fallback history.

## Phase 12 — Step-by-step execution
Planned scope:
- opt-in tracing for root-finding, ODE, integration, and elimination steps.

## Phase 13 — Sensitivity and condition analysis
Planned scope:
- perturbation experiments and empirical sensitivity reporting;
- matrix condition warnings where supported.

## Phase 14 — Computation limits and cancellation
Planned scope:
- iteration/time/allocation ceilings and cooperative cancellation.

## Phase 15 — Intelligent convergence monitoring
Planned scope:
- convergence, stagnation, and oscillation detection;
- configurable warning and iteration history reporting.

## Phase 16 — Advanced testing and validation
Planned scope:
- unit, integration, numerical stability, metamorphic, concurrency, and failure-injection tests.

## Phase 17 — CLI development
Planned scope:
- user-focused command-line interface built on the public NUMERIX API.

## Phase 18 — Build system and cross-platform support
Planned scope:
- Makefile and CMake support for Linux and Windows;
- debug/release/testing/benchmark targets;
- documentation for optional external benchmark dependencies.

## Phase 19+ — Future extensions
Planned scope:
- automatic differentiation, sparse matrices, interval arithmetic, uncertainty propagation, reproducible experiments, and visualization hooks.
