#ifndef NUMERIX_H
#define NUMERIX_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    NUMERIX_STATUS_OK = 0,
    NUMERIX_STATUS_INVALID_ARGUMENT = 1,
    NUMERIX_STATUS_ALLOCATION_FAILED = 2,
    NUMERIX_STATUS_CONVERGENCE_FAILURE = 3,
    NUMERIX_STATUS_NONFINITE_RESULT = 4,
    NUMERIX_STATUS_CANCELLED = 5,
    NUMERIX_STATUS_NOT_SUPPORTED = 6,
    NUMERIX_STATUS_UNAVAILABLE = 7,
    NUMERIX_STATUS_WARNING = 8,
    NUMERIX_STATUS_ERROR = 9,
    NUMERIX_STATUS_SINGULAR_MATRIX = 10,
    NUMERIX_STATUS_NUMERICAL_FAILURE = 11,
    NUMERIX_STATUS_RESOURCE_LIMIT = 12
} NUMERIX_Status;

typedef enum {
    NUMERIX_METHOD_NONE = 0,
    NUMERIX_METHOD_BISECTION = 1,
    NUMERIX_METHOD_NEWTON_RAPHSON = 2,
    NUMERIX_METHOD_SECANT = 3,
    NUMERIX_METHOD_GAUSSIAN_ELIMINATION = 4,
    NUMERIX_METHOD_LU_DECOMPOSITION = 5,
    NUMERIX_METHOD_GOLDEN_SECTION = 6,
    NUMERIX_METHOD_RK4 = 7,
    NUMERIX_METHOD_TRAPEZOIDAL = 8,
    NUMERIX_METHOD_SIMPSON = 9,
    NUMERIX_METHOD_LAGRANGE = 10,
    NUMERIX_METHOD_NEWTON_INTERPOLATION = 11
} NUMERIX_Method;

typedef enum {
    NUMERIX_FUNCTION_NONE = 0,
    NUMERIX_FUNCTION_CALLBACK = 1,
    NUMERIX_FUNCTION_EXPRESSION = 2,
    NUMERIX_FUNCTION_PIECEWISE = 3
} NUMERIX_FunctionType;

typedef enum {
    NUMERIX_DIAGNOSTIC_NONE = 0,
    NUMERIX_DIAGNOSTIC_BASIC = 1,
    NUMERIX_DIAGNOSTIC_VERBOSE = 2,
    NUMERIX_DIAGNOSTIC_TRACE = 3
} NUMERIX_DiagnosticLevel;

typedef double (*NUMERIX_EvalCallback)(double x, void *user_data);
typedef double (*NUMERIX_DerivativeCallback)(double x, void *user_data);
typedef double (*NUMERIX_ODECallback)(double t, double y, void *user_data);

typedef struct {
    NUMERIX_FunctionType type;
    NUMERIX_EvalCallback evaluate;
    NUMERIX_DerivativeCallback derivative;
    void *user_data;
    int domain_known;
    double domain_min;
    double domain_max;
    int continuous;
    int differentiable;
    int owns_resources;
} NUMERIX_Function;

typedef struct {
    double absolute_tolerance;
    double relative_tolerance;
    size_t max_iterations;
    double max_execution_time_seconds;
    double derivative_step_size;
    NUMERIX_DiagnosticLevel diagnostic_level;
    int step_by_step;
    int enable_fallback;
    int enable_parallel_execution;
} NUMERIX_Options;

typedef struct {
    NUMERIX_Options options;
    int max_iterations;
    int cancelled;
    int step_by_step;
    double last_check_time_seconds;
} NUMERIX_ExecutionContext;

typedef struct {
    double value;
    double residual;
    double estimated_error;
    size_t iterations;
    size_t function_evaluations;
    double execution_time_seconds;
    int converged;
    NUMERIX_Method method;
    NUMERIX_Status status;
    char warning[128];
    char diagnostic[256];
} NUMERIX_Result;

