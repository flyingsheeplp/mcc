#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include "log.h"

#define advance nextToken

static MCC_TOKEN cur;

static MCC_TOKEN nextToken()
{
    cur = scanToken();

    return cur;
}

static void match(MCC_TOKEN t)
{
	if(cur == t){
		advance();
		return;
	}

	printf("\nexcept %s at line:%d[%d]\n",kwArray[t],line,pos);
	exit(1);
}

static void typeSpec()
{
	advance();
	match(KW_INT);

	printf("\nCUR in typeSpec:[%s]\n",kwArray[cur]);
}

static void declarator()
{
	match(TK_ID);

	printf("\nCUR in declarator:[%s]\n",kwArray[cur]);
}

static void initializer()
{
	printf("in initializer\n");
	advance();
	match(TK_CINT);
}

void compoundStmt()
{
	while(cur != TK_RBRACE){
		advance();
		if(cur == TK_EOF)
			printf("compoundStmt error\n");
	}
}


struct AstNode* externalDecl()
{
	struct AstNode* e = malloc(sizeof(struct AstNode));

	typeSpec();

	while(cur != TK_EOF){
		declarator();

		if(cur == TK_LBRACE){
		 	compoundStmt();
		 	break;
		}else if(cur == TK_ASSIGN){
			printf("ASSIGN\n");
			initializer();
		}else if(cur == TK_COMMA){
			advance();
		}else if(cur == TK_SEMICOLON){
			advance();
			break;
		}else if(cur == TK_EOF){
			printf("externalDecl error\n");
		}
	}

	return e;
}

struct AstNode* translationUnit()
{
    struct AstNode* n = externalDecl();
    struct AstNode* s = n;
    while(cur != TK_EOF){
        s->next = externalDecl();
        s = s->next;
    }

    return n;
}

struct AstNode* parse()
{
	printf("line:%d pos:%d\n",line,pos);
    struct AstNode* programAst = translationUnit();
    printf("parse success!\n");
}




