#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>

/* Always show error message */
#define DBG_ERR(str, args...)	printf("\033[1;41m[ERR:]\033[0m " str, ##args)	

#ifdef __DEBUG__
#define DBG_PRINT(str, args...)	printf("DEBUG: " str, ##args)
#define DBG_TRACE(str, args...) printf("\033[1;42m[%s][%s][%d]\033[0m\n\t" str, \
					__FILE__, __FUNCTION__, __LINE__, ##args)
#else
#define DBG_PRINT(str)
#define DBG_TRACE(str)
#endif

#endif /* __DEBUG_H__ */
