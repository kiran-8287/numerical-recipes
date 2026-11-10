#include <stdlib.h>
#include <string.h>
#include "expression_internal.h"

NUMERIX_AstNode *numerix_ast_new(NUMERIX_AstKind kind)
{
    NUMERIX_AstNode *node = (NUMERIX_AstNode *)numerix_malloc(sizeof(*node));
    if (node == NULL) {
        return NULL;
    }
    memset(node, 0, sizeof(*node));
    node->kind = kind;
    return node;
}

void numerix_ast_free(NUMERIX_AstNode *node)
{
    if (node == NULL) {
        return;
    }

    numerix_ast_free(node->left);
    numerix_ast_free(node->right);
    numerix_ast_free(node->argument);
    free(node);
}
