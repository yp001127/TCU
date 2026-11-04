#ifndef __CONFIG_H
#define __CONFIG_H

#include "sys.h"
#include "stdio.h"
#include "usart.h"
#include "string.h"


char debug_buf[150];
#define DEBUG_LOG 1

#if DEBUG_LOG
	#define LOG(format, ...)                         \
	                                                 \
	do 																	             \
	{                                                \
		  sprintf(debug_buf, format, ##__VA_ARGS__) ;   \
			printf("%s", debug_buf);                      \
			                                             \
	}while(0)

#else 
	
	#define LOG(format, ...)  void(0)
	
#endif

#endif
