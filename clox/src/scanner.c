#include "scanner.h"
#include "clox_common.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>



#define ERR_TOKEN (Token) { .type= TOKEN_EOF }
#define UNKNOWN_TOKEN(Token) { TOKEN_EOF, tok_str, 0, scanner.line, }

size_t SYSCALL_FILESIZE(const char* filename) {
	struct stat st;
	stat(filename, &st);
	return (size_t)(st.st_size);
}

char *readFile(const char* filename) {
	if (!filename || filename[0] == '\0') PRINTF_FATAL_ERR("Filename passed incorrectly to readFile().");
	size_t filesize = SYSCALL_FILESIZE(filename);

	FILE* inputFile = fopen(filename, "r");
	if (!inputFile) PRINTF_FATAL_ERR("Unable to open file %s.", filename);

	char *buf = malloc(filesize * sizeof(char));
	if (!buf) PRINTF_FATAL_ERR("Failed to allocate buffer for file %s.", filename);
	size_t bytesRead = fread(buf, sizeof(char), filesize, inputFile);
	if (bytesRead != filesize) PRINTF_FATAL_ERR("Failed to read %s, read (%zu/%zu)B.", filename, bytesRead, filesize);
	fclose(inputFile);
	return buf;
}

// advances current, creates and returns token.
// *INDENT-OFF* - we use a lot of weird switch statements
#define Token(TOK_T, TOK_STR, len)\
	(Token) { TOKEN_LEFT_PAREN, tok_str, len, scanner.line}


char* Token_to_string(Token t){
	char type_str[32];

	switch (t.type){
		case TOKEN_LEFT_PAREN 	: strcpy(type_str, "LEFT_PAREN");	 break;
		case TOKEN_RIGHT_PAREN	: strcpy(type_str, "RIGHT_PAREN"); 	 break;
		case TOKEN_LEFT_BRACE 	: strcpy(type_str, "LEFT_BRACE");	 break;
		case TOKEN_RIGHT_BRACE	: strcpy(type_str, "RIGHT_BRACE"); 	 break;
		case TOKEN_COMMA      	: strcpy(type_str, "COMMA");     	 break;
		case TOKEN_DOT        	: strcpy(type_str, "DOT");       	 break;
		case TOKEN_MINUS      	: strcpy(type_str, "MINUS");     	 break;
		case TOKEN_PLUS       	: strcpy(type_str, "PLUS");      	 break;
		case TOKEN_SEMICOLON  	: strcpy(type_str, "SEMICOLON"); 	 break;
		case TOKEN_STAR       	: strcpy(type_str, "STAR");      	 break;
		default			: strcpy(type_str, "UNKNOWN_TOK"); 	 break;
	}
	// account for gaps in the format string (and the null terminator)
	char temp[128];
	sprintf(temp, "[%s, '%s', l=%01d]",type_str, t.start, t.length);
	char* buf = malloc(strlen(temp)+1);
	strcpy(buf,temp);
	return buf;
}

Token addToken(const char* tok_str) {

	if (!tok_str) return ERR_TOKEN;

	size_t TokenLength = 0;
	if (tok_str[1] == '\0' || tok_str[1] == '\n'){
		TokenLength = 1;
	} else{
		printf("tokenlength>1, exiting...\n");
		TokenLength = INT_MAX; // temporary
		return ERR_TOKEN;
	}


	switch (TokenLength) {
	case 1:
		switch (tok_str[0]) {
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
			PRINTF_ERR_LN("Encountered unknown token --> '%c'\n", tok_str[0]);
			break;
		}
	default:
		return (Token) {.type=TOKEN_EOF};
		break;
	}
}
// *INDENT-ON*

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

Token ScanNextToken() {
	if (scanner.current == NULL) {
		// end of input reached.
		fprintf(stderr, "Ending scan.\n");
		return (Token) {
			.type = TOKEN_EOF
		};
	}
	fprintf(stderr, "Scanning for another token:\n");
	// test delimiter = ' '
	const char *delim = " ";
	char *tok_str = strsep(&scanner.current, delim);
	fprintf(stderr, "original: %p\n", tok_str);

	fprintf(stderr, "\tToken found:'%s'\n", tok_str);
	Token newtoken = addToken(tok_str);
	printToken("TOKEN:", newtoken);
	return newtoken;
}
