/*
 * conf.h
 *
 *  Created on: 2012-6-5
 *      Author: wangyq
 */

#ifndef CONF_H_
#define CONF_H_

char* conf_getstr(const char* section, const char* name, char* valuenew, int size, const char* defValue);
int conf_getint(const char* section, const char* name, int defValue);
double conf_getdouble(const char* section, const char* name, double defValue);
int conf_getbool(const char* section, const char* name, int defValue);

int conf_init();

#endif /* CONF_H_ */
