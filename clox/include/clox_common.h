#ifndef CLOX_COMMON_H
#define CLOX_COMMON_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define _DEBUG
void LOG_VERBOSE_STR(const char* fmt, ...);
void LOG_VERBOSE_CH(const char c);
void LOG_DEFAULT_STR(const char* fmt, ...);

#define PRINTF_FATAL_ERR(fmt, ...)do{\
	fprintf(stderr,fmt, ##__VA_ARGS__);\
	fprintf(stderr,"\nexiting...\n");\
	exit(EXIT_FAILURE);\
	}while(0)

#define error(fmt, ...)do{\
	fprintf(stderr,"ERROR [line %d]: ", scanner.line);\
	fprintf(stderr,fmt "", ##__VA_ARGS__);\
	}while(0)

#define MAX_LINE_LEN	1024

// Prints invisible chars like \n, \c as string literals
char *printVerboseChar(const char c);

#endif // COMMON_H

