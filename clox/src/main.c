#include <limits.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clox_common.h"
#include "logging.h"
#include "scanner.h"
#include "hashtable.h"
#define PROG "clox"


/* Files.c/h */
static void runFile(const char* filename);
static void runPrompt();
void printverbstr(const char* str) {
	for (size_t i = 0; i < strlen(str); i++) {
		char *c = printVerboseChar(str[i]);
		printf("%s", c);
		free(c);
	}
}


int main(int argc, char** argv) {
#ifdef _DEBUG
	setlocale(LC_ALL, ""); // for printing wchar
#endif
	if (argc > 2) PRINTF_FATAL_ERR("Error! Usage: %s [source.lox] | %s\n", PROG, PROG);
	else if (argc == 2) 	runFile(argv[1]);
	else if (argc == 1)  	runPrompt();
}


void run(char* source) {

	printf("PHASE 1: PARSING\n");
	scanner = InitScanner(source);
	Token_t token;
	TokenList_t token_list = TokenList();

	do {
		token = ScanNextToken();

		if (token.type == TOKEN_ERROR)
			PRINTF_FATAL_ERR("ScanNextToken() returned TOKEN_ERROR token!\n");

		TokenList_add(&token_list, token);
	} while(token.type != TOKEN_EOF);
	TokenList_Print(&token_list);
	// tokens are added, with one last one for EOF

}


void runFile(const char* filename) {
	printf("ENTERED FILE MODE:\n");
	char *input_bytes = readFile(filename);
	//printverbstr(input_bytes);
	run(input_bytes);
	free(input_bytes);
}


void stripWhiteSpace(char *s) {

	/*
		char *vchar;
		printf("trimming:'%s'\n", s);
		for (int i = 0; i < strlen(s); i++) {
			vchar = printVerboseChar(s[i]);
			printf("input[%d]='%s'\n", i, vchar);
			free(vchar);
		}
	*/
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
		printf("end[%d]='%s'\n", 0, printVerboseChar(end[0]));
	}
}

void runPrompt() {
	printf("ENTERED PROMPT MODE:\n");
	char line[MAX_LINE_LEN];
	while(1) {
		printf("> ");
		if (!fgets(line, sizeof(line), stdin)) break;
		printverbstr(line);
		//stripWhiteSpace(line);
		run(line);
	}
}

