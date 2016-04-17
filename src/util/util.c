#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "util.h"

/*
 * util.c
 *
 *  Created on: 2012-6-5
 *      Author: pmserver
 */

/**
 *
 */
char* get_cur_dir(char* buff, int size)
{
	char* index;

	int rslt = readlink("/proc/self/exe", buff, size - 1);
	if (rslt<0 || rslt >= (size-1) ) {
		DPRINT("READ /proc/self/exe ERROR!", buff);
		return NULL;
	}
	//DPRINT("/proc/self/exe=%s", buff);

	index = rindex(buff, '/');
	if (NULL == index) {
		index = rindex(buff, '\\');
	}
	//
	if (NULL != index) { //not found!
		*++index = '\0';
	}
	return buff;
}

/**
 *
 */
char * util_get_curtime(char* buff, int size) {
	time_t ts;
	struct tm *ptime;

	if (NULL == buff)
		return NULL;

	ts = time(NULL);
	ptime = localtime(&ts);

	sprintf(buff, "%4d-%02d-%02d %02d:%02d:%02d", ptime->tm_year + 1900,
			ptime->tm_mon + 1, ptime->tm_mday, ptime->tm_hour, ptime->tm_min,
			ptime->tm_sec);

	return buff;
}

/**
 *
 */
char* util_get_basepath(const char* filepath, char* buff, int size) {
	char* index = rindex(filepath, '/');
	if (NULL == index) {
		index = rindex(filepath, '\\');
	}

	//
	if (NULL == index) { //not found!
		strcpy(buff, ".");
	} else {
		const char* p = filepath;
		char* q = buff;
		while (p < index)
			*q++ = *p++;
		*q = '\0';
	}
	return buff;
}
