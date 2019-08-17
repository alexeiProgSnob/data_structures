#ifndef LOG4C_H_
#define LOG4C_H_

/**
 *  @file zlog4c.h
 *  @brief
 *
 *  @detailsa
 *
 *
 *  @author Author Alexei Radashkovsky (alexeirada@gmail.com)
 *  @Update by Alexei Radashkovsky on 30/09/17
 *  @bug no bugs.
 */

typedef struct _ZLog* Zlog;

typedef enum _log_level_enum{
	LOG_TRACE = 0,
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR,
	LOG_CRITICAL,
	LOG_SEVERE,
	LOG_FATAL,
	LOG_NONE
}log_level_enum;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvariadic-macros"

void PrintToFile(Zlog _log , log_level_enum _lLevel ,const char* _file,int _line ,char* fmt, ...);
#define ZLOG(log,lLevel,fmt,...)\
do{\
 PrintToFile(log,lLevel,__FILE__,__LINE__,fmt,##__VA_ARGS__);\
}while(0);\

#pragma GCC diagnostic pop

/**
 * @brief initialized the config file for read data
 *
 * @param _fileName : file name of config file
 * @return none zero if the file is not opened
 */
void zlog_init(const char* _fileName);


/**
 * @brief initialized the zlog data
 *
 * @param _modual : mudual to log
 * @
 */
Zlog zlog_get(const char* _modual);

#endif /* LOG4C_H_ */