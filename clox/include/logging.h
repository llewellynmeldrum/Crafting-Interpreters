#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>
#include "clox_common.h"

#define NO_OP() do{ }while(0)



#define _DEBUG
// for scanner.c
#ifdef _DEBUG
	#define log_verbose(fmt, ...)		LOG_VERBOSE_STR(fmt, ##__VA_ARGS__)
	#define log_verbose_ch(fmt, ...)	LOG_VERBOSE_CH(fmt, ##__VA_ARGS__)
	#define log(fmt, ...)			LOG_DEFAULT_STR(fmt, ##__VA_ARGS__)
	#define logln()				fprintf(stderr, "\n");
#else
	#define log_verbose(fmt, ...)	NO_OP()
	#define log_verbose_ch()	NO_OP()
	#define log(fmt, ...)		NO_OP()
	#define logln()			NO_OP()
#endif


// for main.c




#endif //LOGGING_H
