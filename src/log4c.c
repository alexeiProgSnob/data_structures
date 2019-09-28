#include "log4c.h"
#include "hash.h"
#include <stdio.h> 	/*< FILE >*/
#include <string.h>	/*< strcmp >*/
#include <stdlib.h>	/*< calloc >*/
#include <stdarg.h>	/*< va_start >*/
#include <time.h>	/*< time_t >*/
#include <pthread.h>/*< mutex >*/

#define MAXBUFF (1024)
#define CONVERT(LOG_LEVEL) { #LOG_LEVEL, LOG_LEVEL }

HashMap* g_map;

struct _Zlog{
    FILE* m_fp;					/*< log file>*/
    log_level_enum m_log_level; /*< log level >*/
    pthread_mutex_t m_mutex;	/*< for thread safe logger >*/
    char m_modual[MAXBUFF];		/*< hash key >*/
};

typedef struct _str_to_level_conver{
	const char* m_name;
	log_level_enum m_log_level;
}str_to_level_conver;

typedef struct _log_data
{
    log_level_enum m_level;     /*< Log Level >*/
    char m_modual[MAXBUFF];		/*< hash key >*/
    char m_path[MAXBUFF];      	/*< Log file path >*/
    char m_file_name[MAXBUFF];  /*< Log file name >*/
}log_data;

str_to_level_conver pairs[] = {
		{"LOG_TRACE",LOG_TRACE},
		{"LOG_DEBUG",LOG_DEBUG},
		{"LOG_INFO",LOG_INFO},
		{"LOG_WARNING",LOG_WARNING},
		{"LOG_ERROR",LOG_ERROR},
		{"LOG_CRITICAL",LOG_CRITICAL},
		{"LOG_SEVERE",LOG_SEVERE},
		{"LOG_FATAL",LOG_FATAL},
		{"LOG_NONE",LOG_NONE},
};

static log_level_enum StrToEnum (const char *_str){
    int i;
    int ConvertCount = sizeof(pairs)/sizeof(str_to_level_conver);

    for(i = 0 ; i < ConvertCount ; ++i){
        if(strcmp(_str,pairs[i].m_name) == 0){
            return pairs[i].m_log_level;
        }
    }
    return LOG_ERROR;
}

static FILE* CheckConfigFile(const char* _file_name){
	char fline[MAXBUFF];
	size_t lineSize;
	FILE* fp = fopen(_file_name,"r");
	if(fp == NULL){
		return NULL;
	}

	fgets(fline,MAXBUFF,fp);
	lineSize = strlen(fline);
	fline[lineSize - 1] = '\0';

	if(strcmp(fline,"[#]") != 0){
		fclose(fp);
		return NULL;
	}
	return fp;
}


static char* GetModual(char* _line,char* _modual){
    const char s[] = "[]/,?.: \n";
    char *token;

    token = strtok(_line, s);
    strcpy(_modual,token);

    while( token != NULL ){
        strcpy(_modual,token);
        token = strtok(NULL, s);
    }
    return _modual;
}

static void GetLineData(log_data* _log,char* _line){
    const char s[4] = " =\n";
    char *token;

    token = strtok(_line, s);
    if(strcmp(token,"Path") == 0){
        token = strtok(NULL,s);
        strcpy(_log->m_path,token);
    }
    else if (strcmp(token,"Level") == 0){
        token = strtok(NULL,s);
        _log->m_level = StrToEnum(token);
    }
    else if(strcmp(token,"File") == 0){
        token = strtok(NULL,s);
        strcpy(_log->m_file_name,token);
    }
    else{
        return;
    }
}

static int InsertLogToHash(struct _Zlog* _log, log_data* _data_log){
	if(pthread_mutex_init(&_log->m_mutex, NULL) !=0){
		return -1;
	}
	_log->m_log_level = _data_log->m_level;
	_log->m_fp = fopen(_data_log->m_file_name,"a");
	if( HashMapInsert(g_map, _log->m_modual, _log) != MAP_SUCCESS){
		return -1;
	}
	return 0;
}

static void ReadData(FILE* _fp){
	log_data data_log;
	struct _Zlog* log = NULL;
	char c;
    char fline[MAXBUFF] = {0};
    char tempStr[MAXBUFF] = {0};
    rewind(_fp);
	while(fgets(fline,MAXBUFF,_fp) != NULL){
		c = fline[0];
		if(c == '['){
			if(log != NULL){
				strcpy(tempStr,log->m_modual);
				if( InsertLogToHash(log,&data_log) != 0){
					free(log);
				}
			}
			log = calloc(1,sizeof(struct _Zlog));
			data_log.m_level = LOG_ERROR;
			strcpy(data_log.m_path,tempStr);
			GetModual(fline,tempStr);
			strcpy(log->m_modual, tempStr);
		}
		else{
			if(c != '\n'){
				GetLineData(&data_log,fline);
			}
		}
	}
	if( InsertLogToHash(log,&data_log) != 0){
		free(log);
	}
}




/******************* HASH FUNCTIONS *******************/
static size_t HashFunc(const void* _key){
	const char* str = _key;
	return (size_t)(*str)%23;
}

static int EqualFunc(const void* _first_key, const void* _second_key){
    const char* key_one = _first_key;
    const char* key_two = _second_key;
    if(strcmp(key_one,key_two) == 0){
        return 1;
    }
    return 0;
}


static void freeLogs(void* _value){
    struct _Zlog* log = _value;
    pthread_mutex_destroy(&log->m_mutex);
    fclose(log->m_fp);
    free(log);
}


static void freeModual(void* _value){
}

static void DestroyLog(){
    HashMapDestroy(&(g_map), freeModual, freeLogs);
}
/******************* LOG FUNCTIONS *******************/
void zlog_init(const char* _file_name){
	FILE* fp;
	atexit(DestroyLog);
	if(_file_name == NULL){
		return;
	}
	fp = CheckConfigFile(_file_name);
	g_map = HashMapCreate(23,HashFunc,EqualFunc);
	if(g_map == NULL){
		fclose(fp);
		return;
	}
	ReadData(fp);
	fclose(fp);
	return;
}

Zlog zlog_get(const char* _modual){
    Zlog log;
    Map_Result res;
    res = HashMapFind(g_map, _modual,(void**) &log);
    if(res == MAP_SUCCESS){
        return log;
    }
    HashMapFind(g_map, "#",(void**) &log);
    return log;
}

void PrintToFile(Zlog _log , log_level_enum _lLevel ,const char* _file,int _line ,char* fmt, ...){
    va_list ap;
    time_t current_time;
	char* c_time_string;
    struct _Zlog* log =(struct _Zlog*) _log;

    if(_lLevel >= log->m_log_level && log->m_fp != NULL){
		current_time = time(NULL);
		c_time_string = ctime(&current_time);
		c_time_string[strlen(c_time_string) - 1] = '@';
		va_start(ap, fmt);
    	pthread_mutex_lock(&log->m_mutex);
		fputs(c_time_string,log->m_fp);
		fprintf(log->m_fp,"%s %d ",_file,_line);
		vfprintf(log->m_fp, fmt,ap);
		pthread_mutex_unlock(&log->m_mutex);
		va_end(ap);
    }

}