#include <limits.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> // getting filesize

#include "scanner.h"
#include "clox_common.h"
#include "hashtable.h"


#include "logging.h" // should be last to overwrite log()

#define MAX_TOKEN_LEN 128
#define TOKEN_LIST_INIT_CAPACITY 20 // TODO: RESIZING IS BROKEN


#define ERR_TOKEN (Token) { .type= TOKEN_ERROR }

// @ checks if the next char == target, consumes a character
bool Scanner_match(char target);
// @ Advances the scanner, consuming a character
char Scanner_advance();
// @ self explanatory
bool Scanner_isAtEnd();
// @ checks next char, without consuming
char Scanner_peek();
// @ checks next, next char, without consuming
char Scanner_peekNext();
// @ consumes one char, checking if it matches target
bool Scanner_match(const char target);

char *Token_to_string(Token_t t);
void printToken(const char* prefix, Token_t t);


HashTable_t InitKeywordsMap();
void DestroyKeywordsMap(HashTable_t ht);
#define Keywords_get(keyword) HashTable_get(&Keywords, keyword)
HashTable_t Keywords;

Token_t Token(TokenType t, size_t l) {
	return(Token_t) {
		.line = scanner.line,
		.start = scanner.current - l,
		.type = t,
		.length = l,
	};
}
#define EMPTY_TOKEN() 	Token(TOKEN_EMPTY, 0)
#define EOF_TOKEN()	Token(TOKEN_EOF, -1)

typedef enum {
	SINGLE_LINE_COMMENT,
	STRING_LITERAL,
	NUMBER,
	IDENTIFIER,
} LookAhead_t;

// *INDENT-OFF*
Token_t Scanner_lookahead(LookAhead_t target) {
	if(target==SINGLE_LINE_COMMENT){
		if (!Scanner_match('/')) {
			return Token(TOKEN_SLASH, 1);

		}
		char comment_str[256];
		int i = 0;
		while (Scanner_peek() != '\n' && !Scanner_isAtEnd()) {
			comment_str[i] = Scanner_peek();
			i++;
			Scanner_advance();
		}
		Token_t empty_tok = Token(TOKEN_EMPTY, 0);
		comment_str[i] = '\0';
		log("\tParsed comment: \"//%s\"\n",comment_str);
		return empty_tok;
	} else if (target==STRING_LITERAL){
		char *str_start = scanner.current; // skip the opening quote (")
		size_t str_len = 0;
		while (Scanner_peek() != '"' && !Scanner_isAtEnd()) {
			if (Scanner_peek() == '\n') scanner.line++;
			Scanner_advance();
			str_len++;
		}
		if (Scanner_isAtEnd()) {
			char *str_contents = malloc(sizeof(char) * str_len + 1);
			strncpy(str_contents, str_start, str_len + 1);
			error("UNTERMINATED STRING: '%s'",str_contents);
			return (Token_t) {
				.type = TOKEN_EMPTY,
				.start = str_start,
				.line = scanner.line,
				.length = 1,
			};
		}
		char *str_contents = malloc(sizeof(char) * str_len + 1);
		strncpy(str_contents, str_start, str_len + 1);

		Token_t str_token = (Token_t) {
			.type = TOKEN_STRING,
			.start = str_contents,
			.line = scanner.line,
			.length = str_len,
		};
		Scanner_advance(); // consume the closing (")
		return str_token;
	} else if (target==NUMBER){
		char* num_start = scanner.current-1; // include first digit 
		while (isDigit(Scanner_peek())){
			Scanner_advance(); // consume digits 
		}

		if (Scanner_peek()=='.'){
			if (isDigit(Scanner_peekNext()) == false){
				// trailing '123.?'
				error("TRAILING '.' detected!\n");
				return EMPTY_TOKEN();
			}

			Scanner_advance();
			while (isDigit(Scanner_peek())){
				Scanner_advance(); // consume RHS of '.'
			}
		}
		// so now, we have a number, potentially fractional.
		Token_t numtoken = (Token_t){
			.type = TOKEN_NUMBER,
			.start = num_start,
			.length = scanner.current-num_start,
			.line = scanner.line,
		};
		return numtoken;

	} else if (target==IDENTIFIER){
		char* start = scanner.current-1;
		while (isAlpha(Scanner_peek()) || isDigit(Scanner_peek())){
			Scanner_advance();
		}
		// max identifier length?
		int len = scanner.current-start;
		char *potentialKeyword = calloc(len+1,sizeof(char));
		strncpy(potentialKeyword, start, len);

		TokenType type = Keywords_get(potentialKeyword);
		if (type == HMAP_NO_ENTRY){
			type = TOKEN_IDENTIFIER;
		}

		Token_t token = (Token_t){
			.type = type,
			.start = start,
			.length = len,
			.line = scanner.line,
		};

		return token;
	} else {
		PRINTF_FATAL_ERR("Invalid lookahead target!");
		return EMPTY_TOKEN();
	}
}

