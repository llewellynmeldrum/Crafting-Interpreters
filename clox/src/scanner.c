#include "scanner.h"
#include "clox_common.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_TOKEN_LEN 2
#define TOKEN_LIST_INIT_CAPACITY 10

#define ERR_TOKEN (Token) { .type= TOKEN_ERROR }
#define UNKNOWN_TOKEN(Token) { TOKEN_EOF, tok_str, 0, scanner.line, }

bool Scanner_match(char target);
char Scanner_advance();
bool Scanner_isAtEnd();
char *Token_to_string(Token_t t);

size_t SYSCALL_FILESIZE(const char* filename) {
	struct stat st;
	stat(filename, &st);
	return (size_t)(st.st_size);
}

// @ Can trigger FATAL_ERR (and exit)
char *readFile(const char* filename) {
	if (!filename)
		PRINTF_FATAL_ERR("NULL string passed to readFile().");


	FILE* inputFile = fopen(filename, "r");
	if (!inputFile)
		PRINTF_FATAL_ERR("Unable to open file '%s.'", filename);

	size_t filesize = SYSCALL_FILESIZE(filename);
	char *source = malloc(filesize * sizeof(char));
	if (!source)
		PRINTF_FATAL_ERR("Failed to allocate buffer for file '%s'.", filename);


	size_t n_read = fread(source, sizeof(char), filesize, inputFile);
	if (n_read != filesize)
		PRINTF_FATAL_ERR("Failed to read '%s', read (%zu/%zu)B.", filename, n_read, filesize);

	fclose(inputFile);
	return source;
}



Token_t Token(TokenType t, size_t l) {
	return(Token_t) {
		.line = scanner.line,
		.start = scanner.current - l,
		.type = t,
		.length = l,
	};
}

// @ Creates a Token based on scanner.current and forward searching
Token_t Scanner_scanToken() {

	// *INDENT-OFF*
	char c = Scanner_advance();
	if (c=='\0'){
		fprintf(stderr, "Scanner encountered EOF\n");
		return Token(TOKEN_EOF, 1);
	}
	switch (c) {
	// single char lexemes
	case '(': return Token(TOKEN_LEFT_PAREN , 1); break;
	case ')': return Token(TOKEN_RIGHT_PAREN, 1); break;
	case '{': return Token(TOKEN_LEFT_BRACE , 1); break;
	case '}': return Token(TOKEN_RIGHT_BRACE, 1); break;
	case ',': return Token(TOKEN_COMMA	, 1); break;
	case '.': return Token(TOKEN_DOT	, 1); break;
	case '-': return Token(TOKEN_MINUS	, 1); break;
	case '+': return Token(TOKEN_PLUS	, 1); break;
	case ';': return Token(TOKEN_SEMICOLON	, 1); break;
	case '*': return Token(TOKEN_STAR	, 1); break;

	case '!':
		if (Scanner_match('='))
			return Token(TOKEN_BANG_EQUAL, 2);
		else return Token(TOKEN_BANG, 1);
		break;
	case '=':
		if (Scanner_match('='))
			return Token(TOKEN_EQUAL_EQUAL, 2);
		else return Token(TOKEN_EQUAL, 1);
		break;
	case '>':
		if (Scanner_match('='))
			return Token(TOKEN_GREATER_EQUAL, 2);
		else return Token(TOKEN_GREATER, 1);
		break;
	case '<':
		if (Scanner_match('='))
			return Token(TOKEN_LESS_EQUAL, 2);
		else return Token(TOKEN_LESS, 1);
		break;


	default:
		error("Encountered unknown token near --> '%c'\n", c);
		return Token(TOKEN_ERROR, -1);
		break;
	}



	error("FLOW CONTROL ERROR: Reached end of addToken execution.\n");
	return Token(TOKEN_ERROR, -1);
	//*INDENT-ON*
}

void printToken(const char* prefix, Token_t t) {
	char *tokstr = Token_to_string(t);
	printf("%s%s\n", prefix, tokstr);
	free(tokstr);
}

Scanner_t Scanner(char* source) {
	return (Scanner_t) {
		.start = source,
		.current = source,
		.line = 1,
		.length = strlen(source),
	};
}


// @ Can trigger FATAL_ERR (and exit)
Token_t ScanNextToken() {
	Token_t newtoken = Scanner_scanToken();
	printToken("TOKEN:", newtoken);
	return newtoken;
}

bool Scanner_isAtEnd() {
	return scanner.current >= (scanner.start + scanner.length);
}

char Scanner_advance() {
	char c = *scanner.current;
	scanner.current++;
	return c;
}

bool Scanner_match(char target) {
	if (Scanner_isAtEnd()) return false;
	if (*scanner.current != target) return false;

	scanner.current++;
	return true;
}


// @ Can trigger FATAL_ERR (and exit)
void resize_tokens(TokenList_t* tl) {
	fprintf(stderr, "Resizing tokenlist.\n");
	tl->capacity *= 2;
	void *resized = realloc(tl->data, sizeof(Token_t) * tl->capacity);
	if (!resized) {
		free(tl->data);
		PRINTF_FATAL_ERR("Failed to realloc tokens arr.\n");
	}
}

// @ Can trigger FATAL_ERR (and exit)
void TokenList_add(TokenList_t* tl, Token_t t) {
	fprintf(stderr, "adding token: %d\n", t.start[0]);
	if (tl->count >= tl->capacity) {
		resize_tokens(tl);
	}
	tl->data[tl->count++] = t;
}


