#ifndef NUMERIX_EXPRESSION_INTERNAL_H
#define NUMERIX_EXPRESSION_INTERNAL_H

#include <stddef.h>
#include <string.h>
#include "numerix.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    NUMERIX_TOKEN_NUMBER = 0,
    NUMERIX_TOKEN_IDENTIFIER = 1,
    NUMERIX_TOKEN_PLUS = 2,
    NUMERIX_TOKEN_MINUS = 3,
    NUMERIX_TOKEN_MULTIPLY = 4,
    NUMERIX_TOKEN_DIVIDE = 5,
    NUMERIX_TOKEN_POWER = 6,
    NUMERIX_TOKEN_LPAREN = 7,
    NUMERIX_TOKEN_RPAREN = 8,
    NUMERIX_TOKEN_COMMA = 9,
    NUMERIX_TOKEN_END = 10
} NUMERIX_TokenType;

typedef struct {
    NUMERIX_TokenType type;
    double value;
    char text[32];
} NUMERIX_Token;

typedef enum {
    NUMERIX_AST_NUMBER = 0,
    NUMERIX_AST_VARIABLE = 1,
    NUMERIX_AST_UNARY = 2,
    NUMERIX_AST_BINARY = 3,
    NUMERIX_AST_FUNCTION = 4
} NUMERIX_AstKind;

typedef struct NUMERIX_AstNode {
    NUMERIX_AstKind kind;
    char name[32];
    double number_value;
    int unary_op;
    struct NUMERIX_AstNode *left;
    struct NUMERIX_AstNode *right;
    struct NUMERIX_AstNode *argument;
} NUMERIX_AstNode;

NUMERIX_Status numerix_tokenize(const char *expression,
                                NUMERIX_Token **tokens,
                                size_t *token_count);
void numerix_free_tokens(NUMERIX_Token *tokens);

NUMERIX_AstNode *numerix_ast_new(NUMERIX_AstKind kind);
NUMERIX_AstNode *numerix_parse_expression_tokens(NUMERIX_Token *tokens,
                                                size_t token_count,
                                                NUMERIX_Status *status);
void numerix_ast_free(NUMERIX_AstNode *node);

void numerix_ast_evaluate(const NUMERIX_AstNode *node,
                         double x,
                         double *result,
                         int *ok);

#ifdef __cplusplus
}
#endif

#endif /* NUMERIX_EXPRESSION_INTERNAL_H */
