#include <math.h>
#include <stdio.h>
#include "numerix.h"

static double quadratic(double x, void *user_data)
{
    (void)user_data;
    return x * x - 4.0;
}

int main(void)
{
    NUMERIX_Function function;
    NUMERIX_Problem problem;
    NUMERIX_PortfolioResult portfolio;

    numerix_function_init(&function, quadratic, NULL, NULL);
    numerix_problem_init(&problem, &function);
    problem.x0 = 0.0;
    problem.x1 = 3.0;
    problem.has_bracket = 1;
    problem.has_initial_guess = 1;

    if (numerix_portfolio_solve(&problem, &portfolio) != NUMERIX_STATUS_OK) {
        fprintf(stderr, "portfolio solve failed\n");
        return 1;
    }

    if (!portfolio.has_winner || portfolio.count == 0U) {
        fprintf(stderr, "portfolio produced no winner\n");
        return 1;
    }

    if (fabs(portfolio.winner_value - 2.0) > 1e-6) {
        fprintf(stderr, "wrong winner value: %.12f\n", portfolio.winner_value);
        return 1;
    }

    printf("Phase 6 portfolio solver OK\n");
    return 0;
}
