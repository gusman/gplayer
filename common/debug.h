#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>

#ifdef __DEBUG__
#define MSG_ERR(x, args...)	printf("\033[1;41m[ERR:]\033[0m " x, args)	
#define DBG_PRINT(x, args...)	printf("DEBUG: " x, args)
#else
#define MSG_ERR(x)	
#define DBG_PRINT(x)	
#endif

#endif /* __DEBUG_H__ */
