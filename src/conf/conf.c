#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "ini.h"
#include "pmao_core.h"
#include "log.h"
#include "util.h"

/*
 * conf.c
 *
 *  Created on: 2012-6-5
 *      Author: pmserver
 */

#define MAX_VALUE_LEN 2048

typedef struct
{
    char* section;
    char* name;
    char* value;
    int size;
} configuration;

static int handler(void* user, const char* section, const char* name,
                   const char* value)
{
    configuration* pconfig = (configuration*)user;

    #define MATCH(s, n) (strcasecmp(section, s) == 0 && strcasecmp(name, n) == 0)

    if( MATCH(pconfig->section, pconfig->name) ){
    	strncpy(pconfig->value, value,pconfig->size);
    	pconfig->value[pconfig->size] = '\0';
    	return 1;
    }
//    if (MATCH("protocol", "version")) {
//        pconfig->version = atoi(value);
//    } else if (MATCH("user", "name")) {
//        pconfig->name = strdup(value);
//    } else if (MATCH("user", "email")) {
//        pconfig->email = strdup(value);
//    } else {
//        return 0;  /* unknown section/name, error */
//    }
//    return 1;
    return 0;
}


/**
 * the size parameter don't contain the null character. so if size of value is 64, you should make 64-1=63 here!
 */
char* conf_getstr(const char* section, const char* name, char* value, int size, const char* defValue)
{
	configuration conf;
	conf.section = (char*)section;
	conf.name = (char*)name;
	conf.value = value;
	conf.size = size-1;   //here minus one to avoid error!

	if( NULL == value ){
		return NULL;
	}

	if( !ini_parse(g_var.pConfFile, handler, &conf) ){
		strcpy(value,defValue);  //make default value!
	}

	return value;
}

/**
 *
 */
int conf_getint(const char* section, const char* name, int defValue)
{
	char value[MAX_VALUE_LEN] ;
	int v = defValue;

	if( conf_getstr(section,name,value,MAX_VALUE_LEN-1, NULL) != NULL ){
		v = atoi(value);
	}

	return v;
}

/**
 *
 */
double conf_getdouble(const char* section, const char* name, double defValue)
{
	char value[MAX_VALUE_LEN] ;
	double v = defValue;

	if( conf_getstr(section,name,value,MAX_VALUE_LEN-1, NULL) != NULL ){
		v = atof(value);
	}

	return v;
}

/**
 * value of '0' means false and return 0, otherwise return 1.
 */
int conf_getbool(const char* section, const char* name, int defValue)
{
	char value[MAX_VALUE_LEN] ;
	int v = defValue;

	if( conf_getstr(section,name,value,MAX_VALUE_LEN-1, NULL) != NULL ){
		if( (value[0] != '\0') && (value[0] != '0') ) value[0] = '9';
		v = atoi(value);
		v = (v!=0);
	}

	return v;
}

/**
 *
 */
int conf_init()
{
	char value[MAX_VALUE_LEN];

	//Get working directory and config file directory
	//getcwd(g_var.pCurDir,sizeof(g_var.pCurDir)-1);
	get_cur_dir(g_var.pCurDir, sizeof(g_var.pCurDir)-1);

	if( g_var.pCurDir[strlen(g_var.pCurDir)-1] != '/' ){
		g_var.pCurDir[strlen(g_var.pCurDir)] = '/';
		g_var.pCurDir[strlen(g_var.pCurDir) + 1 ] = '\0';
	}

	//config file
	strcpy(g_var.pConfFile, g_var.pCurDir);
	strcat(g_var.pConfFile, PMAO_CONF_FILE);

	//log file
	strcpy(g_var.pLogFile, g_var.pCurDir);
	strcat(g_var.pLogFile, PMAO_LOG_FILE);

	//get log level
	if( conf_getstr("app","file_log_level", value,MAX_VALUE_LEN-1, NULL) != NULL ){
		g_var.file_log_level = log_getlevel_from_str(value);
	} else {
		g_var.file_log_level = PRIORITY_ERROR ; //default log level
	}
	if( conf_getstr("app","console_log_level", value,MAX_VALUE_LEN-1, NULL) != NULL ){
		g_var.console_log_level = log_getlevel_from_str(value);
	} else {
		g_var.console_log_level = PRIORITY_DEBUG ; //default log level
	}

	//mkdir of log directory
	if( mkdir( util_get_basepath(g_var.pLogFile, value, MAX_VALUE_LEN-1) , S_IRWXU ) ){
		if( errno != EEXIST ){
			LOG_ERROR("Cann't mkdir log directory! log_file = %s ", g_var.pLogFile);
		}
	}

	LOG_INFO("ConfFile = %s ", g_var.pConfFile);
	LOG_INFO("LogFile  = %s ", g_var.pLogFile);

	return 0;
}
