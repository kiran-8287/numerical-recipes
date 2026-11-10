#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "expression_internal.h"

#define NUMERIX_EXPRESSION_MAX_LENGTH 1024U

static int numerix_is_identifier_start(char c)
{
    return isalpha((unsigned char)c) || c == '_';
}

static int numerix_is_identifier_char(char c)
{
    return isalnum((unsigned char)c) || c == '_';
}

static NUMERIX_Status numerix_token_reserve(NUMERIX_Token **tokens,
                                            size_t *capacity,
                                            size_t needed)
{
    size_t new_capacity;
    NUMERIX_Token *resized;

    if (needed <= *capacity) {
        return NUMERIX_STATUS_OK;
    }
    if (needed > SIZE_MAX / sizeof(**tokens)) {
        return NUMERIX_STATUS_RESOURCE_LIMIT;
    }
    new_capacity = *capacity;
    while (new_capacity < needed) {
        if (new_capacity > SIZE_MAX / 2U) {
            new_capacity = needed;
            break;
        }
        new_capacity *= 2U;
    }
    if (new_capacity > SIZE_MAX / sizeof(**tokens)) {
        return NUMERIX_STATUS_RESOURCE_LIMIT;
    }
    resized = (NUMERIX_Token *)realloc(*tokens, new_capacity * sizeof(**tokens));
    if (resized == NULL) {
        return NUMERIX_STATUS_ALLOCATION_FAILED;
    }
    *tokens = resized;
    *capacity = new_capacity;
    return NUMERIX_STATUS_OK;
}

NUMERIX_Status numerix_tokenize(const char *expression,
                                NUMERIX_Token **tokens,
                                size_t *token_count)
{
    size_t capacity = 32U;
    size_t length = 0U;
    NUMERIX_Token *result;
    const char *p;

    if (expression == NULL || tokens == NULL || token_count == NULL) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }
    *tokens = NULL;
    *token_count = 0U;
    if (strlen(expression) > NUMERIX_EXPRESSION_MAX_LENGTH) {
        return NUMERIX_STATUS_RESOURCE_LIMIT;
    }
    result = (NUMERIX_Token *)numerix_malloc(capacity * sizeof(*result));
    if (result == NULL) {
        return NUMERIX_STATUS_ALLOCATION_FAILED;
    }
    p = expression;

    while (*p != '\0') {
        NUMERIX_Status reserve_status;
        while (*p != '\0' && isspace((unsigned char)*p)) {
            ++p;
        }
        if (*p == '\0') {
            break;
        }
        if (length > SIZE_MAX - 2U) {
            free(result);
            return NUMERIX_STATUS_RESOURCE_LIMIT;
        }
        reserve_status = numerix_token_reserve(&result, &capacity, length + 2U);
        if (reserve_status != NUMERIX_STATUS_OK) {
            free(result);
            return reserve_status;
        }

        if (isdigit((unsigned char)*p) || *p == '.') {
            char buffer[128];
            char *end = NULL;
            size_t index = 0U;
            int saw_digit = 0;
            int saw_dot = 0;
            double number;

            while (isdigit((unsigned char)*p) || *p == '.') {
                if (*p == '.') {
                    if (saw_dot) {
                        free(result);
                        return NUMERIX_STATUS_INVALID_ARGUMENT;
                    }
                    saw_dot = 1;
                } else {
                    saw_digit = 1;
                }
                if (index >= sizeof(buffer) - 1U) {
                    free(result);
                    return NUMERIX_STATUS_RESOURCE_LIMIT;
                }
                buffer[index++] = *p++;
            }
            if (!saw_digit) {
                free(result);
                return NUMERIX_STATUS_INVALID_ARGUMENT;
            }
            if (*p == 'e' || *p == 'E') {
                if (index >= sizeof(buffer) - 1U) {
                    free(result);
                    return NUMERIX_STATUS_RESOURCE_LIMIT;
                }
                buffer[index++] = *p++;
                if (*p == '+' || *p == '-') {
                    if (index >= sizeof(buffer) - 1U) {
                        free(result);
                        return NUMERIX_STATUS_RESOURCE_LIMIT;
                    }
                    buffer[index++] = *p++;
                }
                if (!isdigit((unsigned char)*p)) {
                    free(result);
                    return NUMERIX_STATUS_INVALID_ARGUMENT;
                }
                while (isdigit((unsigned char)*p)) {
                    if (index >= sizeof(buffer) - 1U) {
                        free(result);
                        return NUMERIX_STATUS_RESOURCE_LIMIT;
                    }
                    buffer[index++] = *p++;
                }
            }
            buffer[index] = '\0';
            errno = 0;
            number = strtod(buffer, &end);
            if (end != buffer + index || errno == ERANGE || !isfinite(number)) {
                free(result);
                return NUMERIX_STATUS_INVALID_ARGUMENT;
            }
            result[length].type = NUMERIX_TOKEN_NUMBER;
            result[length].value = number;
            result[length].text[0] = '\0';
            ++length;
            continue;
        }

        if (numerix_is_identifier_start(*p)) {
            size_t index = 0U;
            result[length].type = NUMERIX_TOKEN_IDENTIFIER;
            while (numerix_is_identifier_char(*p)) {
                if (index >= sizeof(result[length].text) - 1U) {
                    free(result);
                    return NUMERIX_STATUS_RESOURCE_LIMIT;
                }
                result[length].text[index++] = *p++;
            }
            result[length].text[index] = '\0';
            result[length].value = 0.0;
            ++length;
            continue;
        }

        switch (*p) {
        case '+': result[length].type = NUMERIX_TOKEN_PLUS; break;
        case '-': result[length].type = NUMERIX_TOKEN_MINUS; break;
        case '*': result[length].type = NUMERIX_TOKEN_MULTIPLY; break;
        case '/': result[length].type = NUMERIX_TOKEN_DIVIDE; break;
        case '^': result[length].type = NUMERIX_TOKEN_POWER; break;
        case '(': result[length].type = NUMERIX_TOKEN_LPAREN; break;
        case ')': result[length].type = NUMERIX_TOKEN_RPAREN; break;
        case ',': result[length].type = NUMERIX_TOKEN_COMMA; break;
        default:
            free(result);
            return NUMERIX_STATUS_INVALID_ARGUMENT;
        }
        result[length].value = 0.0;
        result[length].text[0] = '\0';
        ++length;
        ++p;
    }

    result[length].type = NUMERIX_TOKEN_END;
    result[length].value = 0.0;
    result[length].text[0] = '\0';
    *tokens = result;
    *token_count = length + 1U;
    return NUMERIX_STATUS_OK;
}

void numerix_free_tokens(NUMERIX_Token *tokens)
{
    free(tokens);
}