// *INDENT-ON*
// @ Creates a Token based on scanner.current and forward searching
Token_t Scanner_scanToken() {

	// *INDENT-OFF*
	char c = Scanner_advance();
	if (c=='\0'){
		fprintf(stderr, "Scanner encountered EOF\n");
		return EOF_TOKEN(); 
	}
	switch (c) {
	// 1 char lexemes 
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

	//*INDENT-ON*
		// 1-2 char
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
	case '/':
		return Scanner_lookahead(SINGLE_LINE_COMMENT);
		break;
	case '"':
		return Scanner_lookahead(STRING_LITERAL);
		break;
	case ' ':
	case '\r':
	case '\t':
		return EMPTY_TOKEN();
		break;
	case '\n':
		scanner.line++;
		return EMPTY_TOKEN();
		break;
	default:
		if (isDigit(c)) {
			return Scanner_lookahead(NUMBER);
		} else if(isAlpha(c)) { // identifiers cannot begin with numbers
			return Scanner_lookahead(IDENTIFIER);
		} else {
			error("Encountered unknown token near -->'");
			log_verbose_ch(c);
			fprintf(stderr, "'\n");
			return Token(TOKEN_ERROR, -1);
			break;
		}
	}



	error("FLOW CONTROL ERROR: Reached end of addToken execution.\n");
	return Token(TOKEN_ERROR, -1);
}

void printToken(const char* prefix, Token_t t) {
	char *tokstr = Token_to_string(t);
	printf("%s%s\n", prefix, tokstr);
	free(tokstr);
}

Scanner_t InitScanner(char* source) {
	Keywords = InitKeywordsMap();
	return (Scanner_t) {
		.start = source,
		.current = source,
		.line = 1,
		.length = strlen(source),
	};
}


// @ Can trigger FATAL_ERR (and exit)
Token_t ScanNextToken() {
	Token_t newtoken;
	do {
		newtoken = Scanner_scanToken();
		if (Scanner_isAtEnd()) {
			return EOF_TOKEN();
		}
	} while(newtoken.type == TOKEN_EMPTY);
	printToken("\tParsed token:", newtoken);
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

char Scanner_peek() {
	if (Scanner_isAtEnd()) return '\0';
	return *scanner.current;
}
char Scanner_peekNext() {
	const char *scanner_end = (scanner.start + scanner.length);
	if (scanner.current + 1 >= scanner_end) return '\0';
	return *(scanner.current + 1);
}

bool Scanner_match(const char target) {
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
//	fprintf(stderr, "adding token: %d\n", t.start[0]);
	//	causing segfault if eof
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
		printf("\t[%02zu]:", i);
		printToken("", token_list->data[i]);
	}
	printf("\n");
}

void TokenList_destroy(TokenList_t* tl) {
	free(tl->data);
}

