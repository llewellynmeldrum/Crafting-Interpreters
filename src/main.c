#include <stdio.h>
#include <stdlib.h>

#include "clox_common.h"
#include "scanner.h"
#define PROG "clox"


/* Files.c/h */
static void runFile(const char* filename);
static void runPrompt();

int main(int argc, char** argv) {
	sc_current_line = 1;
	if (argc > 2) {
		fprintf(stderr, "Error! Usage: %s [script] | %s\n", PROG, PROG);
		exit(EXIT_FAILURE);
	} else if (argc == 2) {
		runFile(argv[1]);
	} else if (argc == 1) {
		runPrompt();
	}
}
#define TOKENS_INIT_CAPACITY 8

void resize_tokens(Token* tokens, size_t tokens_capacity) {
	tokens_capacity *= 2;
	char *resized = realloc(tokens, sizeof(Token) * tokens_capacity);
	if (!resized) {
		free(tokens);
		PRINTF_FATAL_ERR("Failed to realloc tokens arr.\n");
	}
}

void run(char* input) {
	printf("%s\n", input);
	Token t;
	size_t count = 0;
	size_t tokens_capacity = TOKENS_INIT_CAPACITY;
	Token * tokens = malloc(sizeof(Token) * tokens_capacity);
	while ((t = ScanNextToken(input) ).type != TOKEN_EOF) {
		if (count >= tokens_capacity) {
			resize_tokens(tokens, tokens_capacity);
		}
		tokens[count++] = t;
	}
}


void runFile(const char* filename) {
	printf("ENTERED FILE MODE:\n");
	char *input_bytes = readFile(filename);
	run(input_bytes);
	free(input_bytes);
}

void runPrompt() {
	printf("ENTERED PROMPT MODE:\n");
	char line[MAX_LINE_LEN];
	while(1) {
		printf("> ");
		if (!fgets(line, sizeof(line), stdin)) break;
		run(line);
	}
}