typedef struct {
    NUMERIX_Function function;
    double x0;
    double x1;
    double target;
    double reference_solution;
    int has_reference_solution;
    int has_bracket;
    int has_initial_guess;
    NUMERIX_Method preferred_method;
    NUMERIX_Options options;
} NUMERIX_Problem;

typedef struct {
    NUMERIX_Method recommended_method;
    int confidence;
    char reason[256];
    char advantages[256];
    char limitations[256];
    char alternatives[256];
} NUMERIX_MethodRecommendation;

typedef struct {
    NUMERIX_Method attempted_method;
    NUMERIX_Method fallback_method;
    char reason[192];
} NUMERIX_FallbackRecord;

typedef struct {
    NUMERIX_FallbackRecord records[8];
    size_t count;
} NUMERIX_FallbackHistory;

typedef struct {
    NUMERIX_Method method;
    double value;
    double residual;
    int verified;
    double execution_time_seconds;
    char diagnostic[128];
} NUMERIX_PortfolioCandidate;

typedef struct {
    NUMERIX_PortfolioCandidate candidates[8];
    size_t count;
    NUMERIX_Method winner;
    double winner_value;
    double winner_residual;
    int has_winner;
    char summary[256];
} NUMERIX_PortfolioResult;

typedef struct {
    NUMERIX_Method method;
    double value;
    double residual;
    double estimated_error;
    size_t iterations;
    size_t function_evaluations;
    double execution_time_seconds;
    int converged;
    NUMERIX_Status status;
    char summary[128];
} NUMERIX_ComparisonEntry;

typedef struct {
    NUMERIX_ComparisonEntry entries[8];
    size_t count;
    NUMERIX_Method winner_method;
    double winner_value;
    double winner_residual;
    int has_winner;
    char summary[256];
} NUMERIX_ComparisonResult;

typedef struct {
    double tolerance;
    NUMERIX_Method method;
    double actual_error;
    double residual;
    double execution_time_seconds;
    size_t iterations;
    size_t function_evaluations;
    int converged;
    char note[128];
} NUMERIX_AccuracySample;

typedef struct {
    NUMERIX_AccuracySample samples[16];
    size_t count;
    double best_error;
    double best_tolerance;
    double best_residual;
    char summary[256];
} NUMERIX_AccuracyStudy;

typedef enum {
    NUMERIX_LIBRARY_INTERNAL = 0,
    NUMERIX_LIBRARY_GSL = 1,
    NUMERIX_LIBRARY_BLAS = 2,
    NUMERIX_LIBRARY_LAPACK = 3,
    NUMERIX_LIBRARY_NUMERICAL_RECIPES = 4
} NUMERIX_ExternalLibrary;

typedef struct {
    NUMERIX_ExternalLibrary library;
    NUMERIX_Method method;
    double accuracy;
    double residual;
    double runtime_seconds;
    int available;
    char note[128];
} NUMERIX_ReferenceResult;

typedef struct {
    NUMERIX_ReferenceResult entries[8];
    size_t count;
    double best_accuracy;
    int has_reference;
    char summary[256];
} NUMERIX_ExternalComparison;

typedef enum {
    NUMERIX_ERROR_NONE = 0,
    NUMERIX_ERROR_INVALID_INPUT = 1,
    NUMERIX_ERROR_INVALID_INTERVAL = 2,
    NUMERIX_ERROR_DIVISION_BY_ZERO = 3,
    NUMERIX_ERROR_DOMAIN_VIOLATION = 4,
    NUMERIX_ERROR_SINGULAR_MATRIX = 5,
    NUMERIX_ERROR_NEARLY_SINGULAR_MATRIX = 6,
    NUMERIX_ERROR_OVERFLOW = 7,
    NUMERIX_ERROR_UNDERFLOW = 8,
    NUMERIX_ERROR_NAN_RESULT = 9,
    NUMERIX_ERROR_INFINITY_RESULT = 10,
    NUMERIX_ERROR_DIVERGENCE = 11,
    NUMERIX_ERROR_NONCONVERGENCE = 12,
    NUMERIX_ERROR_STAGNATION = 13,
    NUMERIX_ERROR_OSCILLATION = 14,
    NUMERIX_ERROR_ILL_CONDITIONING = 15,
    NUMERIX_ERROR_NUMERICAL_INSTABILITY = 16,
    NUMERIX_ERROR_RESOURCE_LIMIT_REACHED = 17,
    NUMERIX_ERROR_CANCELLATION = 18,
    NUMERIX_ERROR_DERIVATIVE_ZERO = 19
} NUMERIX_ErrorCode;

