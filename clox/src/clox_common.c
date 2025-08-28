#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "clox_common.h"

#define ASCII_BUF_SZ 128
#define UTF8_BUF_SZ 256

#define UTF8_SP u8"\u2423" // OPEN BOX

char *ascii_to_utf8(const char c) {
	const int max_utf8_sz = 4;
	char chbuf[2];
	chbuf[0] = c;
	chbuf[1] = '\0';

	char *utf8_buf = calloc(max_utf8_sz + 1, sizeof(char));
	switch (c) {
	case ' ':
		strcpy(utf8_buf, UTF8_SP);
		break;
	default:
		strcpy(utf8_buf, chbuf);
		break;
	}

	return utf8_buf;
}

void LOG_VERBOSE_CH(const char c) {
	char *w_str = ascii_to_utf8(c);
	fprintf(stderr, "%s", w_str);
	free(w_str);
}

void LOG_VERBOSE_STR(const char* fmt, ...) {
	char str[ASCII_BUF_SZ];
	va_list args;
	va_start(args, fmt);
	vsnprintf(str, ASCII_BUF_SZ, fmt, args);
	va_end(args);


	int length = strnlen(str, ASCII_BUF_SZ);

	char w_str[ASCII_BUF_SZ * 2];
	char *wch;
	for (int i = 0; i < length; i++) {
		wch = ascii_to_utf8(str[i]);
		strcat(w_str, wch);
		free(wch);
	}
	fprintf(stderr, "'%s'", w_str);
}

void LOG_DEFAULT_STR(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
}


// *INDENT-OFF*
// DOESNT ACTUALLY PRINT, RETURNS BUFFER
char *printVerboseChar(const char c) {
	char *res = malloc(sizeof(char) * 10);
	if (!res) PRINTF_FATAL_ERR("Failed to alloc for vchar.\n");
	switch (c) {
	case '\0': strcpy(res,"\\0"); break;
	case '\n': strcpy(res,"\\n"); break;
	case '\r': strcpy(res,"\\r"); break;
	case '\t': strcpy(res,"\\t"); break;
	case ' ' : strcpy(res,"."); break;

	default: 
		res[0] = c; res[1] = '\0';
		break;
	}
	return res;
}
// *INDENT-ON*
