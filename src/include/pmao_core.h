/*
 * pmao.h
 *
 *  Created on: 2012-6-5
 *      Author: pmserver
 */

#ifndef PMAO_CORE_H_
#define PMAO_CORE_H_

#define S_OK 0
#define S_ERROR -1

#define MAX_CONF_FILE_LEN 256

//configuration file name!
#define PMAO_CONF_FILE "conf/pmao.conf"
#define PMAO_LOG_FILE "log/pmao.log"

/**
 *
 */
typedef struct tagS_Global{
	char pCurDir[MAX_CONF_FILE_LEN];        //working directory!
	char pConfFile[MAX_CONF_FILE_LEN];      //config file path, for example, "/srv/pmdo/pmao.conf"
	char pLogFile[MAX_CONF_FILE_LEN];       //log file path , for example: "/srv/pmao/pmao.log"

	int file_log_level ;                         //file_log_level
	int console_log_level;                       //console_log_level


}S_GLOBAL;

extern S_GLOBAL g_var;

void pmao_init(int argc, const char* argv[]);
void pmao_destroy();

#endif /* PMAO_CORE_H_ */
