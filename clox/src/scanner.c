#include "scanner.h"
#include "clox_common.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define TOKEN_LIST_INIT_CAPACITY 2

#define ERR_TOKEN (Token) { .type= TOKEN_ERROR }
#define UNKNOWN_TOKEN(Token) { TOKEN_EOF, tok_str, 0, scanner.line, }

size_t SYSCALL_FILESIZE(const char* filename) {
	struct stat st;
	stat(filename, &st);
	return (size_t)(st.st_size);
}

// @ Can trigger FATAL_ERR (and exit)
char *readFile(const char* filename) {
	if (!filename || filename[0] == '\0')
		PRINTF_FATAL_ERR("Filename passed incorrectly to readFile().");


	FILE* inputFile = fopen(filename, "r");
	if (!inputFile)
		PRINTF_FATAL_ERR("Unable to open file %s.", filename);

	size_t filesize = SYSCALL_FILESIZE(filename);
	char *source = malloc(filesize * sizeof(char));
	if (!source)
		PRINTF_FATAL_ERR("Failed to allocate buffer for file %s.", filename);


	size_t n_read = fread(source, sizeof(char), filesize, inputFile);
	if (n_read != filesize)
		PRINTF_FATAL_ERR("Failed to read %s, read (%zu/%zu)B.", filename, n_read, filesize);

	fclose(inputFile);
	return source;
}



// *INDENT-OFF* - we use a lot of weird switch statements
char* Token_to_string(Token t){
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
			PRINTF_FATAL_ERR("ERROR! to_string() on unrecognized token.");
			break;
	}

	char temp[128]; // sorta unsafe
	sprintf(temp, "[%s, '%s', l=%01d]",type_str, t.start, t.length);
	char* buf = malloc(strlen(temp)+1);
	strcpy(buf,temp);
	return buf;
}

// @ only supposed to be used by addToken function.
#define Token(TOK_T, TOK_STR, len)\
	(Token) { TOKEN_LEFT_PAREN, potential_tok_str, len, scanner.line}
// @ Creates a Token based on a potential_tok_str 
Token addToken(const char* potential_tok_str) {
	if (!potential_tok_str) 
		return ERR_TOKEN;

	size_t TokenLength = strlen(potential_tok_str);


	switch (TokenLength) {
	case 1:
		switch (potential_tok_str[0]) {
		case '(': return Token(TOKEN_LEFT_PAREN , tok_str, 1); break;
		case ')': return Token(TOKEN_RIGHT_PAREN, tok_str, 1); break;
		case '{': return Token(TOKEN_LEFT_BRACE , tok_str, 1); break;
		case '}': return Token(TOKEN_RIGHT_BRACE, tok_str, 1); break;
		case ',': return Token(TOKEN_COMMA	, tok_str, 1); break;
		case '.': return Token(TOKEN_DOT	, tok_str, 1); break;
		case '-': return Token(TOKEN_MINUS	, tok_str, 1); break;
		case '+': return Token(TOKEN_PLUS	, tok_str, 1); break;
		case ';': return Token(TOKEN_SEMICOLON	, tok_str, 1); break;
		case '*': return Token(TOKEN_STAR	, tok_str, 1); break;
		default:
			PRINTF_ERR_LN("Encountered unknown len=1 token --> '%c'\n", potential_tok_str[0]);
			return ERR_TOKEN;
			break;
		}
	break;

	default:
		PRINTF_ERR_LN("tokenlength=%zu!=1 exiting...\n", TokenLength);
		return ERR_TOKEN;
	break;
	}
	PRINTF_ERR_LN("FLOW CONTROL ERROR: Reached end of addToken execution.\n");
	return ERR_TOKEN;
}

void printToken(const char* prefix, Token t) {
	char *tokstr = Token_to_string(t);
	printf("%s%s\n", prefix, tokstr);
	free(tokstr);
}

Scanner_t Scanner(char* source) {
	return (Scanner_t) {
		.start = source,
		.current = source,
		.line = 1,
	};
}

int tokcount =0;
// @ Can trigger FATAL_ERR (and exit)
Token ScanNextToken() {
	if (scanner.current == NULL) {
		// end of input reached.
		fprintf(stderr, "Ending scan.\n");
		return (Token) {
			.type = TOKEN_EOF
		};
	}
	fprintf(stderr, "Scanning for token (%d):\n",tokcount);
	// test delimiter = ' '
	const char *delim = " ";
	char *tok_str = strsep(&scanner.current, delim);
	fprintf(stderr, "original: %p\n", tok_str);

	fprintf(stderr, "\tToken found:'%s'\n", tok_str);
	Token newtoken = addToken(tok_str);
	tokcount++;
	printToken("TOKEN:", newtoken);
	return newtoken;
}



// @ Can trigger FATAL_ERR (and exit)
void resize_tokens(TokenList_t* tl) {
	fprintf(stderr, "Resizing tokenlist.\n");
	tl->capacity *= 2;
	void *resized = realloc(tl->data, sizeof(Token) * tl->capacity);
	if (!resized) {
		free(tl->data);
		PRINTF_FATAL_ERR("Failed to realloc tokens arr.\n");
	}
}

// @ Can trigger FATAL_ERR (and exit)
void TokenList_add(TokenList_t* tl, Token t) {
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
		.data = malloc(sizeof(Token) * TOKEN_LIST_INIT_CAPACITY),
	};
}
void TokenList_destroy(TokenList_t* tl){
	free(tl->data);
}
