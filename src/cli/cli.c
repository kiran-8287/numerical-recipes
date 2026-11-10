#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "numerix.h"

static double cli_square_callback(double x, void *context)
{
    (void)context;
    return x * x;
}

static double cli_linear_callback(double x, void *context)
{
    (void)context;
    return x + 1.0;
}

static double cli_ode_callback(double t, double y, void *context)
{
    (void)t;
    (void)context;
    return y;
}

NUMERIX_Status numerix_cli_config_init(NUMERIX_CLIConfig *config)
{
    if (config == NULL) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    memset(config, 0, sizeof(*config));
    config->operation = NUMERIX_CLI_OPERATION_ROOT_FINDING;
    config->show_reliability = 1;
    config->lower_limit = 0.0;
    config->upper_limit = 3.0;
    config->initial_guess = 1.5;
    config->tolerance = 1e-8;
    snprintf(config->expression, sizeof(config->expression), "x^2 - 4");
    snprintf(config->last_status, sizeof(config->last_status), "configured");
    return NUMERIX_STATUS_OK;
}

void numerix_cli_print_banner(void)
{
    printf("NUMERIX - Numerical Computation Library\n");
}

void numerix_cli_print_menu(void)
{
    printf("1. Root Finding\n");
    printf("2. Linear Algebra (sample 2x2 solve)\n");
    printf("3. Interpolation\n");
    printf("4. Integration (composite Simpson)\n");
    printf("5. Differentiation\n");
    printf("6. Optimization (golden-section minimization)\n");
    printf("7. ODE Solving (RK4 sample y'=y)\n");
    printf("8. Piecewise Functions\n");
    printf("9. Method Recommendation\n");
    printf("10. Sequential Root-Finding Portfolio\n");
    printf("11. Algorithm Comparison\n");
    printf("12. Accuracy Study\n");
    printf("13. Sensitivity Analysis\n");
    printf("14. Step-by-Step Execution (not connected to solvers)\n");
    printf("15. Exit\n");
}

NUMERIX_Status numerix_cli_parse_selection(const char *input, int *selection)
{
    char *end = NULL;
    long parsed_value;

    if (input == NULL || selection == NULL) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }
    parsed_value = strtol(input, &end, 10);
    if (end == input || *end != '\0' || parsed_value < 1 || parsed_value > 15) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }
    *selection = (int)parsed_value;
    return NUMERIX_STATUS_OK;
}

static void numerix_cli_set_status(NUMERIX_CLIConfig *config,
                                   NUMERIX_Status status,
                                   const char *message)
{
    snprintf(config->last_status, sizeof(config->last_status), "%s: %s",
             numerix_status_string(status), message != NULL ? message : "");
}

