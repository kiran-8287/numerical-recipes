#include <math.h>
#include <stdio.h>
#include "numerix.h"

static double square(double x, void *context)
{
    (void)context;
    return x * x;
}

static double quadratic(double x, void *context)
{
    double center = *(const double *)context;
    double delta = x - center;
    return delta * delta + 2.0;
}

static double exponential_derivative(double t, double y, void *context)
{
    (void)t;
    (void)context;
    return y;
}

static double unit_derivative(double t, double y, void *context)
{
    (void)t;
    (void)y;
    (void)context;
    return 1.0;
}

static int close_to(double actual, double expected, double tolerance)
{
    return isfinite(actual) && fabs(actual - expected) <= tolerance;
}

int main(void)
{
    const double a[] = {0.0, 2.0, 1.0, 3.0};
    const double rhs[] = {4.0, 7.0};
    const double vector[] = {1.0, 2.0, 3.0};
    const double other[] = {4.0, 5.0, 6.0};
    const double matrix[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    const double singular[] = {1.0, 2.0, 2.0, 4.0};
    double solution[2] = {-1.0, -1.0};
    double lower[4];
    double upper[4];
    size_t permutation[2];
    double dot = 0.0;
    double norm = 0.0;
    double product[2];
    double center = 1.25;
    NUMERIX_Function function;
    double minimum;
    double integral;
    double y = -1.0;
    size_t steps = 0U;
    size_t i;
    size_t j;

    if (numerix_vector_dot(vector, other, 3U, &dot) != NUMERIX_STATUS_OK || dot != 32.0 ||
        numerix_vector_norm2((double[]){3.0, 4.0}, 2U, &norm) != NUMERIX_STATUS_OK || norm != 5.0) {
        fputs("vector operation failed\n", stderr);
        return 1;
    }
    if (numerix_matrix_vector_multiply(matrix, vector, 2U, 3U, product) != NUMERIX_STATUS_OK ||
        product[0] != 14.0 || product[1] != 32.0) {
        fputs("matrix-vector product failed\n", stderr);
        return 2;
    }
    if (numerix_gaussian_elimination(a, rhs, 2U, solution) != NUMERIX_STATUS_OK ||
        !close_to(solution[0], 1.0, 1e-14) || !close_to(solution[1], 2.0, 1e-14)) {
        fputs("partial-pivoting solve failed\n", stderr);
        return 3;
    }
    if (numerix_lu_decomposition(a, 2U, lower, upper, permutation) != NUMERIX_STATUS_OK) {
        fputs("LU factorization failed\n", stderr);
        return 4;
    }
    for (i = 0U; i < 2U; ++i) {
        for (j = 0U; j < 2U; ++j) {
            double reconstructed = lower[i * 2U] * upper[j] +
                                   lower[i * 2U + 1U] * upper[2U + j];
            if (!close_to(reconstructed, a[permutation[i] * 2U + j], 1e-12)) {
                fputs("permuted LU reconstruction failed\n", stderr);
                return 5;
            }
        }
    }
    if (numerix_gaussian_elimination(singular, rhs, 2U, solution) !=
        NUMERIX_STATUS_SINGULAR_MATRIX || solution[0] != 1.0 || solution[1] != 2.0) {
        fputs("singular matrix was not reported\n", stderr);
        return 6;
    }
    if (numerix_matrix_vector_multiply(matrix, vector, (size_t)-1, 2U, product) !=
        NUMERIX_STATUS_INVALID_ARGUMENT) {
        fputs("matrix dimension overflow was not rejected\n", stderr);
        return 15;
    }

    if (numerix_function_init(&function, square, NULL, NULL) != NUMERIX_STATUS_OK) {
        fputs("function initialization failed\n", stderr);
        return 7;
    }
    integral = numerix_trapezoidal_integral(&function, 0.0, 1.0, 10000U);
    if (!close_to(integral, 1.0 / 3.0, 1e-8)) {
        fputs("trapezoidal integral inaccurate\n", stderr);
        return 8;
    }
    integral = numerix_simpson_integral(&function, 0.0, 1.0, 10U);
    if (!close_to(integral, 1.0 / 3.0, 1e-14) ||
        !close_to(numerix_simpson_integral(&function, 1.0, 0.0, 10U), -1.0 / 3.0, 1e-14) ||
        !isnan(numerix_simpson_integral(&function, 0.0, 1.0, 3U)) ||
        !isnan(numerix_trapezoidal_integral(&function, 0.0, 1.0, 10000001U))) {
        fputs("Simpson integral or panel validation failed\n", stderr);
        return 9;
    }

    if (numerix_function_init(&function, quadratic, NULL, &center) != NUMERIX_STATUS_OK) {
        fputs("optimization function initialization failed\n", stderr);
        return 10;
    }
    minimum = numerix_golden_section_minimize(&function, -3.0, 5.0, 1e-9, 256U);
    if (!close_to(minimum, center, 1e-7) ||
        isfinite(numerix_golden_section_minimize(&function, -3.0, 5.0, 1e-12, 1U))) {
        fputs("golden-section minimization failed convergence checks\n", stderr);
        return 11;
    }

    if (numerix_rk4_solve(exponential_derivative, NULL, 0.0, 1.0, 1.0,
                          0.1, 20U, &y, &steps) != NUMERIX_STATUS_OK ||
        !close_to(y, exp(1.0), 3e-6) || steps != 10U) {
        fputs("RK4 exponential solution failed\n", stderr);
        return 12;
    }
    y = -1.0;
    if (numerix_rk4_solve(unit_derivative, NULL, 2.0, 2.0, 0.0,
                          0.1, 30U, &y, &steps) != NUMERIX_STATUS_OK ||
        !close_to(y, 0.0, 1e-14) || steps != 20U) {
        fputs("backward-time RK4 failed\n", stderr);
        return 13;
    }
    y = 123.0;
    if (numerix_rk4_solve(unit_derivative, NULL, 0.0, 0.0, 1.0,
                          0.1, 2U, &y, &steps) != NUMERIX_STATUS_RESOURCE_LIMIT ||
        y != 123.0) {
        fputs("RK4 resource limit or output preservation failed\n", stderr);
        return 14;
    }

    puts("Advanced numerical algorithms OK (linear algebra, integration, optimization, RK4)");
    return 0;
}