TokenList_t TokenList() {
	return (TokenList_t) {
		.capacity = TOKEN_LIST_INIT_CAPACITY,
		.count = 0,
		.data = malloc(sizeof(Token_t) * TOKEN_LIST_INIT_CAPACITY),
	};
}

void TokenList_Print(TokenList_t* token_list) {
	printf("Printing TokenList:\n");
	for (size_t i = 0; i < token_list->count; i++) {
		printToken("\t", token_list->data[i]);
	}
	printf("\n");
}

void TokenList_destroy(TokenList_t* tl) {
	free(tl->data);
}

// *INDENT-OFF* - we use a lot of weird switch statements
char* Token_to_string(Token_t t){
	char type_str[32];

	switch (t.type){
	// One character tokens.
		case TOKEN_LEFT_PAREN 	: strcpy(type_str, "LEFT_PAREN");	break;
		case TOKEN_RIGHT_PAREN	: strcpy(type_str, "RIGHT_PAREN"); 	break;
		case TOKEN_LEFT_BRACE 	: strcpy(type_str, "LEFT_BRACE");	break;
		case TOKEN_RIGHT_BRACE	: strcpy(type_str, "RIGHT_BRACE"); 	break;
		case TOKEN_COMMA      	: strcpy(type_str, "COMMA");     	break;
		case TOKEN_DOT        	: strcpy(type_str, "DOT");       	break;
		case TOKEN_MINUS      	: strcpy(type_str, "MINUS");     	break;
		case TOKEN_PLUS       	: strcpy(type_str, "PLUS");      	break;
		case TOKEN_SEMICOLON  	: strcpy(type_str, "SEMICOLON"); 	break;
		case TOKEN_STAR       	: strcpy(type_str, "TOKEN_STAR");	break;
	// One to two char tokens
		case TOKEN_BANG         : strcpy(type_str, "TOKEN_BANG");	break;
		case TOKEN_BANG_EQUAL   : strcpy(type_str, "TOKEN_BANG_EQUAL");	break;
		case TOKEN_EQUAL        : strcpy(type_str, "TOKEN_EQUAL");	break;
		case TOKEN_EQUAL_EQUAL  : strcpy(type_str, "TOKEN_EQUAL_EQUAL");break;
		case TOKEN_GREATER      : strcpy(type_str, "TOKEN_GREATER");	break;
		case TOKEN_GREATER_EQUAL: strcpy(type_str, "TOKEN_GREATER_EQUAL");break;
		case TOKEN_LESS         : strcpy(type_str, "TOKEN_LESS");	break;
		case TOKEN_LESS_EQUAL   : strcpy(type_str, "TOKEN_LESS_EQUAL");	break;
	// Literals 
		case TOKEN_IDENTIFIER   : strcpy(type_str, "TOKEN_IDENTIFIER");	break;
		case TOKEN_STRING       : strcpy(type_str, "TOKEN_STRING");	break;
		case TOKEN_NUMBER       : strcpy(type_str, "TOKEN_NUMBER");	break;
	// Keywords 
		case TOKEN_AND          : strcpy(type_str, "TOKEN_AND");	break;
		case TOKEN_CLASS        : strcpy(type_str, "TOKEN_CLASS");	break;
		case TOKEN_ELSE         : strcpy(type_str, "TOKEN_ELSE");	break;
		case TOKEN_FALSE        : strcpy(type_str, "TOKEN_FALSE");	break;
		case TOKEN_FOR          : strcpy(type_str, "TOKEN_FOR");	break;
		case TOKEN_FUN          : strcpy(type_str, "TOKEN_FUN");	break;
		case TOKEN_IF           : strcpy(type_str, "TOKEN_IF");		break;
		case TOKEN_NIL          : strcpy(type_str, "TOKEN_NIL");	break;
		case TOKEN_OR           : strcpy(type_str, "TOKEN_OR");		break;
		case TOKEN_PRINT        : strcpy(type_str, "TOKEN_PRINT");	break;
		case TOKEN_RETURN       : strcpy(type_str, "TOKEN_RETURN");	break;
		case TOKEN_SUPER        : strcpy(type_str, "TOKEN_SUPER");	break;
		case TOKEN_THIS         : strcpy(type_str, "TOKEN_THIS");	break;
		case TOKEN_TRUE         : strcpy(type_str, "TOKEN_TRUE");	break;
		case TOKEN_VAR          : strcpy(type_str, "TOKEN_VAR");	break;
		case TOKEN_WHILE        : strcpy(type_str, "TOKEN_WHILE");	break;
	// Errors 
		case TOKEN_ERROR        : strcpy(type_str, "TOKEN_ERROR");	break;
		case TOKEN_EOF          : strcpy(type_str, "TOKEN_EOF");	break;

		default	:
			printf("ERROR! to_string() on unrecognized token.\n");
			break;
	}

	char plaintext[128];
	if (t.length < 0){
		PRINTF_FATAL_ERR("Tried to print token with L<%d (L=%d)",MAX_TOKEN_LEN,t.length);
	}
	if (t.length > MAX_TOKEN_LEN){
		PRINTF_FATAL_ERR("Tried to print token with L>%d (L=%d)",MAX_TOKEN_LEN,t.length);
	}
	strncpy(plaintext, t.start, t.length);

	char temp[128]; // sorta unsafe
	sprintf(temp, "%s, '%s' (l=%d)",type_str, plaintext,t.length);

	char* buf = malloc(strlen(temp)+1);
	strcpy(buf,temp);
	return buf;
}