typedef struct {
    NUMERIX_ErrorCode code;
    NUMERIX_Method method;
    char what[192];
    char where[128];
    char why[256];
    char action[256];
    char alternative[256];
} NUMERIX_ErrorReport;

typedef enum {
    NUMERIX_CLI_OPERATION_ROOT_FINDING = 1,
    NUMERIX_CLI_OPERATION_LINEAR_ALGEBRA = 2,
    NUMERIX_CLI_OPERATION_INTERPOLATION = 3,
    NUMERIX_CLI_OPERATION_INTEGRATION = 4,
    NUMERIX_CLI_OPERATION_DIFFERENTIATION = 5,
    NUMERIX_CLI_OPERATION_OPTIMIZATION = 6,
    NUMERIX_CLI_OPERATION_ODE_SOLVING = 7,
    NUMERIX_CLI_OPERATION_PIECEWISE = 8,
    NUMERIX_CLI_OPERATION_METHOD_RECOMMENDATION = 9,
    NUMERIX_CLI_OPERATION_PARALLEL_SOLVER = 10,
    NUMERIX_CLI_OPERATION_ALGORITHM_COMPARISON = 11,
    NUMERIX_CLI_OPERATION_ACCURACY_ANALYSIS = 12,
    NUMERIX_CLI_OPERATION_SENSITIVITY_ANALYSIS = 13,
    NUMERIX_CLI_OPERATION_STEP_EXECUTION = 14,
    NUMERIX_CLI_OPERATION_EXIT = 15
} NUMERIX_CLI_Operation;

typedef struct {
    int operation;
    int step_by_step;
    int compare_methods;
    int show_reliability;
    int verbose;
    char expression[128];
    double lower_limit;
    double upper_limit;
    double initial_guess;
    double tolerance;
    char last_status[128];
} NUMERIX_CLIConfig;

typedef enum {
    NUMERIX_TEST_UNIT = 0,
    NUMERIX_TEST_ANALYTICAL = 1,
    NUMERIX_TEST_EDGE_CASE = 2,
    NUMERIX_TEST_METAMORPHIC = 3,
    NUMERIX_TEST_FAILURE_INJECTION = 4,
    NUMERIX_TEST_CONCURRENCY = 5,
    NUMERIX_TEST_NUMERICAL_STABILITY = 6
} NUMERIX_TestCategory;

typedef enum {
    NUMERIX_TEST_PASS = 0,
    NUMERIX_TEST_FAIL = 1,
    NUMERIX_TEST_SKIP = 2
} NUMERIX_TestResult;

typedef struct {
    const char *name;
    NUMERIX_TestCategory category;
    int passed;
    NUMERIX_TestResult result;
    double duration_seconds;
    char message[128];
} NUMERIX_TestCase;

typedef struct {
    const char *name;
    NUMERIX_TestCase *cases;
    size_t count;
    int has_failures;
} NUMERIX_TestSuite;

typedef struct {
    size_t total_tests;
    size_t passed_tests;
    size_t failed_tests;
    size_t skipped_tests;
    double total_duration_seconds;
    char summary[256];
} NUMERIX_ValidationSummary;

typedef enum {
    NUMERIX_CONVERGENCE_UNKNOWN = 0,
    NUMERIX_CONVERGENCE_PROGRESS = 1,
    NUMERIX_CONVERGENCE_STAGNANT = 2,
    NUMERIX_CONVERGENCE_DIVERGING = 3,
    NUMERIX_CONVERGENCE_OSCILLATORY = 4,
    NUMERIX_CONVERGENCE_REACHED = 5
} NUMERIX_ConvergenceState;