// *INDENT-OFF* - we use a lot of weird switch statements
char* TokenType_to_string(TokenType t){
	char* s_TokenType = malloc(sizeof(char) * 32);
	switch (t){
		case TOKEN_LEFT_PAREN 	: strcpy(s_TokenType, "TOKEN_LEFT_PAREN");		break;
		case TOKEN_RIGHT_PAREN	: strcpy(s_TokenType, "TOKEN_RIGHT_PAREN"); 		break;
		case TOKEN_LEFT_BRACE 	: strcpy(s_TokenType, "TOKEN_LEFT_BRACE");		break;
		case TOKEN_RIGHT_BRACE	: strcpy(s_TokenType, "TOKEN_RIGHT_BRACE"); 		break;
		case TOKEN_COMMA      	: strcpy(s_TokenType, "TOKEN_COMMA");     		break;
		case TOKEN_DOT        	: strcpy(s_TokenType, "TOKEN_DOT");       		break;
		case TOKEN_MINUS      	: strcpy(s_TokenType, "TOKEN_MINUS");     		break;
		case TOKEN_PLUS       	: strcpy(s_TokenType, "TOKEN_PLUS");      		break;
		case TOKEN_SEMICOLON  	: strcpy(s_TokenType, "TOKEN_SEMICOLON"); 		break;
		case TOKEN_STAR       	: strcpy(s_TokenType, "TOKEN_STAR");		break;
		case TOKEN_BANG         : strcpy(s_TokenType, "TOKEN_BANG");		break;
		case TOKEN_BANG_EQUAL   : strcpy(s_TokenType, "TOKEN_BANG_EQUAL");	break;
		case TOKEN_EQUAL        : strcpy(s_TokenType, "TOKEN_EQUAL");		break;
		case TOKEN_EQUAL_EQUAL  : strcpy(s_TokenType, "TOKEN_EQUAL_EQUAL");	break;
		case TOKEN_GREATER      : strcpy(s_TokenType, "TOKEN_GREATER");		break;
		case TOKEN_GREATER_EQUAL: strcpy(s_TokenType, "TOKEN_GREATER_EQUAL");	break;
		case TOKEN_LESS         : strcpy(s_TokenType, "TOKEN_LESS");		break;
		case TOKEN_LESS_EQUAL   : strcpy(s_TokenType, "TOKEN_LESS_EQUAL");	break;
		case TOKEN_IDENTIFIER   : strcpy(s_TokenType, "TOKEN_IDENTIFIER");	break;
		case TOKEN_STRING       : strcpy(s_TokenType, "TOKEN_STRING");		break;
		case TOKEN_NUMBER       : strcpy(s_TokenType, "TOKEN_NUMBER");		break;
		case TOKEN_AND          : strcpy(s_TokenType, "TOKEN_AND");		break;
		case TOKEN_CLASS        : strcpy(s_TokenType, "TOKEN_CLASS");		break;
		case TOKEN_ELSE         : strcpy(s_TokenType, "TOKEN_ELSE");		break;
		case TOKEN_FALSE        : strcpy(s_TokenType, "TOKEN_FALSE");		break;
		case TOKEN_FOR          : strcpy(s_TokenType, "TOKEN_FOR");		break;
		case TOKEN_FUN          : strcpy(s_TokenType, "TOKEN_FUN");		break;
		case TOKEN_IF           : strcpy(s_TokenType, "TOKEN_IF");		break;
		case TOKEN_NIL          : strcpy(s_TokenType, "TOKEN_NIL");		break;
		case TOKEN_OR           : strcpy(s_TokenType, "TOKEN_OR");		break;
		case TOKEN_PRINT        : strcpy(s_TokenType, "TOKEN_PRINT");		break;
		case TOKEN_RETURN       : strcpy(s_TokenType, "TOKEN_RETURN");		break;
		case TOKEN_SUPER        : strcpy(s_TokenType, "TOKEN_SUPER");		break;
		case TOKEN_THIS         : strcpy(s_TokenType, "TOKEN_THIS");		break;
		case TOKEN_TRUE         : strcpy(s_TokenType, "TOKEN_TRUE");		break;
		case TOKEN_VAR          : strcpy(s_TokenType, "TOKEN_VAR");		break;
		case TOKEN_WHILE        : strcpy(s_TokenType, "TOKEN_WHILE");		break;
		case TOKEN_SLASH	: strcpy(s_TokenType, "TOKEN_SLASH");		break;
		case TOKEN_ERROR        : strcpy(s_TokenType, "TOKEN_ERROR");		break;
		case TOKEN_EOF          : strcpy(s_TokenType, "TOKEN_EOF");		break;
		case TOKEN_EMPTY 	: strcpy(s_TokenType, "TOKEN_EMPTY");		break;
		//--------------------------------------------------------------------------//
		default			: strcpy(s_TokenType, "!!!!!!!!!!");
					printf("ERROR! to_string() on unrecognized token.\n");
					break;
	}
	return s_TokenType;
}

