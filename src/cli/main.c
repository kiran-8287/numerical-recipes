#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "numerix.h"

static void print_usage(const char *program)
{
    printf("Usage: %s [COMMAND] [OPTIONS]\n", program);
    puts("Commands: root, linear, differentiate, interpolate, integrate, minimize,");
    puts("          ode, piecewise, recommend, portfolio, compare, accuracy, sensitivity");
    puts("Options:  --expr EXPR  --a VALUE  --b VALUE  --x VALUE  --tol VALUE");
    puts("          --help  --version");
    puts("With no command, the interactive menu is shown.");
}

static int parse_double_arg(const char *text, double *value)
{
    char *end = NULL;
    double parsed;
    if (text == NULL || value == NULL) {
        return 0;
    }
    parsed = strtod(text, &end);
    if (end == text || *end != '\0' || !isfinite(parsed)) {
        return 0;
    }
    *value = parsed;
    return 1;
}

static int command_to_operation(const char *command)
{
    if (strcmp(command, "root") == 0) return NUMERIX_CLI_OPERATION_ROOT_FINDING;
    if (strcmp(command, "differentiate") == 0) return NUMERIX_CLI_OPERATION_DIFFERENTIATION;
    if (strcmp(command, "interpolate") == 0) return NUMERIX_CLI_OPERATION_INTERPOLATION;
    if (strcmp(command, "recommend") == 0) return NUMERIX_CLI_OPERATION_METHOD_RECOMMENDATION;
    if (strcmp(command, "portfolio") == 0) return NUMERIX_CLI_OPERATION_PARALLEL_SOLVER;
    if (strcmp(command, "compare") == 0) return NUMERIX_CLI_OPERATION_ALGORITHM_COMPARISON;
    if (strcmp(command, "accuracy") == 0) return NUMERIX_CLI_OPERATION_ACCURACY_ANALYSIS;
    if (strcmp(command, "sensitivity") == 0) return NUMERIX_CLI_OPERATION_SENSITIVITY_ANALYSIS;
    if (strcmp(command, "integrate") == 0) return NUMERIX_CLI_OPERATION_INTEGRATION;
    if (strcmp(command, "minimize") == 0) return NUMERIX_CLI_OPERATION_OPTIMIZATION;
    if (strcmp(command, "linear") == 0) return NUMERIX_CLI_OPERATION_LINEAR_ALGEBRA;
    if (strcmp(command, "ode") == 0) return NUMERIX_CLI_OPERATION_ODE_SOLVING;
    if (strcmp(command, "piecewise") == 0) return NUMERIX_CLI_OPERATION_PIECEWISE;
    return 0;
}

int main(int argc, char **argv)
{
    NUMERIX_CLIConfig config;
    int operation = 0;
    int i;
    NUMERIX_Status status;

    if (argc > 1 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)) {
        print_usage(argv[0]);
        return 0;
    }
    if (argc > 1 && strcmp(argv[1], "--version") == 0) {
        puts("NUMERIX 0.1.0");
        return 0;
    }

    if (numerix_cli_config_init(&config) != NUMERIX_STATUS_OK) {
        fputs("CLI configuration failed\n", stderr);
        return 1;
    }

    if (argc == 1) {
        char buffer[32];
        numerix_cli_print_banner();
        numerix_cli_print_menu();
        printf("Select operation: ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fputs("No input received\n", stderr);
            return 2;
        }
        buffer[strcspn(buffer, "\r\n")] = '\0';
        if (numerix_cli_parse_selection(buffer, &operation) != NUMERIX_STATUS_OK) {
            fputs("Invalid selection. Choose an operation from 1 to 15.\n", stderr);
            return 3;
        }
    } else {
        operation = command_to_operation(argv[1]);
        if (operation == 0) {
            fprintf(stderr, "Unknown command: %s\n", argv[1]);
            print_usage(argv[0]);
            return 2;
        }
        for (i = 2; i < argc; ++i) {
            const char *option = argv[i];
            const char *argument = NULL;
            double parsed;

            if (i + 1 >= argc) {
                fprintf(stderr, "Missing value for %s\n", option);
                return 2;
            }
            argument = argv[++i];
            if (strcmp(option, "--expr") == 0) {
                if (strlen(argument) >= sizeof(config.expression)) {
                    fputs("Expression is too long\n", stderr);
                    return 2;
                }
                snprintf(config.expression, sizeof(config.expression), "%s", argument);
            } else if (parse_double_arg(argument, &parsed)) {
                if (strcmp(option, "--a") == 0) config.lower_limit = parsed;
                else if (strcmp(option, "--b") == 0) config.upper_limit = parsed;
                else if (strcmp(option, "--x") == 0) config.initial_guess = parsed;
                else if (strcmp(option, "--tol") == 0) {
                    if (parsed <= 0.0) {
                        fputs("Tolerance must be positive\n", stderr);
                        return 2;
                    }
                    config.tolerance = parsed;
                }
                else {
                    fprintf(stderr, "Unknown option: %s\n", option);
                    return 2;
                }
            } else {
                fprintf(stderr, "Invalid numeric value for %s: %s\n", option, argument);
                return 2;
            }
        }
    }

    numerix_cli_print_banner();
    status = numerix_cli_run_demo(&config, operation);
    printf("Status: %s\n", config.last_status);
    return status == NUMERIX_STATUS_OK ? 0 : 1;
}
