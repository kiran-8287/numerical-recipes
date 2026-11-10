#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "expression_internal.h"

static int numerix_is_function_name(const char *text)
{
    static const char *names[] = {"sin", "cos", "tan", "asin", "acos", "atan",
                                  "exp", "log", "log10", "sqrt", "abs"};
    size_t i;
    for (i = 0; i < sizeof(names) / sizeof(names[0]); ++i) {
        if (strcmp(text, names[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

static int numerix_is_constant_name(const char *text)
{
    return strcmp(text, "pi") == 0 || strcmp(text, "e") == 0;
}

static NUMERIX_AstNode *parse_primary(NUMERIX_Token *tokens, size_t *index, size_t token_count, NUMERIX_Status *status);
static NUMERIX_AstNode *parse_unary(NUMERIX_Token *tokens, size_t *index, size_t token_count, NUMERIX_Status *status);
static NUMERIX_AstNode *parse_power(NUMERIX_Token *tokens, size_t *index, size_t token_count, NUMERIX_Status *status);
static NUMERIX_AstNode *parse_multiplicative(NUMERIX_Token *tokens, size_t *index, size_t token_count, NUMERIX_Status *status);
static NUMERIX_AstNode *parse_additive(NUMERIX_Token *tokens, size_t *index, size_t token_count, NUMERIX_Status *status);

static NUMERIX_AstNode *parse_expression_node(NUMERIX_Token *tokens, size_t *index, size_t token_count, NUMERIX_Status *status)
{
    return parse_additive(tokens, index, token_count, status);
}

static NUMERIX_AstNode *parse_primary(NUMERIX_Token *tokens, size_t *index, size_t token_count, NUMERIX_Status *status)
{
    NUMERIX_AstNode *node = NULL;
    NUMERIX_Token current;

    if (*index >= token_count) {
        *status = NUMERIX_STATUS_INVALID_ARGUMENT;
        return NULL;
    }

    current = tokens[*index];

    if (current.type == NUMERIX_TOKEN_NUMBER) {
        node = numerix_ast_new(NUMERIX_AST_NUMBER);
        if (node == NULL) {
            *status = NUMERIX_STATUS_ALLOCATION_FAILED;
            return NULL;
        }
        node->number_value = current.value;
        (*index)++;
        return node;
    }

    if (current.type == NUMERIX_TOKEN_IDENTIFIER) {
        if (strcmp(current.text, "x") == 0) {
            node = numerix_ast_new(NUMERIX_AST_VARIABLE);
            if (node == NULL) {
                *status = NUMERIX_STATUS_ALLOCATION_FAILED;
                return NULL;
            }
            strncpy(node->name, current.text, sizeof(node->name) - 1U);
            (*index)++;
            return node;
        }

        if (numerix_is_constant_name(current.text)) {
            node = numerix_ast_new(NUMERIX_AST_VARIABLE);
            if (node == NULL) {
                *status = NUMERIX_STATUS_ALLOCATION_FAILED;
                return NULL;
            }
            strncpy(node->name, current.text, sizeof(node->name) - 1U);
            (*index)++;
            return node;
        }

        if (numerix_is_function_name(current.text)) {
            size_t function_index = *index;
            NUMERIX_AstNode *argument = NULL;
            (*index)++;
            if (*index >= token_count || tokens[*index].type != NUMERIX_TOKEN_LPAREN) {
                *status = NUMERIX_STATUS_INVALID_ARGUMENT;
                return NULL;
            }
            (*index)++;
            argument = parse_expression_node(tokens, index, token_count, status);
            if (*status != NUMERIX_STATUS_OK || argument == NULL) {
                return NULL;
            }
            if (*index >= token_count || tokens[*index].type != NUMERIX_TOKEN_RPAREN) {
                *status = NUMERIX_STATUS_INVALID_ARGUMENT;
                numerix_ast_free(argument);
                return NULL;
            }
            (*index)++;

            node = numerix_ast_new(NUMERIX_AST_FUNCTION);
            if (node == NULL) {
                *status = NUMERIX_STATUS_ALLOCATION_FAILED;
                numerix_ast_free(argument);
                return NULL;
            }
            strncpy(node->name, tokens[function_index].text, sizeof(node->name) - 1U);
            node->argument = argument;
            return node;
        }

        *status = NUMERIX_STATUS_INVALID_ARGUMENT;
        return NULL;
    }

    if (current.type == NUMERIX_TOKEN_LPAREN) {
        (*index)++;
        node = parse_expression_node(tokens, index, token_count, status);
        if (*status != NUMERIX_STATUS_OK) {
            return NULL;
        }
        if (*index >= token_count || tokens[*index].type != NUMERIX_TOKEN_RPAREN) {
            *status = NUMERIX_STATUS_INVALID_ARGUMENT;
            numerix_ast_free(node);
            return NULL;
        }
        (*index)++;
        return node;
    }

    *status = NUMERIX_STATUS_INVALID_ARGUMENT;
    return NULL;
}

static NUMERIX_AstNode *parse_unary(NUMERIX_Token *tokens, size_t *index, size_t token_count, NUMERIX_Status *status)
{
    NUMERIX_Token current;
    NUMERIX_AstNode *node = NULL;

    if (*index >= token_count) {
        *status = NUMERIX_STATUS_INVALID_ARGUMENT;
        return NULL;
    }

    current = tokens[*index];
    if (current.type == NUMERIX_TOKEN_PLUS) {
        (*index)++;
        node = parse_unary(tokens, index, token_count, status);
        return node;
    }

    if (current.type == NUMERIX_TOKEN_MINUS) {
        node = numerix_ast_new(NUMERIX_AST_UNARY);
        if (node == NULL) {
            *status = NUMERIX_STATUS_ALLOCATION_FAILED;
            return NULL;
        }
        (*index)++;
        node->argument = parse_unary(tokens, index, token_count, status);
        if (*status != NUMERIX_STATUS_OK) {
            numerix_ast_free(node);
            return NULL;
        }
        node->unary_op = '-';
        return node;
    }

    return parse_power(tokens, index, token_count, status);
}

static NUMERIX_AstNode *parse_power(NUMERIX_Token *tokens, size_t *index, size_t token_count, NUMERIX_Status *status)
{
    NUMERIX_AstNode *node = parse_primary(tokens, index, token_count, status);
    if (*status != NUMERIX_STATUS_OK) {
        return NULL;
    }

    if (*index < token_count && tokens[*index].type == NUMERIX_TOKEN_POWER) {
        NUMERIX_AstNode *rhs = NULL;
        NUMERIX_AstNode *binary = NULL;
        (*index)++;
        rhs = parse_unary(tokens, index, token_count, status);
        if (*status != NUMERIX_STATUS_OK || rhs == NULL) {
            numerix_ast_free(node);
            return NULL;
        }
        binary = numerix_ast_new(NUMERIX_AST_BINARY);
        if (binary == NULL) {
            numerix_ast_free(node);
            numerix_ast_free(rhs);
            *status = NUMERIX_STATUS_ALLOCATION_FAILED;
            return NULL;
        }
        strncpy(binary->name, "^", sizeof(binary->name) - 1U);
        binary->left = node;
        binary->right = rhs;
        return binary;
    }

    return node;
}

static NUMERIX_AstNode *parse_multiplicative(NUMERIX_Token *tokens, size_t *index, size_t token_count, NUMERIX_Status *status)
{
    NUMERIX_AstNode *left = parse_unary(tokens, index, token_count, status);
    NUMERIX_AstNode *binary = NULL;

    if (*status != NUMERIX_STATUS_OK || left == NULL) {
        return NULL;
    }

    while (*index < token_count &&
           (tokens[*index].type == NUMERIX_TOKEN_MULTIPLY ||
            tokens[*index].type == NUMERIX_TOKEN_DIVIDE)) {
        NUMERIX_Token operation = tokens[*index];
        NUMERIX_AstNode *right = NULL;
        (*index)++;
        right = parse_unary(tokens, index, token_count, status);
        if (*status != NUMERIX_STATUS_OK || right == NULL) {
            numerix_ast_free(left);
            return NULL;
        }
        binary = numerix_ast_new(NUMERIX_AST_BINARY);
        if (binary == NULL) {
            numerix_ast_free(left);
            numerix_ast_free(right);
            *status = NUMERIX_STATUS_ALLOCATION_FAILED;
            return NULL;
        }
        strncpy(binary->name,
                operation.type == NUMERIX_TOKEN_MULTIPLY ? "*" : "/",
                sizeof(binary->name) - 1U);
        binary->left = left;
        binary->right = right;
        left = binary;
    }

    return left;
}

static NUMERIX_AstNode *parse_additive(NUMERIX_Token *tokens, size_t *index, size_t token_count, NUMERIX_Status *status)
{
    NUMERIX_AstNode *left = parse_multiplicative(tokens, index, token_count, status);
    NUMERIX_AstNode *binary = NULL;

    if (*status != NUMERIX_STATUS_OK || left == NULL) {
        return NULL;
    }

    while (*index < token_count &&
           (tokens[*index].type == NUMERIX_TOKEN_PLUS ||
            tokens[*index].type == NUMERIX_TOKEN_MINUS)) {
        NUMERIX_Token operation = tokens[*index];
        NUMERIX_AstNode *right = NULL;
        (*index)++;
        right = parse_multiplicative(tokens, index, token_count, status);
        if (*status != NUMERIX_STATUS_OK || right == NULL) {
            numerix_ast_free(left);
            return NULL;
        }
        binary = numerix_ast_new(NUMERIX_AST_BINARY);
        if (binary == NULL) {
            numerix_ast_free(left);
            numerix_ast_free(right);
            *status = NUMERIX_STATUS_ALLOCATION_FAILED;
            return NULL;
        }
        strncpy(binary->name,
                operation.type == NUMERIX_TOKEN_PLUS ? "+" : "-",
                sizeof(binary->name) - 1U);
        binary->left = left;
        binary->right = right;
        left = binary;
    }

    return left;
}

NUMERIX_AstNode *numerix_parse_expression_tokens(NUMERIX_Token *tokens,
                                                size_t token_count,
                                                NUMERIX_Status *status)
{
    size_t index = 0;
    NUMERIX_AstNode *root = NULL;

    if (status == NULL) {
        return NULL;
    }

    *status = NUMERIX_STATUS_OK;

    if (tokens == NULL || token_count == 0U) {
        *status = NUMERIX_STATUS_INVALID_ARGUMENT;
        return NULL;
    }

    root = parse_expression_node(tokens, &index, token_count, status);
    if (*status != NUMERIX_STATUS_OK || root == NULL) {
        return NULL;
    }

    if (index >= token_count || tokens[index].type != NUMERIX_TOKEN_END) {
        numerix_ast_free(root);
        *status = NUMERIX_STATUS_INVALID_ARGUMENT;
        return NULL;
    }

    return root;
}
