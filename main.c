/*
*    Micro C compiler
*    Copyright(c) flyingsheep.lp@gmail.com
*
*/

#include <stdio.h>
#include "global.h"
#include "log.h"
FILE* f = NULL;

static void usage(){
	printf("mcc [filename]\n");
}


int main(int argc, char** argv)
{
	if(argc != 2){
		usage();
		return 0;
	}

	f = fopen(argv[1],"r+");

	if(!f)
	{
		loge("no such file:%s",argv[1]);
		return -1;
	}


	parse();

	return 0;
}