NUMERIX_Status numerix_cli_run_demo(NUMERIX_CLIConfig *config, int operation)
{
    NUMERIX_Function function;
    NUMERIX_Problem problem;
    NUMERIX_Status status;
    double value = NAN;
    int function_initialized = 0;

    if (config == NULL || operation < 1 || operation > 15) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }
    config->operation = operation;
    if (operation == NUMERIX_CLI_OPERATION_EXIT) {
        numerix_cli_set_status(config, NUMERIX_STATUS_OK, "exit selected");
        return NUMERIX_STATUS_OK;
    }
    if (!isfinite(config->tolerance) || config->tolerance <= 0.0) {
        numerix_cli_set_status(config, NUMERIX_STATUS_INVALID_ARGUMENT,
                               "tolerance must be finite and positive");
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    switch (operation) {
    case NUMERIX_CLI_OPERATION_ROOT_FINDING:
    case NUMERIX_CLI_OPERATION_LINEAR_ALGEBRA:
    case NUMERIX_CLI_OPERATION_INTERPOLATION:
    case NUMERIX_CLI_OPERATION_INTEGRATION:
    case NUMERIX_CLI_OPERATION_DIFFERENTIATION:
    case NUMERIX_CLI_OPERATION_OPTIMIZATION:
    case NUMERIX_CLI_OPERATION_ODE_SOLVING:
    case NUMERIX_CLI_OPERATION_PIECEWISE:
    case NUMERIX_CLI_OPERATION_METHOD_RECOMMENDATION:
    case NUMERIX_CLI_OPERATION_PARALLEL_SOLVER:
    case NUMERIX_CLI_OPERATION_ALGORITHM_COMPARISON:
    case NUMERIX_CLI_OPERATION_ACCURACY_ANALYSIS:
    case NUMERIX_CLI_OPERATION_SENSITIVITY_ANALYSIS:
        break;
    default:
        numerix_cli_set_status(config, NUMERIX_STATUS_NOT_SUPPORTED,
                               "this operation is not implemented by the current library");
        fprintf(stderr, "%s\n", config->last_status);
        return NUMERIX_STATUS_NOT_SUPPORTED;
    }

    memset(&function, 0, sizeof(function));
    function_initialized = 1;

    status = numerix_expression_parse(&function, config->expression);
    if (status != NUMERIX_STATUS_OK) {
        numerix_cli_set_status(config, status, "invalid expression");
        goto done;
    }
    status = numerix_problem_init(&problem, &function);
    if (status != NUMERIX_STATUS_OK) {
        numerix_cli_set_status(config, status, "problem initialization failed");
        goto done;
    }
    problem.options.absolute_tolerance = config->tolerance;
    problem.options.relative_tolerance = config->tolerance;
    problem.x0 = config->lower_limit;
    problem.x1 = config->upper_limit;
    problem.has_bracket = 1;

    switch (operation) {
    case NUMERIX_CLI_OPERATION_ROOT_FINDING:
        value = numerix_bisection(&function, config->lower_limit,
                                  config->upper_limit, config->tolerance);
        if (!isfinite(value)) {
            status = NUMERIX_STATUS_CONVERGENCE_FAILURE;
            numerix_cli_set_status(config, status, "bisection did not produce a root");
        } else {
            status = NUMERIX_STATUS_OK;
            snprintf(config->last_status, sizeof(config->last_status),
                     "root %.12g (bisection)", value);
            printf("Root: %.12g\n", value);
        }
        break;
    case NUMERIX_CLI_OPERATION_LINEAR_ALGEBRA: {
        const double matrix[] = {3.0, 2.0, 1.0, 2.0};
        const double rhs[] = {5.0, 5.0};
        double solution[2];
        status = numerix_gaussian_elimination(matrix, rhs, 2U, solution);
        if (status == NUMERIX_STATUS_OK) {
            snprintf(config->last_status, sizeof(config->last_status),
                     "2x2 solution [%.6g, %.6g]", solution[0], solution[1]);
            printf("2x2 solution: [%.6g, %.6g]\n", solution[0], solution[1]);
        } else {
            numerix_cli_set_status(config, status, "sample linear system failed");
        }
        break;
    }
    case NUMERIX_CLI_OPERATION_DIFFERENTIATION:
        value = numerix_central_difference(&function, config->initial_guess,
                                           sqrt(config->tolerance));
        if (!isfinite(value)) {
            status = NUMERIX_STATUS_NONFINITE_RESULT;
            numerix_cli_set_status(config, status, "derivative evaluation failed");
        } else {
            status = NUMERIX_STATUS_OK;
            snprintf(config->last_status, sizeof(config->last_status),
                     "derivative at %.12g is %.12g", config->initial_guess, value);
            printf("Derivative at %.12g: %.12g\n", config->initial_guess, value);
        }
        break;
    case NUMERIX_CLI_OPERATION_INTEGRATION:
        value = numerix_simpson_integral(&function, config->lower_limit,
                                         config->upper_limit, 100U);
        if (!isfinite(value)) {
            status = NUMERIX_STATUS_NONFINITE_RESULT;
            numerix_cli_set_status(config, status, "Simpson integration failed");
        } else {
            status = NUMERIX_STATUS_OK;
            snprintf(config->last_status, sizeof(config->last_status),
                     "Simpson integral %.12g", value);
            printf("Simpson integral: %.12g\n", value);
        }
        break;
    case NUMERIX_CLI_OPERATION_INTERPOLATION: {
        double x0 = config->lower_limit;
        double x1 = config->upper_limit;
        double y0 = numerix_function_evaluate(&function, x0, NULL);
        double y1 = numerix_function_evaluate(&function, x1, NULL);
        value = numerix_linear_interpolation(NULL, x0, y0, x1, y1,
                                             config->initial_guess);
        if (!isfinite(y0) || !isfinite(y1) || !isfinite(value)) {
            status = NUMERIX_STATUS_NONFINITE_RESULT;
            numerix_cli_set_status(config, status, "interpolation evaluation failed");
        } else {
            status = NUMERIX_STATUS_OK;
            snprintf(config->last_status, sizeof(config->last_status),
                     "linear interpolant at %.12g is %.12g",
                     config->initial_guess, value);
            printf("Linear interpolant at %.12g: %.12g\n", config->initial_guess, value);
        }
        break;
    }
    case NUMERIX_CLI_OPERATION_OPTIMIZATION:
        value = numerix_golden_section_minimize(&function, config->lower_limit,
                                                config->upper_limit,
                                                config->tolerance, 512U);
        if (!isfinite(value)) {
            status = NUMERIX_STATUS_CONVERGENCE_FAILURE;
            numerix_cli_set_status(config, status, "golden-section minimization failed");
        } else {
            status = NUMERIX_STATUS_OK;
            snprintf(config->last_status, sizeof(config->last_status),
                     "minimum location %.12g", value);
            printf("Minimum location: %.12g\n", value);
        }
        break;
    case NUMERIX_CLI_OPERATION_ODE_SOLVING: {
        double y_final;
        size_t steps;
        status = numerix_rk4_solve(cli_ode_callback, NULL, config->lower_limit,
                                   1.0, config->upper_limit, 0.1, 10000U,
                                   &y_final, &steps);
        if (status == NUMERIX_STATUS_OK) {
            snprintf(config->last_status, sizeof(config->last_status),
                     "RK4 y=%.12g after %lu steps", y_final, (unsigned long)steps);
            printf("RK4 sample y'=y: y(%.6g)=%.12g (%lu steps)\n",
                   config->upper_limit, y_final, (unsigned long)steps);
        } else {
            numerix_cli_set_status(config, status, "RK4 sample solve failed");
        }
        break;
    }
    case NUMERIX_CLI_OPERATION_PIECEWISE: {
        NUMERIX_Function left;
        NUMERIX_Function right;
        NUMERIX_Function bound;
        NUMERIX_PiecewiseFunction piecewise;
        double midpoint = config->lower_limit * 0.5 + config->upper_limit * 0.5;

        if (!(config->lower_limit < midpoint && midpoint < config->upper_limit)) {
            status = NUMERIX_STATUS_INVALID_ARGUMENT;
            numerix_cli_set_status(config, status, "piecewise interval is too small");
            break;
        }
        status = numerix_function_init(&left, cli_square_callback, NULL, NULL);
        if (status != NUMERIX_STATUS_OK) break;
        status = numerix_function_init(&right, cli_linear_callback, NULL, NULL);
        if (status != NUMERIX_STATUS_OK) break;
        status = numerix_piecewise_init(&piecewise);
        if (status != NUMERIX_STATUS_OK) break;
        status = numerix_piecewise_add_segment(&piecewise, config->lower_limit,
                                                midpoint, 1, 0, &left);
        if (status == NUMERIX_STATUS_OK) {
            status = numerix_piecewise_add_segment(&piecewise, midpoint,
                                                    config->upper_limit, 1, 1, &right);
        }
        if (status == NUMERIX_STATUS_OK) {
            status = numerix_piecewise_bind(&bound, &piecewise);
        }
        if (status == NUMERIX_STATUS_OK) {
            value = numerix_function_evaluate(&bound, config->initial_guess, NULL);
            if (isfinite(value)) {
                snprintf(config->last_status, sizeof(config->last_status),
                         "piecewise value %.12g at %.12g", value,
                         config->initial_guess);
                printf("Piecewise value at %.12g: %.12g\n",
                       config->initial_guess, value);
            } else {
                status = NUMERIX_STATUS_NONFINITE_RESULT;
                numerix_cli_set_status(config, status, "point is outside the piecewise domain");
            }
        }
        numerix_piecewise_destroy(&piecewise);
        if (status != NUMERIX_STATUS_OK) {
            numerix_cli_set_status(config, status, "piecewise evaluation failed");
        }
        break;
    }
    case NUMERIX_CLI_OPERATION_METHOD_RECOMMENDATION: {
        NUMERIX_MethodRecommendation recommendation;
        status = numerix_recommend_method(&problem, &recommendation);
        if (status == NUMERIX_STATUS_OK) {
            snprintf(config->last_status, sizeof(config->last_status),
                     "recommended method %d: %.120s",
                     recommendation.recommended_method, recommendation.reason);
            printf("Recommended method: %d\nReason: %s\n",
                   recommendation.recommended_method, recommendation.reason);
        } else {
            numerix_cli_set_status(config, status, "recommendation failed");
        }
        break;
    }
    case NUMERIX_CLI_OPERATION_PARALLEL_SOLVER: {
        NUMERIX_PortfolioResult portfolio;
        status = numerix_portfolio_solve(&problem, &portfolio);
        if (status == NUMERIX_STATUS_OK) {
            snprintf(config->last_status, sizeof(config->last_status),
                     "sequential portfolio root %.12g", portfolio.winner_value);
            printf("Sequential portfolio root: %.12g (method %d)\n",
                   portfolio.winner_value, portfolio.winner);
        } else {
            numerix_cli_set_status(config, status, "portfolio found no verified root");
        }
        break;
    }
    case NUMERIX_CLI_OPERATION_ALGORITHM_COMPARISON: {
        const NUMERIX_Method methods[] = {
            NUMERIX_METHOD_BISECTION, NUMERIX_METHOD_SECANT
        };
        NUMERIX_ComparisonResult comparison;
        status = numerix_compare_methods(&problem, methods,
                                        sizeof(methods) / sizeof(methods[0]),
                                        &comparison);
        if (status == NUMERIX_STATUS_OK) {
            snprintf(config->last_status, sizeof(config->last_status),
                     "comparison winner %d at %.12g",
                     comparison.winner_method, comparison.winner_value);
            printf("Comparison winner: method %d, root %.12g\n",
                   comparison.winner_method, comparison.winner_value);
        } else {
            numerix_cli_set_status(config, status, "no method converged");
        }
        break;
    }
    case NUMERIX_CLI_OPERATION_ACCURACY_ANALYSIS: {
        const NUMERIX_Method methods[] = {
            NUMERIX_METHOD_BISECTION, NUMERIX_METHOD_SECANT
        };
        const double tolerances[] = {1e-4, 1e-6, 1e-8};
        NUMERIX_AccuracyStudy study;
        status = numerix_accuracy_study(&problem, methods,
                                        sizeof(methods) / sizeof(methods[0]),
                                        tolerances,
                                        sizeof(tolerances) / sizeof(tolerances[0]),
                                        &study);
        if (status == NUMERIX_STATUS_OK) {
            snprintf(config->last_status, sizeof(config->last_status),
                     "accuracy study: %lu samples; best residual %.3e",
                     (unsigned long)study.count, study.best_residual);
            printf("Accuracy samples: %lu; best residual: %.3e\n",
                   (unsigned long)study.count, study.best_residual);
        } else {
            numerix_cli_set_status(config, status, "accuracy study failed");
        }
        break;
    }
    case NUMERIX_CLI_OPERATION_SENSITIVITY_ANALYSIS: {
        NUMERIX_SensitivityReport report;
        problem.x0 = config->initial_guess;
        status = numerix_sensitivity_experiment(&problem, config->tolerance, &report);
        if (status == NUMERIX_STATUS_OK && isfinite(report.absolute_sensitivity)) {
            snprintf(config->last_status, sizeof(config->last_status),
                     "absolute sensitivity %.3e", report.absolute_sensitivity);
            printf("Absolute sensitivity: %.6e\nRelative sensitivity: %.6e\n",
                   report.absolute_sensitivity, report.relative_sensitivity);
        } else {
            if (status == NUMERIX_STATUS_OK) {
                status = NUMERIX_STATUS_NONFINITE_RESULT;
            }
            numerix_cli_set_status(config, status, "sensitivity evaluation failed");
        }
        break;
    }
    default:
        status = NUMERIX_STATUS_NOT_SUPPORTED;
        numerix_cli_set_status(config, status, "operation is not implemented");
        break;
    }

done:
    if (function_initialized) {
        numerix_function_destroy(&function);
    }
    if (status != NUMERIX_STATUS_OK) {
        fprintf(stderr, "%s\n", config->last_status);
    }
    return status;
}
