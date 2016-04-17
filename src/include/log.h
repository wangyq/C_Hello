/*
 * log.h
 *
 *  Created on: 2012-6-5
 *      Author: wangyq
 */

#ifndef LOG_H_
#define LOG_H_

/*!
* 调试级别的枚举类型
*/
enum LOG_LEVEL{
	PRIORITY_FATAL = 000,    /*!< 此调试信息为致命错误，程序可能会退出 */
	PRIORITY_ERROR = 100,    /*!< 此调试信息为普通错误 */
	PRIORITY_WARN  = 200,    /*!<  */
	PRIORITY_INFO  = 300,    /*!< 此调试信息为打印出的变量值等内容信息 */
	PRIORITY_DEBUG = 400,    /*!< 此调试信息为打印出的接收到的XML数据内容 */
	PRIORITY_TRACE = 500     /* TRACE INFO */
};


void log_print(int level , const char* src_file, int line, const char* fmt, ...);
int log_getlevel_from_str(const char* msg);

#define LOG_FATAL(args... ) log_print(PRIORITY_FATAL,__FILE__, __LINE__,  ##args)
#define LOG_ERROR(args... ) log_print(PRIORITY_ERROR,__FILE__, __LINE__,  ##args)
#define LOG_WARN(args... ) log_print(PRIORITY_WARN,__FILE__, __LINE__,    ##args)
#define LOG_INFO(args... ) log_print(PRIORITY_INFO,__FILE__, __LINE__,    ##args)
#define LOG_DEBUG(args... ) log_print(PRIORITY_DEBUG,__FILE__, __LINE__,  ##args)
#define LOG_TRACE(args... ) log_print(PRIORITY_TRACE,__FILE__, __LINE__,  ##args)

#endif /* LOG_H_ */
