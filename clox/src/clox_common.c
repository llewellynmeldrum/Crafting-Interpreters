#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "clox_common.h"
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
