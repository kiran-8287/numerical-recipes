#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "expression_internal.h"

static int numerix_expression_has_division_by_zero(const NUMERIX_AstNode *node)
{
    if (node == NULL) {
        return 0;
    }

    if (node->kind == NUMERIX_AST_BINARY) {
        if (strcmp(node->name, "/") == 0) {
            if (node->right != NULL && node->right->kind == NUMERIX_AST_NUMBER && node->right->number_value == 0.0) {
                return 1;
            }
        }
        if (numerix_expression_has_division_by_zero(node->left)) {
            return 1;
        }
        if (numerix_expression_has_division_by_zero(node->right)) {
            return 1;
        }
    }

    if (node->kind == NUMERIX_AST_UNARY) {
        return numerix_expression_has_division_by_zero(node->argument);
    }

    if (node->kind == NUMERIX_AST_FUNCTION) {
        return numerix_expression_has_division_by_zero(node->argument);
    }

    return 0;
}

static double numerix_expression_eval_callback(double x, void *user_data)
{
    NUMERIX_AstNode *root = (NUMERIX_AstNode *)user_data;
    int ok = 1;
    double value = 0.0;

    if (root == NULL) {
        return NAN;
    }

    numerix_ast_evaluate(root, x, &value, &ok);
    if (!ok || !isfinite(value)) {
        return NAN;
    }

    return value;
}

NUMERIX_Status numerix_expression_parse(NUMERIX_Function *function,
                                       const char *expression)
{
    NUMERIX_Token *tokens = NULL;
    size_t token_count = 0U;
    NUMERIX_AstNode *root = NULL;
    NUMERIX_Status status = NUMERIX_STATUS_OK;

    if (function == NULL || expression == NULL) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    numerix_function_destroy(function);

    status = numerix_tokenize(expression, &tokens, &token_count);
    if (status != NUMERIX_STATUS_OK) {
        return status;
    }

    root = numerix_parse_expression_tokens(tokens, token_count, &status);
    numerix_free_tokens(tokens);
    if (status != NUMERIX_STATUS_OK || root == NULL) {
        return status == NUMERIX_STATUS_OK ? NUMERIX_STATUS_INVALID_ARGUMENT : status;
    }

    if (numerix_expression_has_division_by_zero(root)) {
        numerix_ast_free(root);
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    function->type = NUMERIX_FUNCTION_EXPRESSION;
    function->evaluate = numerix_expression_eval_callback;
    function->derivative = NULL;
    function->user_data = root;
    function->owns_resources = 1;
    function->domain_known = 0;
    function->continuous = 0;
    function->differentiable = 0;

    return NUMERIX_STATUS_OK;
}

void numerix_function_destroy(NUMERIX_Function *function)
{
    if (function == NULL) {
        return;
    }

    if (function->owns_resources && function->user_data != NULL) {
        numerix_ast_free((NUMERIX_AstNode *)function->user_data);
    }

    function->type = NUMERIX_FUNCTION_NONE;
    function->evaluate = NULL;
    function->derivative = NULL;
    function->user_data = NULL;
    function->owns_resources = 0;
}

double numerix_function_evaluate(const NUMERIX_Function *function,
                                 double x,
                                 void *user_data)
{
    if (function == NULL) {
        return NAN;
    }

    if (function->type == NUMERIX_FUNCTION_EXPRESSION) {
        return numerix_expression_eval_callback(x, function->user_data);
    }

    if (function->type == NUMERIX_FUNCTION_PIECEWISE) {
        return numerix_piecewise_evaluate((const NUMERIX_PiecewiseFunction *)function->user_data, x);
    }

    if (function->evaluate != NULL) {
        return function->evaluate(x, user_data != NULL ? user_data : function->user_data);
    }

    return NAN;
}
