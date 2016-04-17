/*
 * util.h
 *
 *  Created on: 2012-6-5
 *      Author: wangyq
 */

#ifndef UTIL_H_
#define UTIL_H_


void dprintf_impl(const char* file, int line, int enable, const char* fmt, ...);

//DPRINT definition!
#ifndef NDEBUG
#define DPRINT( args...) \
           dprintf_impl(__FILE__, __LINE__, 1, ##args)
#else
#define DPRINT( args...) // define to nothing in release mode
#endif

char* get_cur_dir(char* buff, int size);
char * util_get_curtime(char* buff, int size);
char* util_get_basepath(const char* filepath, char* buff, int size);

#endif /* UTIL_H_ */