typedef struct {
    size_t iteration;
    double x;
    double value;
    double derivative;
    double next_x;
    char message[128];
} NUMERIX_StepRecord;

typedef struct {
    NUMERIX_StepRecord records[64];
    size_t count;
    int enabled;
} NUMERIX_StepExecutionLog;

typedef struct {
    double absolute_sensitivity;
    double relative_sensitivity;
    double perturbation;
    char summary[128];
} NUMERIX_SensitivityReport;

typedef struct {
    double condition_estimate;
    double relative_condition;
    int ill_conditioned;
    char summary[128];
} NUMERIX_ConditionReport;

typedef struct {
    size_t max_iterations;
    size_t max_function_evaluations;
    double max_execution_time_seconds;
    int cancellation_requested;
    char reason[128];
} NUMERIX_ExecutionLimits;

typedef struct {
    NUMERIX_ConvergenceState state;
    double residual;
    double previous_residual;
    double current_value;
    double previous_value;
    size_t iterations;
    char summary[128];
} NUMERIX_ConvergenceMonitor;

typedef struct {
    NUMERIX_Method method;
    NUMERIX_Status status;
    double value;
    double residual;
    double estimated_error;
    size_t iterations;
    size_t function_evaluations;
    double execution_time_seconds;
    char summary[256];
    char warnings[256];
} NUMERIX_ReliabilityReport;

typedef enum {
    NUMERIX_DISCONTINUITY_NONE = 0,
    NUMERIX_DISCONTINUITY_JUMP = 1,
    NUMERIX_DISCONTINUITY_REMOVABLE = 2,
    NUMERIX_DISCONTINUITY_INFINITE = 3,
    NUMERIX_DISCONTINUITY_UNDEFINED = 4
} NUMERIX_DiscontinuityType;

typedef struct {
    double lower_bound;
    double upper_bound;
    int include_lower;
    int include_upper;
    NUMERIX_Function function;
} NUMERIX_PiecewiseSegment;

typedef struct {
    NUMERIX_PiecewiseSegment *segments;
    size_t count;
    size_t capacity;
    double *breakpoints;
    size_t breakpoint_count;
    int has_gaps;
    int has_overlaps;
} NUMERIX_PiecewiseFunction;

typedef struct {
    double point;
    NUMERIX_DiscontinuityType type;
    char description[64];
} NUMERIX_Discontinuity;

NUMERIX_Status numerix_function_init(NUMERIX_Function *function,
                                     NUMERIX_EvalCallback evaluate,
                                     NUMERIX_DerivativeCallback derivative,
                                     void *user_data);

NUMERIX_Status numerix_problem_init(NUMERIX_Problem *problem,
                                   const NUMERIX_Function *function);

NUMERIX_Status numerix_recommendation_init(NUMERIX_MethodRecommendation *recommendation);
NUMERIX_Status numerix_recommend_method(const NUMERIX_Problem *problem,
                                       NUMERIX_MethodRecommendation *recommendation);
NUMERIX_Status numerix_problem_summary(const NUMERIX_Problem *problem,
                                      char *summary,
                                      size_t summary_size);
NUMERIX_Status numerix_method_fallback(const NUMERIX_Problem *problem,
                                     NUMERIX_Method current_method,
                                     NUMERIX_MethodRecommendation *fallback,
                                     NUMERIX_FallbackHistory *history);
NUMERIX_Status numerix_fallback_history_init(NUMERIX_FallbackHistory *history);
NUMERIX_Status numerix_portfolio_solve(const NUMERIX_Problem *problem,
                                      NUMERIX_PortfolioResult *result);
