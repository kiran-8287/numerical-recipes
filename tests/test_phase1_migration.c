#include <math.h>
#include <stdio.h>
#include "numerix.h"

static double f_square(double x, void *user_data)
{
    (void)user_data;
    return x * x - 2.0;
}

static double f_linear(double x, void *user_data)
{
    (void)user_data;
    return x - 5.0;
}

static double df_square(double x, void *user_data)
{
    (void)user_data;
    return 2.0 * x;
}

int main(void)
{
    NUMERIX_Function root_f;
    NUMERIX_Function lin_f;
    NUMERIX_Function diff_f;
    double root_newton;
    double root_secant;
    double derivative_backward;
    double derivative_central;

    numerix_function_init(&root_f, f_square, df_square, NULL);
    numerix_function_init(&lin_f, f_linear, NULL, NULL);
    numerix_function_init(&diff_f, f_square, NULL, NULL);

    const double root = numerix_bisection(&root_f, 1.0, 2.0, 1e-8);
    const double linear_root = numerix_bisection(&lin_f, 0.0, 10.0, 1e-8);
    const double derivative = numerix_forward_difference(&diff_f, 3.0, 0.1);
    root_newton = numerix_newton_raphson(&root_f, 1.0, 1e-10);
    root_secant = numerix_secant(&root_f, 1.0, 2.0, 1e-8);
    derivative_backward = numerix_backward_difference(&diff_f, 3.0, 0.1);
    derivative_central = numerix_central_difference(&diff_f, 3.0, 0.1);

    if (!isfinite(root) || fabs(root - sqrt(2.0)) > 1e-6) {
        fprintf(stderr, "bisection migration failed: %.17g\n", root);
        return 1;
    }

    if (!isfinite(linear_root) || fabs(linear_root - 5.0) > 1e-8) {
        fprintf(stderr, "linear root migration failed: %.17g\n", linear_root);
        return 1;
    }

    if (!isfinite(derivative) || fabs(derivative - 6.1) > 1e-7) {
        fprintf(stderr, "forward difference migration failed: %.17g\n", derivative);
        return 1;
    }

    if (!isfinite(root_newton) || fabs(root_newton - sqrt(2.0)) > 1e-8 ||
        !isfinite(root_secant) || fabs(root_secant - sqrt(2.0)) > 1e-7) {
        fprintf(stderr, "Newton/secant migration failed: %.17g / %.17g\n",
                root_newton, root_secant);
        return 1;
    }

    if (!isfinite(derivative_backward) || fabs(derivative_backward - 5.9) > 1e-7 ||
        !isfinite(derivative_central) || fabs(derivative_central - 6.0) > 1e-7) {
        fprintf(stderr, "backward/central difference migration failed: %.17g / %.17g\n",
                derivative_backward, derivative_central);
        return 1;
    }

    if (isfinite(numerix_bisection(&root_f, 2.0, 3.0, 1e-8)) ||
        isfinite(numerix_central_difference(&diff_f, 3.0, 0.0))) {
        fprintf(stderr, "NUMERIX wrappers accepted invalid numerical inputs\n");
        return 1;
    }

    printf("Phase 1 migration API OK\n");
    return 0;
}
