#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "numerix.h"

static int matrix_size_valid(size_t n, size_t *elements)
{
    if (n == 0U || n > SIZE_MAX / n) {
        return 0;
    }
    *elements = n * n;
    return *elements <= SIZE_MAX / sizeof(double);
}

NUMERIX_Status numerix_vector_dot(const double *a, const double *b,
                                  size_t n, double *result)
{
    size_t i;
    long double sum = 0.0L;

    if (a == NULL || b == NULL || result == NULL || n == 0U) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }
    for (i = 0U; i < n; ++i) {
        if (!isfinite(a[i]) || !isfinite(b[i])) {
            return NUMERIX_STATUS_NONFINITE_RESULT;
        }
        sum += (long double)a[i] * (long double)b[i];
        if (!isfinite(sum) || fabsl(sum) > (long double)DBL_MAX) {
            return NUMERIX_STATUS_NUMERICAL_FAILURE;
        }
    }
    *result = (double)sum;
    return NUMERIX_STATUS_OK;
}

NUMERIX_Status numerix_vector_norm2(const double *vector, size_t n,
                                    double *result)
{
    size_t i;
    double scale = 0.0;
    double sum_squares = 1.0;

    if (vector == NULL || result == NULL || n == 0U) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }
    for (i = 0U; i < n; ++i) {
        double value;
        if (!isfinite(vector[i])) {
            return NUMERIX_STATUS_NONFINITE_RESULT;
        }
        value = fabs(vector[i]);
        if (value != 0.0) {
            if (scale < value) {
                double ratio = scale / value;
                sum_squares = 1.0 + sum_squares * ratio * ratio;
                scale = value;
            } else {
                double ratio = value / scale;
                sum_squares += ratio * ratio;
            }
        }
    }
    *result = scale == 0.0 ? 0.0 : scale * sqrt(sum_squares);
    if (!isfinite(*result)) {
        return NUMERIX_STATUS_NUMERICAL_FAILURE;
    }
    return NUMERIX_STATUS_OK;
}

NUMERIX_Status numerix_matrix_vector_multiply(const double *matrix,
                                             const double *vector,
                                             size_t rows, size_t columns,
                                             double *result)
{
    size_t i;
    size_t j;
    size_t elements;
    double *temporary;

    if (matrix == NULL || vector == NULL || result == NULL || rows == 0U || columns == 0U ||
        rows > SIZE_MAX / columns || rows > SIZE_MAX / sizeof(double)) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }
    elements = rows * columns;
    if (elements > SIZE_MAX / sizeof(*matrix)) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }
    temporary = (double *)malloc(rows * sizeof(*temporary));
    if (temporary == NULL) {
        return NUMERIX_STATUS_ALLOCATION_FAILED;
    }
    for (i = 0U; i < rows; ++i) {
        long double sum = 0.0L;
        for (j = 0U; j < columns; ++j) {
            double a = matrix[i * columns + j];
            double x = vector[j];
            if (!isfinite(a) || !isfinite(x)) {
                free(temporary);
                return NUMERIX_STATUS_NONFINITE_RESULT;
            }
            sum += (long double)a * (long double)x;
        }
        if (!isfinite(sum) || fabsl(sum) > (long double)DBL_MAX) {
            free(temporary);
            return NUMERIX_STATUS_NUMERICAL_FAILURE;
        }
        temporary[i] = (double)sum;
    }
    memcpy(result, temporary, rows * sizeof(*result));
    free(temporary);
    return NUMERIX_STATUS_OK;
}