NUMERIX_Status numerix_comparison_result_init(NUMERIX_ComparisonResult *result);
NUMERIX_Status numerix_compare_methods(const NUMERIX_Problem *problem,
                                      const NUMERIX_Method *methods,
                                      size_t method_count,
                                      NUMERIX_ComparisonResult *result);
NUMERIX_Status numerix_accuracy_study(const NUMERIX_Problem *problem,
                                     const NUMERIX_Method *methods,
                                     size_t method_count,
                                     const double *tolerances,
                                     size_t tolerance_count,
                                     NUMERIX_AccuracyStudy *study);
NUMERIX_Status numerix_compare_standard_libraries(const NUMERIX_Problem *problem,
                                                const NUMERIX_Method *methods,
                                                size_t method_count,
                                                NUMERIX_ExternalComparison *comparison);
NUMERIX_Status numerix_error_report_init(NUMERIX_ErrorReport *report);
NUMERIX_Status numerix_error_detect(NUMERIX_Method method,
                                   NUMERIX_ErrorCode code,
                                   const char *context,
                                   const char *details,
                                   NUMERIX_ErrorReport *report);
const char *numerix_error_code_string(NUMERIX_ErrorCode code);

NUMERIX_Status numerix_reliability_report_init(NUMERIX_ReliabilityReport *report);
NUMERIX_Status numerix_reliability_report_from_result(const NUMERIX_Result *result,
                                                    const NUMERIX_Problem *problem,
                                                    NUMERIX_ReliabilityReport *report);
NUMERIX_Status numerix_cli_config_init(NUMERIX_CLIConfig *config);
void numerix_cli_print_banner(void);
void numerix_cli_print_menu(void);
NUMERIX_Status numerix_cli_parse_selection(const char *input, int *selection);
NUMERIX_Status numerix_cli_run_demo(NUMERIX_CLIConfig *config, int operation);
NUMERIX_Status numerix_test_case_init(NUMERIX_TestCase *test_case);
NUMERIX_Status numerix_test_suite_init(NUMERIX_TestSuite *suite);
NUMERIX_Status numerix_validation_run_suite(const NUMERIX_TestSuite *suite,
                                          NUMERIX_ValidationSummary *summary);

NUMERIX_Status numerix_step_execution_log_init(NUMERIX_StepExecutionLog *log);
NUMERIX_Status numerix_step_execution_log_append(NUMERIX_StepExecutionLog *log,
                                                const NUMERIX_StepRecord *record);

NUMERIX_Status numerix_sensitivity_experiment(const NUMERIX_Problem *problem,
                                             double perturbation,
                                             NUMERIX_SensitivityReport *report);
NUMERIX_Status numerix_condition_analysis(const NUMERIX_Problem *problem,
                                        NUMERIX_ConditionReport *report);

NUMERIX_Status numerix_execution_limits_init(NUMERIX_ExecutionLimits *limits);
int numerix_execution_limits_reached(const NUMERIX_ExecutionLimits *limits,
                                     size_t iterations,
                                     size_t function_evaluations,
                                     double elapsed_seconds);

NUMERIX_Status numerix_convergence_monitor_init(NUMERIX_ConvergenceMonitor *monitor);
NUMERIX_Status numerix_convergence_monitor_update(NUMERIX_ConvergenceMonitor *monitor,
                                                double residual,
                                                double previous_residual,
                                                double current_value,
                                                double previous_value);

NUMERIX_Status numerix_options_init(NUMERIX_Options *options);

NUMERIX_Status numerix_execution_context_init(NUMERIX_ExecutionContext *context);

NUMERIX_Status numerix_result_init(NUMERIX_Result *result);

const char *numerix_status_string(NUMERIX_Status status);

void *numerix_malloc(size_t bytes);
void numerix_free(void *ptr);
void numerix_function_destroy(NUMERIX_Function *function);

double numerix_function_evaluate(const NUMERIX_Function *function,
                                 double x,
                                 void *user_data);

NUMERIX_Status numerix_expression_parse(NUMERIX_Function *function,
                                       const char *expression);

