/*
*    Micro C compiler
*    Copyright(c) flyingsheep.lp@gmail.com
*
*/

#include <stdarg.h>
#include <stdio.h>
#include "log.h"

void logd(char* fmt,...)
{
	va_list arg;
	va_start(arg,fmt);
	printf(fmt,arg);
	va_end(arg);
	printf("\n");
}

void loge(char* fmt,...)
{
	va_list arg;
	va_start(arg,fmt);
	fprintf(stderr,fmt,arg);
	va_end(arg);
	fprintf(stderr,"\n");
}