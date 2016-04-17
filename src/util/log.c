/*
 * log.c
 *
 *  Created on: 2012-6-5
 *      Author: pmsserver
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

#include "pmao_core.h"
#include "log.h"
#include "util.h"


static unsigned long log_count = 0;
static pthread_mutex_t mutex_log;

int log_getlevel_from_str(const char* msg)
{
	int level = (int)PRIORITY_TRACE;
	if( strcasestr( msg, "fatal") != NULL ){
		level = (int)PRIORITY_FATAL;
	} else if( strcasestr( msg, "error") != NULL ){
		level = (int)PRIORITY_ERROR;
	} else if( strcasestr( msg, "warn") != NULL ){
		level = (int)PRIORITY_WARN;
	} else if( strcasestr( msg, "info") != NULL ){
		level = (int)PRIORITY_INFO;
	} else if( strcasestr( msg, "debug") != NULL ){
		level = (int)PRIORITY_DEBUG;
	} else if( strcasestr( msg, "trace") != NULL ){
		level = (int)PRIORITY_TRACE;
	}

	return level;
}
/**
 *
 */
const char* log_getlevel_str(int level){
	static const char* level_str[] ={"FATAL","ERROR", "WARN", "INFO", "DEBUG", "TRACE"};
	return level_str[level/100];
}

/**
 *
 */
void log_print(int level , const char* src_file, int line, const char* fmt, ...)
{
	int err_num = errno;

	va_list ap;
	char cur_time[32] = {0};;

	const char* log_file = g_var.pLogFile;
	int file_log_level = g_var.file_log_level;
	int console_log_level = g_var.console_log_level;

	log_count ++;
	util_get_curtime(cur_time,31); //cur time!

	pthread_mutex_lock(&mutex_log);

	va_start(ap, fmt);

	if( level <= console_log_level ){
		fprintf(stderr, "%ld [%s %s(%d)] %s : ", log_count, cur_time, src_file, line,log_getlevel_str(level));
		vfprintf(stderr, fmt, ap);
		fprintf(stderr,"\n");

		if( level <= PRIORITY_ERROR ){
			fprintf(stderr,"\terrno = [%d], %s \n",err_num, strerror(err_num));
		}
	}
	if( level <= file_log_level ){
		FILE * file = fopen(log_file, "a");
		if( file ){
			fprintf(file, "%ld [%s  %s(%d)=]%s : ", log_count, cur_time, src_file, line, log_getlevel_str(level));
			vfprintf(file, fmt, ap);
			fprintf(file,"\n");
			if( level <= PRIORITY_ERROR ){
				fprintf(file,"\terrno = [%d], %s \n",err_num, strerror(err_num));
			}
			fclose(file);
		}
	}
	va_end(ap);

	pthread_mutex_unlock(&mutex_log);
}