NUMERIX_Status numerix_piecewise_init(NUMERIX_PiecewiseFunction *piecewise);
NUMERIX_Status numerix_piecewise_add_segment(NUMERIX_PiecewiseFunction *piecewise,
                                            double lower_bound,
                                            double upper_bound,
                                            int include_lower,
                                            int include_upper,
                                            const NUMERIX_Function *function);
NUMERIX_Status numerix_piecewise_bind(NUMERIX_Function *function,
                                     NUMERIX_PiecewiseFunction *piecewise);
NUMERIX_Status numerix_piecewise_validate(const NUMERIX_PiecewiseFunction *piecewise);
NUMERIX_Status numerix_piecewise_collect_breakpoints(const NUMERIX_PiecewiseFunction *piecewise,
                                                    double **breakpoints,
                                                    size_t *count);
int numerix_piecewise_contains_breakpoint(const NUMERIX_PiecewiseFunction *piecewise,
                                          double x);
NUMERIX_Status numerix_piecewise_detect_discontinuity(const NUMERIX_PiecewiseFunction *piecewise,
                                                    double point,
                                                    NUMERIX_Discontinuity *discontinuity);
void numerix_piecewise_destroy(NUMERIX_PiecewiseFunction *piecewise);
double numerix_piecewise_evaluate(const NUMERIX_PiecewiseFunction *piecewise,
                                 double x);
double numerix_piecewise_solve_root(const NUMERIX_Function *function,
                                    double a,
                                    double b,
                                    double tolerance);

double numerix_bisection(const NUMERIX_Function *function,
                         double a,
                         double b,
                         double tolerance);

double numerix_newton_raphson(const NUMERIX_Function *function,
                              double initial_guess,
                              double tolerance);

double numerix_secant(const NUMERIX_Function *function,
                      double x0,
                      double x1,
                      double tolerance);

double numerix_forward_difference(const NUMERIX_Function *function,
                                  double x,
                                  double h);

double numerix_backward_difference(const NUMERIX_Function *function,
                                   double x,
                                   double h);

double numerix_central_difference(const NUMERIX_Function *function,
                                  double x,
                                  double h);

double numerix_linear_interpolation(const NUMERIX_Function *function,
                                   double x0,
                                   double y0,
                                   double x1,
                                   double y1,
                                   double x);

double numerix_lagrange_interpolation(const NUMERIX_Function *function,
                                      const double x_values[],
                                      const double y_values[],
                                      size_t n,
                                      double x);

double numerix_newton_interpolation(const NUMERIX_Function *function,
                                    const double x_values[],
                                    const double y_values[],
                                    size_t n,
                                    double x);

NUMERIX_Status numerix_vector_dot(const double *a, const double *b,
                                  size_t n, double *result);
NUMERIX_Status numerix_vector_norm2(const double *vector, size_t n,
                                    double *result);
NUMERIX_Status numerix_matrix_vector_multiply(const double *matrix,
                                             const double *vector,
                                             size_t rows, size_t columns,
                                             double *result);
NUMERIX_Status numerix_gaussian_elimination(const double *matrix,
                                            const double *right_hand_side,
                                            size_t n, double *solution);
NUMERIX_Status numerix_lu_decomposition(const double *matrix, size_t n,
                                        double *lower, double *upper,
                                        size_t *permutation);

double numerix_trapezoidal_integral(const NUMERIX_Function *function,
                                    double a, double b, size_t panels);
double numerix_simpson_integral(const NUMERIX_Function *function,
                                double a, double b, size_t panels);
double numerix_golden_section_minimize(const NUMERIX_Function *function,
                                       double a, double b, double tolerance,
                                       size_t max_iterations);
NUMERIX_Status numerix_rk4_solve(NUMERIX_ODECallback derivative,
                                 void *user_data,
                                 double t0, double y0, double t1,
                                 double step_size, size_t max_steps,
                                 double *y_final, size_t *steps_taken);

#ifdef __cplusplus
}
#endif

#endif /* NUMERIX_H */
