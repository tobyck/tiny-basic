#ifndef INCLUDE_LEXER_H
#define INCLUDE_LEXER_H

#include <stddef.h>
#include <stdbool.h>

typedef enum {
	TOKEN_LET,
	TOKEN_NAME,
	TOKEN_ASSIGN,
	TOKEN_NUMBER,
	TOKEN_BINARY_OP,
	TOKEN_UNARY_OP,
	TOKEN_OPEN_PAREN,
	TOKEN_CLOSE_PAREN,
	TOKEN_PRINT,
	TOKEN_STRING,
	TOKEN_COMMA,
	TOKEN_SEMICOLON,
	TOKEN_EOF
} TokenType;

extern char *stringify_token_type(TokenType token_type);

typedef struct {
	TokenType type;
	char *string_literal;
	char char_literal;
	size_t line, column;
} Token;

extern void free_token_literal(Token token);

typedef struct {
	char *message;
	size_t line, start_column, error_column;
} Error;

typedef struct {
	bool success;
	union {
		Token token;
		Error error;
	} result;
} TokenResult;

typedef struct {
	Token *tokens;
	size_t capacity;
	size_t length;
	size_t next_index;
	bool peeked;
} TokenBuffer;

typedef struct {
	char *code;
	size_t current_index;
	size_t line;
	size_t column_start; // index in code of first char in current column
	TokenBuffer tokens;
} Lexer;

extern Lexer *new_lexer(char *code, size_t buffer_capacity);
extern void free_lexer(Lexer *lexer);

extern inline char peek(Lexer *lexer);
extern inline char consume(Lexer *lexer);
extern bool case_insensitive_match(Lexer *lexer, char *str);
extern inline bool valid_variable_char(Lexer *lexer);

// this is where the actual tokenising happens
extern TokenResult _get_next_token(Lexer *lexer);

// if result is a success then write it to the buffer at the correct index
extern void _write_token_result(
	Lexer *lexer,
	TokenResult token_result,
	size_t index
);

// these are the functions that will be used in the parser (these are the ones
// which actually interact with the token buffer)
extern TokenResult peek_token(Lexer *lexer);
extern TokenResult next_token(Lexer *lexer);
extern Token *get_most_recent_token(Lexer *lexer);

#endif // INCLUDE_LEXER_H
