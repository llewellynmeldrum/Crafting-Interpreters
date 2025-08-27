#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clox_common.h"
#include "scanner.h"
#define PROG "clox"


/* Files.c/h */
static void runFile(const char* filename);
static void runPrompt();

int main(int argc, char** argv) {
	if (argc > 2) {
		fprintf(stderr, "Error! Usage: %s [script] | %s\n", PROG, PROG);
		exit(EXIT_FAILURE);
	} else if (argc == 2) {
		runFile(argv[1]);
	} else if (argc == 1) {
		runPrompt();
	}
}


#define TOKEN_LIST_INIT_CAPACITY 8
typedef struct TokenList_t {
	Token *data;
	size_t capacity;
	size_t count;
} TokenList_t;

void resize_tokens(TokenList_t* tl) {
	fprintf(stderr, "Resizing tokenlist.\n");
	tl->capacity *= 2;
	void *resized = realloc(tl->data, sizeof(Token) * tl->capacity);
	if (!resized) {
		free(tl->data);
		PRINTF_FATAL_ERR("Failed to realloc tokens arr.\n");
	}
}

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
void run(char* source) {
	scanner = Scanner(source);
	Token t;
	TokenList_t tok_list = TokenList();
	while ((t = ScanNextToken() ).type != TOKEN_EOF) {
		TokenList_add(&tok_list, t);
	}
}


void runFile(const char* filename) {
	printf("ENTERED FILE MODE:\n");
	char *input_bytes = readFile(filename);
	run(input_bytes);
	free(input_bytes);
}

// *INDENT-OFF*
char *printVerboseChar(const char c) {
	char *res = malloc(sizeof(char) * 10);
	if (!res) PRINTF_FATAL_ERR("Failed to alloc for vchar.\n")
	switch (c) {
	case '\0': strcpy(res,"\\0"); break;
	case '\n': strcpy(res,"\\n"); break;
	case '\r': strcpy(res,"\\r"); break;
	case '\t': strcpy(res,"\\t"); break;
	case ' ' : strcpy(res,"\\."); break;

	default: 
		res[0] = c; res[1] = '\0';
		break;
	}
	return res;
}
// *INDENT-ON*

#define DEBUG_SWS
#undef DEBUG_SWS
void stripWhiteSpace(char *s) {

#ifdef DEBUG_SWS
	char *vchar;
	printf("trimming:'%s'\n", s);
	for (int i = 0; i < strlen(s); i++) {
		vchar = printVerboseChar(s[i]);
		printf("input[%d]='%s'\n", i, vchar);
		free(vchar);
	}
#endif

	int length = strlen(s);
	char *start = s;
	char *end = s + length - 1;
	while (end > start) {
		if (*end == '\0') break;
		if (*end == '\n' || *end == ' ') {
			*end = '\0';
			end--;
		} else {
			break;
		}
#ifdef DEBUG_SWS
		printf("end[%d]='%s'\n", 0, printVerboseChar(end[0]));
	}
	for (int i = 0; i < length; i++) {
		vchar = printVerboseChar(s[i]);
		printf("input[%d]='%s'\n", i, vchar);
		free(vchar);
#endif
	}
}

void runPrompt() {
	printf("ENTERED PROMPT MODE:\n");
	char line[MAX_LINE_LEN];
	while(1) {
		printf("> ");
		if (!fgets(line, sizeof(line), stdin)) break;
		stripWhiteSpace(line);
		run(line);
	}
}

