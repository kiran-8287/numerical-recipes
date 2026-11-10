# NUMERIX API reference

The public declarations are in [`include/numerix.h`](../include/numerix.h). The API is still evolving; this page describes the callable numerical subset and its current contracts. Several enum values and reports are groundwork for methods that are not implemented yet.

## Callback functions

```c
typedef double (*NUMERIX_EvalCallback)(double x, void *user_data);
typedef double (*NUMERIX_DerivativeCallback)(double x, void *user_data);
```

Initialize callback functions with `numerix_function_init`. The callback and `user_data` remain caller-owned and must remain valid while in use. A derivative callback is optional except when calling Newton-Raphson.

```c
NUMERIX_Function f;
if (numerix_function_init(&f, evaluate, derivative, context) != NUMERIX_STATUS_OK) {
    /* Handle invalid function setup. */
}
```

## Expression functions

`numerix_expression_parse` parses the supported scalar expression grammar and attaches an owned AST to a `NUMERIX_Function`. Pass a zero-initialized or previously initialized function object. Call `numerix_function_destroy` once when finished. A parsed expression supports evaluation, root finding, and finite-difference derivatives; it does not provide an analytic derivative callback.

Expressions are limited to 1,024 input characters. The grammar supports `x`, constants `pi` and `e`, finite decimal/scientific numeric literals, parentheses, unary `+`/`-`, binary `+`, `-`, `*`, `/`, and `^`, and the single-argument functions `sin`, `cos`, `tan`, `asin`, `acos`, `atan`, `exp`, `log`, `log10`, `sqrt`, and `abs`. Powers associate right-to-left and bind more tightly than a leading unary sign (`-2^2` is `-(2^2)`); a signed exponent is allowed (`2^-2`). Implicit multiplication and multi-argument functions are not supported. Invalid syntax and out-of-range literals return a non-OK status.

```c
NUMERIX_Function f = {0};
if (numerix_expression_parse(&f, "x^2 - 2") == NUMERIX_STATUS_OK) {
    double root = numerix_bisection(&f, 1.0, 2.0, 1e-9);
    /* Check root with isfinite() and validate the residual. */
    numerix_function_destroy(&f);
}
```

`numerix_function_evaluate` evaluates callback, expression, or bound piecewise functions. It returns `NAN` when the function is invalid or evaluation fails.

## Root finding

| Function | Requirements | Current behavior |
|---|---|---|
| `numerix_bisection(f, a, b, tol)` | Valid function; finite `a < b`; finite `tol > 0`; endpoint values bracket a sign change or are zero | Returns an endpoint or an interval midpoint; `NAN` for invalid brackets, non-finite values, or inability to make progress. |
| `numerix_newton_raphson(f, x0, tol)` | Valid function with derivative callback; finite initial guess and positive finite tolerance | Stops on an exact zero or sufficiently small step; `NAN` on zero/non-finite derivative, invalid arithmetic, or iteration exhaustion. |
| `numerix_secant(f, x0, x1, tol)` | Valid function; distinct finite guesses; positive finite tolerance | Stops on an exact zero or sufficiently small step; `NAN` on invalid arithmetic or iteration exhaustion. |

These scalar functions use a fixed internal maximum of 2,048 iterations and return `double`; they do not populate `NUMERIX_Result` or honor the `NUMERIX_Problem` execution limits. A finite return is not a certified error bound; evaluate the residual independently.

## Differentiation

`numerix_forward_difference`, `numerix_backward_difference`, and `numerix_central_difference` evaluate callback or expression functions at finite offsets. Each requires a finite point, finite `h > 0`, distinct representable sample points, and finite function values. The routines return `NAN` on detected invalid inputs or non-finite arithmetic. Central difference has second-order truncation error for sufficiently smooth functions; the one-sided forms have first-order truncation error.

## Interpolation

`numerix_linear_interpolation`, `numerix_lagrange_interpolation`, and `numerix_newton_interpolation` use the corresponding legacy interpolation implementation. Linear interpolation takes two points. Polynomial methods take caller-owned `x_values[]`, `y_values[]`, a positive point count, and query `x`; node coordinates must be distinct and input values finite. Input arrays are not modified. Newton interpolation allocates temporary coefficient storage and returns `NAN` if allocation fails.

