# Algorithms and Numerical Limitations

This document summarizes the mathematical methods exposed by the library. The formulas describe the methods; floating-point implementations can differ from exact arithmetic.

## 1. Root finding

Each root-finding method seeks an approximate solution of \(f(x)=0\). The legacy routines attempt at most 1,000 iterations. NUMERIX scalar wrappers use a separate 2,048-iteration cap.

### Bisection

Given an interval \([a,b]\) whose endpoint function values have opposite signs, repeatedly evaluate the midpoint

\[
m = a + \frac{b-a}{2}.
\]

Keep the half-interval whose endpoint values bracket a root. The implementation stops when the midpoint is an exact zero or the interval half-width is no greater than the requested tolerance.

**Strengths:** robust when the function is continuous on the interval and the initial bracket is valid.

**Limitations:** requires a valid sign-changing bracket unless an endpoint is already a root; convergence is comparatively slow. A small interval width does not by itself guarantee a small function residual \(|f(m)|\).

### Newton–Raphson

Starting from \(x_k\), update

\[
x_{k+1}=x_k-\frac{f(x_k)}{f'(x_k)}.
\]

The implementation can stop when the function value is exactly zero or when the step size \(|x_{k+1}-x_k|\) is no greater than the tolerance.

**Strengths:** often converges quickly near a simple root when the starting point and derivative are suitable.

**Limitations:** requires a derivative callback; a zero or unusable derivative causes failure; a poor initial guess can diverge or converge to another root. A small step is not a universal guarantee of a small residual.

### Secant method

Using two initial estimates, construct a secant-line approximation and update

\[
x_{k+1}=x_k-f(x_k)
\frac{x_k-x_{k-1}}{f(x_k)-f(x_{k-1})}.
\]

The method uses function values rather than a derivative callback. The implementation stops at an exact zero or when successive estimates differ by no more than the tolerance.

**Limitations:** the denominator can become zero or numerically unusable, and convergence is not guaranteed. A bracketing interval is not required.

## 2. Numerical differentiation

For a function \(f\), point \(x\), and positive step \(h\):

### Forward difference

\[
f'(x)\approx\frac{f(x+h)-f(x)}{h}.
\]

For sufficiently smooth functions, its leading truncation error is generally \(O(h)\).

### Backward difference

\[
f'(x)\approx\frac{f(x)-f(x-h)}{h}.
\]

Its leading truncation error is generally \(O(h)\) for sufficiently smooth functions.

### Central difference

\[
f'(x)\approx\frac{f(x+h)-f(x-h)}{2h}.
\]

For sufficiently smooth functions, its leading truncation error is generally \(O(h^2)\), so it is often more accurate than one-sided differences at a suitable step size.

### Step-size trade-off

A step that is too large increases truncation error. A step that is too small can amplify floating-point rounding and cancellation. The implementation rejects non-positive or non-finite steps, unusable evaluation coordinates, and non-finite callback results. The caller remains responsible for choosing a meaningful step for the function and scale.

## 3. Interpolation

Interpolation estimates a value from known data points. For polynomial interpolation, the \(x\)-coordinates must be distinct.

### Linear interpolation

For two points \((x_0,y_0)\) and \((x_1,y_1)\),

\[
y(x)=y_0+\frac{x-x_0}{x_1-x_0}(y_1-y_0).
\]

If \(x\) lies outside the interval between the two nodes, the same formula performs extrapolation. The denominator must be nonzero.

### Lagrange polynomial interpolation

For \(n\) distinct nodes, the interpolating polynomial can be written

\[
P(x)=\sum_{i=0}^{n-1} y_i L_i(x),
\qquad
L_i(x)=\prod_{\substack{j=0\\j\ne i}}^{n-1}
\frac{x-x_j}{x_i-x_j}.
\]

This implementation uses \(O(n^2)\) arithmetic and no dynamic allocation. Duplicate nodes are invalid because they create zero denominators.

### Newton divided-difference interpolation

Newton interpolation represents the polynomial in nested form:

\[
P(x)=c_0+c_1(x-x_0)+c_2(x-x_0)(x-x_1)+\cdots.
\]

The coefficients \(c_i\) are obtained from divided differences. The implementation uses \(O(n^2)\) arithmetic and \(O(n)\) temporary storage. It checks for invalid input, impossible allocation sizes, allocation failure, and detected non-finite intermediate values.

### Interpolation limitations

High-degree global polynomial interpolation can be poorly conditioned, especially with many or unevenly spaced nodes. Roundoff can grow, and extrapolation is generally less reliable than interpolation within the data range. The robustness suite includes 256-point tests designed to exercise handling and known-node behavior; these are not a guarantee that arbitrary high-degree interpolation is accurate.

## 4. Floating-point and failure behavior

All calculations use C `double`. Finite precision means mathematical identities need not hold exactly in computation. Detected invalid input, non-finite arithmetic, convergence failure, or allocation failure produces a diagnostic and `NAN`, as documented by the public header. Callers should inspect the returned value and independently validate residuals or errors when application accuracy matters.

## 5. Additional NUMERIX algorithms

### Dense linear algebra

Vector dot products and matrix-vector products use row-major storage. The Euclidean norm uses a scaled sum-of-squares calculation to avoid avoidable overflow/underflow. Gaussian elimination and LU factorization use partial pivoting. Exact zero pivots are reported as singular; the current routines do not estimate condition numbers or classify near-singular matrices. Work arrays are temporary, and Gaussian elimination writes the solution buffer only after a successful solve.

### Composite integration

The trapezoid rule uses endpoint half-weights and interior unit weights. Simpson integration uses the standard `1,4,2,...,4,1` weights and requires an even number of panels. Both use compensated accumulation, support reversed bounds, and reject non-finite function values. They do not estimate truncation error, adapt panel counts, or detect discontinuities. More than ten million panels are rejected.

### Golden-section minimization

The optimizer repeatedly contracts a finite interval using the golden ratio and returns the estimated minimizer location. It assumes the objective is unimodal over the interval. It reports non-convergence as `NAN` if the requested interval width is not reached within the iteration limit. It does not prove global optimality or return the objective value.

### Classical fixed-step RK4

The ODE routine applies the classical four-stage Runge-Kutta method to a scalar initial value problem. It supports forward or backward time and shortens the last step to end at the requested time. The caller supplies a maximum step count. The solver has no adaptive error estimate; accuracy depends on the caller's step size and the problem's smoothness.
