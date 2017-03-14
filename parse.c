#include <stdio.h>
#include "global.h"
#include "log.h"

static MCC_TOKEN cur;

static void match(MCC_TOKEN t)
{
	if(cur == t)
		return;

	loge("except %s at line:%d[%d]",kwArray[t],line,pos);
}