NUMERIX_Status numerix_gaussian_elimination(const double *matrix,
                                            const double *right_hand_side,
                                            size_t n, double *solution)
{
    size_t elements;
    size_t i;
    size_t j;
    size_t k;
    double *work;
    double *rhs;
    double *answer;

    if (matrix == NULL || right_hand_side == NULL || solution == NULL ||
        !matrix_size_valid(n, &elements) || n > SIZE_MAX / sizeof(double)) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }
    work = (double *)malloc(elements * sizeof(*work));
    rhs = (double *)malloc(n * sizeof(*rhs));
    answer = (double *)malloc(n * sizeof(*answer));
    if (work == NULL || rhs == NULL || answer == NULL) {
        free(work);
        free(rhs);
        free(answer);
        return NUMERIX_STATUS_ALLOCATION_FAILED;
    }
    memcpy(work, matrix, elements * sizeof(*work));
    memcpy(rhs, right_hand_side, n * sizeof(*rhs));
    for (i = 0U; i < elements; ++i) {
        if (!isfinite(work[i])) {
            free(work); free(rhs); free(answer);
            return NUMERIX_STATUS_NONFINITE_RESULT;
        }
    }
    for (i = 0U; i < n; ++i) {
        if (!isfinite(rhs[i])) {
            free(work); free(rhs); free(answer);
            return NUMERIX_STATUS_NONFINITE_RESULT;
        }
    }

    for (k = 0U; k < n; ++k) {
        size_t pivot = k;
        double largest = fabs(work[k * n + k]);
        for (i = k + 1U; i < n; ++i) {
            double candidate = fabs(work[i * n + k]);
            if (candidate > largest) {
                largest = candidate;
                pivot = i;
            }
        }
        if (largest == 0.0) {
            free(work); free(rhs); free(answer);
            return NUMERIX_STATUS_SINGULAR_MATRIX;
        }
        if (pivot != k) {
            for (j = k; j < n; ++j) {
                double swap = work[k * n + j];
                work[k * n + j] = work[pivot * n + j];
                work[pivot * n + j] = swap;
            }
            {
                double swap = rhs[k];
                rhs[k] = rhs[pivot];
                rhs[pivot] = swap;
            }
        }
        for (i = k + 1U; i < n; ++i) {
            double factor = work[i * n + k] / work[k * n + k];
            if (!isfinite(factor)) {
                free(work); free(rhs); free(answer);
                return NUMERIX_STATUS_NUMERICAL_FAILURE;
            }
            work[i * n + k] = 0.0;
            for (j = k + 1U; j < n; ++j) {
                work[i * n + j] -= factor * work[k * n + j];
                if (!isfinite(work[i * n + j])) {
                    free(work); free(rhs); free(answer);
                    return NUMERIX_STATUS_NUMERICAL_FAILURE;
                }
            }
            rhs[i] -= factor * rhs[k];
            if (!isfinite(rhs[i])) {
                free(work); free(rhs); free(answer);
                return NUMERIX_STATUS_NUMERICAL_FAILURE;
            }
        }
    }
    for (i = n; i-- > 0U;) {
        long double value = rhs[i];
        for (j = i + 1U; j < n; ++j) {
            value -= (long double)work[i * n + j] * (long double)answer[j];
        }
        if (work[i * n + i] == 0.0) {
            free(work); free(rhs); free(answer);
            return NUMERIX_STATUS_SINGULAR_MATRIX;
        }
        value /= work[i * n + i];
        if (!isfinite(value) || fabsl(value) > (long double)DBL_MAX) {
            free(work); free(rhs); free(answer);
            return NUMERIX_STATUS_NUMERICAL_FAILURE;
        }
        answer[i] = (double)value;
    }
    memcpy(solution, answer, n * sizeof(*solution));
    free(work); free(rhs); free(answer);
    return NUMERIX_STATUS_OK;
}

NUMERIX_Status numerix_lu_decomposition(const double *matrix, size_t n,
                                        double *lower, double *upper,
                                        size_t *permutation)
{
    size_t elements;
    size_t i;
    size_t j;
    size_t k;
    double *u;
    double *l;
    size_t *p;

    if (matrix == NULL || lower == NULL || upper == NULL || permutation == NULL ||
        !matrix_size_valid(n, &elements) || n > SIZE_MAX / sizeof(size_t)) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }
    u = (double *)malloc(elements * sizeof(*u));
    l = (double *)calloc(elements, sizeof(*l));
    p = (size_t *)malloc(n * sizeof(*p));
    if (u == NULL || l == NULL || p == NULL) {
        free(u); free(l); free(p);
        return NUMERIX_STATUS_ALLOCATION_FAILED;
    }
    memcpy(u, matrix, elements * sizeof(*u));
    for (i = 0U; i < elements; ++i) {
        if (!isfinite(u[i])) {
            free(u); free(l); free(p);
            return NUMERIX_STATUS_NONFINITE_RESULT;
        }
    }
    for (i = 0U; i < n; ++i) {
        p[i] = i;
        l[i * n + i] = 1.0;
    }
    for (k = 0U; k < n; ++k) {
        size_t pivot = k;
        double largest = fabs(u[k * n + k]);
        for (i = k + 1U; i < n; ++i) {
            double candidate = fabs(u[i * n + k]);
            if (candidate > largest) {
                largest = candidate;
                pivot = i;
            }
        }
        if (largest == 0.0) {
            free(u); free(l); free(p);
            return NUMERIX_STATUS_SINGULAR_MATRIX;
        }
        if (pivot != k) {
            for (j = 0U; j < n; ++j) {
                double swap = u[k * n + j];
                u[k * n + j] = u[pivot * n + j];
                u[pivot * n + j] = swap;
            }
            for (j = 0U; j < k; ++j) {
                double swap = l[k * n + j];
                l[k * n + j] = l[pivot * n + j];
                l[pivot * n + j] = swap;
            }
            {
                size_t swap = p[k];
                p[k] = p[pivot];
                p[pivot] = swap;
            }
        }
        for (i = k + 1U; i < n; ++i) {
            double factor = u[i * n + k] / u[k * n + k];
            if (!isfinite(factor)) {
                free(u); free(l); free(p);
                return NUMERIX_STATUS_NUMERICAL_FAILURE;
            }
            l[i * n + k] = factor;
            u[i * n + k] = 0.0;
            for (j = k + 1U; j < n; ++j) {
                u[i * n + j] -= factor * u[k * n + j];
                if (!isfinite(u[i * n + j])) {
                    free(u); free(l); free(p);
                    return NUMERIX_STATUS_NUMERICAL_FAILURE;
                }
            }
        }
    }
    memcpy(lower, l, elements * sizeof(*lower));
    memcpy(upper, u, elements * sizeof(*upper));
    memcpy(permutation, p, n * sizeof(*permutation));
    free(u); free(l); free(p);
    return NUMERIX_STATUS_OK;
}
