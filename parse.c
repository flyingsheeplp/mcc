#include <stdio.h>
#include "global.h"
#include "log.h"

static MCC_TOKEN cur;

static MCC_TOKEN nextToken()
{
    cur = scanToken();
}

static void match(MCC_TOKEN t)
{
	if(cur == t)
		return;

	loge("except %s at line:%d[%d]",kwArray[t],line,pos);
}

struct AstNode* parse()
{
    struct AstNode* programAst = translationUnit();
}

struct AstNode* translationUnit()
{
    struct AstNode* n = externalDecl();
    struct AstNode* s = n;
    while(cur != TK_EOF){
        s->sib = externalDecl();
        s = s->sib;
    }

    return n;
}
