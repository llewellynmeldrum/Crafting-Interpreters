#include "scanner.h"
#include "clox_common.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

size_t SYSCALL_FILESIZE(const char* filename) {
	struct stat st;
	stat(filename, &st);
	return (size_t)(st.st_size);
}

char *readFile(const char* filename) {
	if (!filename || filename[0]=='\0') PRINTF_FATAL_ERR("Filename passed incorrectly to readFile().");
	size_t filesize = SYSCALL_FILESIZE(filename);

	FILE* inputFile = fopen(filename, "r");
	if (!inputFile) PRINTF_FATAL_ERR("Unable to open file %s.", filename);

	char *buf = malloc(filesize * sizeof(char));
	if (!buf) PRINTF_FATAL_ERR("Failed to allocate buffer for file %s.",filename);
	size_t bytesRead = fread(buf, sizeof(char), filesize, inputFile);
	if (bytesRead!=filesize) PRINTF_FATAL_ERR("Failed to read %s, read (%zu/%zu)B.",filename,bytesRead,filesize);
	fclose(inputFile);
	return buf;
}

// advances current, creates and returns token.
Token addToken(const char* tok_str) {
	if (!tok_str) return (Token) {
		TOKEN_EOF, NULL, NULL, NULL
	}; // end of string reached

	size_t TokenLength;
	if (tok_str[1]=='\0') TokenLength = 1;
	else TokenLength = INT_MAX; // temporary


	switch (TokenLength) {
	case 1:
		char ch = tok_str[0];
		switch (ch) {
		case '(':
			return (Token) {
				TOKEN_LEFT_PAREN,tok_str, 1, sc_current_line
			};
			break;
		case ')':
			return (Token) {
				TOKEN_RIGHT_PAREN,tok_str, 1, sc_current_line
			};
			break;
		case '{':
			return (Token) {
				TOKEN_LEFT_BRACE,tok_str, 1, sc_current_line
			};
			break;
		case '}':
			return (Token) {
				TOKEN_RIGHT_BRACE,tok_str, 1, sc_current_line
			};
			break;
		case ',':
			return (Token) {
				TOKEN_COMMA	,tok_str, 1, sc_current_line
			};
			break;
		case '.':
			return (Token) {
				TOKEN_DOT	,tok_str, 1, sc_current_line
			};
			break;
		case '-':
			return (Token) {
				TOKEN_MINUS	,tok_str, 1, sc_current_line
			};
			break;
		case '+':
			return (Token) {
				TOKEN_PLUS	,tok_str, 1, sc_current_line
			};
			break;
		case ';':
			return (Token) {
				TOKEN_SEMICOLON,tok_str, 1, sc_current_line
			};
			break;
		case '*':
			return (Token) {
				TOKEN_STAR	,tok_str, 1, sc_current_line
			};
			break;
		default:
			PRINTF_ERR_LN(sc_current_line, "Encountered unknown token --> '%c'\n", ch);
			break;
		}
	default:
		return (Token) {};
		break;
	}
}

Token ScanNextToken(char* current) {
	// test delimiter = ' '
	const char *delim = " ";
	char *tok_str = strtok(current, delim);
	return addToken(tok_str);
}