## Linear algebra

Matrices use contiguous row-major `double` arrays. `numerix_vector_dot` and `numerix_vector_norm2` compute a dot product and Euclidean norm. `numerix_matrix_vector_multiply` multiplies an `rows` by `columns` matrix by a vector. `numerix_gaussian_elimination` solves a dense square system using partial pivoting; it returns `NUMERIX_STATUS_SINGULAR_MATRIX` for an exactly zero pivot and only copies a solution out on success. `numerix_lu_decomposition` returns `L`, `U`, and a row permutation such that `P*A = L*U`; output buffers must be distinct and large enough. Near-singular conditioning is not estimated.

## Integration

`numerix_trapezoidal_integral(function, a, b, panels)` computes the composite trapezoid rule. `numerix_simpson_integral` computes composite Simpson integration and requires an even panel count of at least two. Both support reversed bounds, callback or expression functions, and return `NAN` for invalid inputs, non-finite evaluations, or non-finite accumulation. Panel counts above ten million are rejected as a resource guard. The panel count controls discretization; these routines do not estimate their own error.

## Optimization

`numerix_golden_section_minimize(function, a, b, tolerance, max_iterations)` returns the estimated location of a minimum on a finite interval. The method assumes a unimodal objective over the interval. It returns `NAN` if the input/evaluations are invalid or the interval does not reach the requested width before the iteration limit. It does not return the objective value or a global-optimality certificate.

## Ordinary differential equations

`numerix_rk4_solve(derivative, user_data, t0, y0, t1, step_size, max_steps, &y_final, &steps_taken)` integrates a scalar initial value problem `y' = derivative(t, y, user_data)` with classical fixed-step RK4. The final step is shortened to land on `t1`; backward integration is supported. Invalid data, non-finite stages, and step limits return a status. On failure during the integration, output values are not committed. RK4 has no adaptive error estimate, and users must select a suitable step size.

## Piecewise functions

Initialize a `NUMERIX_PiecewiseFunction`, add non-overlapping segments, and bind it to a `NUMERIX_Function` with `numerix_piecewise_bind`. The container owns its segment and breakpoint buffers. Segment function objects are copied by value, but callback contexts and expression ASTs remain borrowed; keep them alive until the piecewise container is no longer used. `numerix_piecewise_destroy` frees only the container buffers.

Piecewise evaluation returns `NAN` outside included segment bounds or in gaps. `numerix_piecewise_solve_root` is an early implementation; it has not been validated for every boundary, gap, or discontinuity case.

## Status and ownership

Status-returning helpers use `NUMERIX_Status`; convert values with `numerix_status_string`. `numerix_malloc`/`numerix_free` are allocator helpers, not required for caller-owned callback data. Destroy each owned expression exactly once. Do not shallow-copy an owning expression function and destroy both copies.

The legacy interface remains available through `include/numerical_recipes.h`; its contract is documented in that header. Both interfaces are included in `libnumerix.a`.

## Comparison and accuracy reports

`NUMERIX_Problem.has_reference_solution` and `reference_solution` provide an optional known answer for validation. `numerix_accuracy_study` records up to 16 measured solver residuals, elapsed times, and function-evaluation counts. It sets `actual_error`/`best_error` only when an exact reference was supplied; without one these fields are `NAN`, while residuals remain available. Iteration counts are currently not instrumented and are reported as zero with a note. `numerix_compare_standard_libraries` measures the internal NUMERIX baseline only; external adapters are unavailable and explicitly marked so.

## Sensitivity and conditioning

`numerix_sensitivity_experiment` evaluates a function at `problem.x0` and at a positive perturbation to the right. `absolute_sensitivity` is the absolute output change; `relative_sensitivity` is that change divided by the baseline magnitude and is undefined (reported as `NAN` with a warning status) when the baseline is zero. `numerix_condition_analysis` estimates the scalar relative condition number `|x f'(x) / f(x)|` using a central finite difference. The estimate is local and numerical, and is undefined when `f(x)` is zero.

## Not-yet-implemented API areas

Although related method constants or CLI menu entries exist, the public header currently has no callable linear algebra, integration, optimization, or ODE solver APIs. Parallel execution, structured result-producing solver calls, and complete execution-limit integration are also outstanding.
