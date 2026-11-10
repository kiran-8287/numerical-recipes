#include <stdio.h>
#include <string.h>
#include "numerix.h"

int main(void)
{
    NUMERIX_CLIConfig config;
    int selection = 0;

    if (numerix_cli_config_init(&config) != NUMERIX_STATUS_OK) {
        fprintf(stderr, "cli config init failed\n");
        return 1;
    }

    if (numerix_cli_parse_selection("10", &selection) != NUMERIX_STATUS_OK) {
        fprintf(stderr, "cli parse selection failed\n");
        return 2;
    }

    if (selection != 10) {
        fprintf(stderr, "selection mismatch\n");
        return 3;
    }

    config.lower_limit = 0.0;
    config.upper_limit = 3.0;
    config.tolerance = 1e-9;
    if (numerix_cli_run_demo(&config, NUMERIX_CLI_OPERATION_ROOT_FINDING) !=
        NUMERIX_STATUS_OK) {
        fprintf(stderr, "CLI root operation failed: %s\n", config.last_status);
        return 4;
    }
    if (strstr(config.last_status, "root") == NULL) {
        fprintf(stderr, "CLI root operation did not report its result\n");
        return 5;
    }
    if (numerix_cli_run_demo(&config, NUMERIX_CLI_OPERATION_INTEGRATION) !=
        NUMERIX_STATUS_OK || strstr(config.last_status, "integral") == NULL) {
        fprintf(stderr, "CLI integration failed: %s\n", config.last_status);
        return 6;
    }
    if (numerix_cli_run_demo(&config, NUMERIX_CLI_OPERATION_STEP_EXECUTION) !=
        NUMERIX_STATUS_NOT_SUPPORTED) {
        fprintf(stderr, "CLI reported disconnected step tracing as supported\n");
        return 7;
    }

    printf("Phase 17 CLI execution and capability reporting OK\n");
    return 0;
}
