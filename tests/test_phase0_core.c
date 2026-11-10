#include <stdio.h>
#include <math.h>
#include "numerix.h"

static double sample_fn(double x, void *user_data)
{
    (void)user_data;
    return x * x - 2.0;
}

int main(void)
{
    NUMERIX_Function function;
    NUMERIX_Problem problem;
    NUMERIX_Result result;
    NUMERIX_Options options;
    NUMERIX_ExecutionContext context;

    numerix_function_init(&function, sample_fn, NULL, NULL);
    numerix_problem_init(&problem, &function);
    numerix_options_init(&options);
    numerix_execution_context_init(&context);
    numerix_result_init(&result);

    if (function.evaluate == NULL) {
        fprintf(stderr, "Function evaluator missing\n");
        return 1;
    }

    if (problem.function.evaluate == NULL) {
        fprintf(stderr, "Problem function missing\n");
        return 1;
    }

    if (context.max_iterations <= 0) {
        fprintf(stderr, "Execution context invalid\n");
        return 1;
    }

    if (numerix_status_string(NUMERIX_STATUS_OK) == NULL) {
        fprintf(stderr, "Status string missing\n");
        return 1;
    }

    printf("Phase 0 core API OK\n");
    return 0;
}
