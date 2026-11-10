#include <math.h>
#include <stdio.h>
#include <string.h>
#include "expression_internal.h"

static int numerix_is_valid_number(double value)
{
    return isfinite(value);
}

static double numerix_eval_function(const char *name, double x)
{
    if (strcmp(name, "sin") == 0) {
        return sin(x);
    }
    if (strcmp(name, "cos") == 0) {
        return cos(x);
    }
    if (strcmp(name, "tan") == 0) {
        return tan(x);
    }
    if (strcmp(name, "asin") == 0) {
        return asin(x);
    }
    if (strcmp(name, "acos") == 0) {
        return acos(x);
    }
    if (strcmp(name, "atan") == 0) {
        return atan(x);
    }
    if (strcmp(name, "exp") == 0) {
        return exp(x);
    }
    if (strcmp(name, "log") == 0) {
        return log(x);
    }
    if (strcmp(name, "log10") == 0) {
        return log10(x);
    }
    if (strcmp(name, "sqrt") == 0) {
        return sqrt(x);
    }
    if (strcmp(name, "abs") == 0) {
        return fabs(x);
    }
    return NAN;
}

void numerix_ast_evaluate(const NUMERIX_AstNode *node,
                         double x,
                         double *result,
                         int *ok)
{
    double left = 0.0;
    double right = 0.0;
    int left_ok = 0;
    int right_ok = 0;

    if (node == NULL || result == NULL || ok == NULL) {
        if (ok != NULL) {
            *ok = 0;
        }
        return;
    }

    *ok = 1;

    switch (node->kind) {
        case NUMERIX_AST_NUMBER:
            *result = node->number_value;
            break;
        case NUMERIX_AST_VARIABLE:
            if (strcmp(node->name, "x") == 0) {
                *result = x;
            } else if (strcmp(node->name, "pi") == 0) {
                *result = 3.14159265358979323846;
            } else if (strcmp(node->name, "e") == 0) {
                *result = 2.71828182845904523536;
            } else {
                *ok = 0;
                *result = NAN;
            }
            break;
        case NUMERIX_AST_UNARY:
            numerix_ast_evaluate(node->argument, x, &left, &left_ok);
            if (!left_ok) {
                *ok = 0;
                *result = NAN;
                break;
            }
            if (node->unary_op == '-') {
                *result = -left;
            } else {
                *ok = 0;
                *result = NAN;
            }
            break;
        case NUMERIX_AST_BINARY:
            numerix_ast_evaluate(node->left, x, &left, &left_ok);
            numerix_ast_evaluate(node->right, x, &right, &right_ok);
            if (!left_ok || !right_ok) {
                *ok = 0;
                *result = NAN;
                break;
            }
            if (strcmp(node->name, "+") == 0) {
                *result = left + right;
            } else if (strcmp(node->name, "-") == 0) {
                *result = left - right;
            } else if (strcmp(node->name, "*") == 0) {
                *result = left * right;
            } else if (strcmp(node->name, "/") == 0) {
                if (right == 0.0) {
                    *ok = 0;
                    *result = NAN;
                } else {
                    *result = left / right;
                }
            } else if (strcmp(node->name, "^") == 0) {
                *result = pow(left, right);
            } else {
                *ok = 0;
                *result = NAN;
            }
            break;
        case NUMERIX_AST_FUNCTION:
            numerix_ast_evaluate(node->argument, x, &left, &left_ok);
            if (!left_ok) {
                *ok = 0;
                *result = NAN;
                break;
            }
            *result = numerix_eval_function(node->name, left);
            if (!isfinite(*result)) {
                *ok = 0;
            }
            break;
        default:
            *ok = 0;
            *result = NAN;
            break;
    }

    if (*ok && !numerix_is_valid_number(*result)) {
        *ok = 0;
        *result = NAN;
    }
}