char* Token_to_string(Token_t t){
	const size_t scannerResultSize = 128;
	const size_t copy_buf_sz = 256;
	const bool printAddr = false;


	if (t.length > MAX_TOKEN_LEN){
		PRINTF_FATAL_ERR("Tried to print token with L>%d (L=%d)",MAX_TOKEN_LEN,t.length);
	}

	char* s_TokenType = TokenType_to_string(t.type);
	char* s_scannerResult = calloc(scannerResultSize, sizeof(char));

	if (t.length == EOF ){
		strcpy(s_scannerResult, "EOF");
	} else {
		sprintf(s_scannerResult, "'%.*s'",  t.length, t.start);
	}

	size_t TokenTypeSize = strlen(s_TokenType);
	char tabstr[8];
	strcpy(tabstr,"\t");
	if (TokenTypeSize<17){
		strcpy(tabstr,"\t\t");
	} 
	if (TokenTypeSize<10){
		strcpy(tabstr,"\t\t\t");
	}
	char* copy_buf = calloc(copy_buf_sz, sizeof(char));

	if (printAddr){
		sprintf(copy_buf, "%20s,'%s'\tL=%d, %p | %p",s_TokenType, s_scannerResult,t.length,t.start,t.start+t.length);
	} else{
		sprintf(copy_buf, "%20s,%10s\tL=%d",s_TokenType, s_scannerResult,t.length);
	}
	free(s_TokenType);
	free(s_scannerResult);

	char* ret_buf = malloc(strlen(copy_buf)+1);
	strcpy(ret_buf,copy_buf);

	free(copy_buf);
	return ret_buf;
}

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

HashTable_t InitKeywordsMap(){
	HashTable_t ht = HashTable_create();
	HashTable_put(&ht, "and",    TOKEN_AND);
	HashTable_put(&ht, "class",  TOKEN_CLASS);
	HashTable_put(&ht, "else",   TOKEN_ELSE);
	HashTable_put(&ht, "false",  TOKEN_FALSE);
	HashTable_put(&ht, "for",    TOKEN_FOR);
	HashTable_put(&ht, "fun",    TOKEN_FUN);
	HashTable_put(&ht, "if",     TOKEN_IF);
	HashTable_put(&ht, "nil",    TOKEN_NIL);
	HashTable_put(&ht, "or",     TOKEN_OR);
	HashTable_put(&ht, "print",  TOKEN_PRINT);
	HashTable_put(&ht, "return", TOKEN_RETURN);
	HashTable_put(&ht, "super",  TOKEN_SUPER);
	HashTable_put(&ht, "this",   TOKEN_THIS);
	HashTable_put(&ht, "true",   TOKEN_TRUE);
	HashTable_put(&ht, "var",    TOKEN_VAR);
	HashTable_put(&ht, "while",  TOKEN_WHILE);
	return ht;
}
void DestroyKeywordsMap(HashTable_t ht) {
	for (size_t i = 0; i < ht.capacity; i++) {
		if ((int)ht.pairs[i].value != INT_MIN) {
			free(ht.pairs[i].key);
		}
	}
	free(ht.pairs);
}
