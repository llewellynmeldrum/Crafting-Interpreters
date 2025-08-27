#ifndef SCANNER_H
#define SCANNER_H
#include "clox_common.h"


typedef struct Scanner_t {
	const char *start;
	char *current;
	int line;
	size_t length;
} Scanner_t;
Scanner_t scanner;

// # Scanner_t Constructor (.h)
Scanner_t Scanner(char* source);

typedef enum {
	// Single-character tokens.
	TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
	TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
	TOKEN_COMMA, TOKEN_DOT, TOKEN_MINUS, TOKEN_PLUS,
	TOKEN_SEMICOLON, TOKEN_SLASH, TOKEN_STAR,

	// One or two character tokens.
	TOKEN_BANG, TOKEN_BANG_EQUAL,
	TOKEN_EQUAL, TOKEN_EQUAL_EQUAL,
	TOKEN_GREATER, TOKEN_GREATER_EQUAL,
	TOKEN_LESS, TOKEN_LESS_EQUAL,
	// Literals.
	TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_NUMBER,

	// Keywords.
	TOKEN_AND, TOKEN_CLASS, TOKEN_ELSE, TOKEN_FALSE,
	TOKEN_FOR, TOKEN_FUN, TOKEN_IF, TOKEN_NIL, TOKEN_OR,
	TOKEN_PRINT, TOKEN_RETURN, TOKEN_SUPER, TOKEN_THIS,
	TOKEN_TRUE, TOKEN_VAR, TOKEN_WHILE,

	TOKEN_ERROR, TOKEN_EOF,
	TOKEN_EMPTY
} TokenType;

// @-> .start is the ptr to source on the heap,
// @-> .length is the length of the token, IGNORES NULL TERMINAL
typedef struct Token {
	TokenType type;
	const char *start;
	int length;
	int line;
} Token_t;
Token_t ScanNextToken();

// @-> dynamic list of Tokens, resize factor=2
// @----|Methods|----
// @ constructor: TokenList()
// @ deconstructor: TokenList_destroy(this)
// @ add: void TokenList_add(this, tok)
typedef struct TokenList_t {
	Token_t *data;
	size_t capacity;
	size_t count;
} TokenList_t;
TokenList_t TokenList();
void TokenList_add(TokenList_t* tl, Token_t t);
void TokenList_destroy(TokenList_t* tl);
void TokenList_Print(TokenList_t* tl);


// @ returns heap allocated plaintext file contents (ascii)
// @ Can trigger FATAL_ERR (and exit)
char *readFile(const char* filename);

//char* token_to_str(Token tok, char* str){ }

#endif // SCANNER_H
