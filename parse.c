#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include "log.h"

#define advance nextToken
#define skip nextToken

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

	printf("\ncur [%s] except %s at line:%d[%d]\n",kwArray[cur],kwArray[t],line,pos);
	exit(1);
}

static void structSpec(){

}

static void typeSpec(struct AstNode* n)
{
    switch(cur){
        case KW_INT:
            match(KW_INT);
            break;
        case KW_SHORT:
            match(KW_SHORT);
            break;
        case KW_CHAR:
            match(KW_CHAR);
            break;
        case KW_STRUCT:
            match(KW_STRUCT);
            structSpec();
            break;
        default:
            printf("type meets unexpected token:[%s]\n",kwArray[cur]);
            exit(1);
    }
}

static void declarator()
{
	match(TK_ID);
}

static void initializer()
{
	advance();
	match(TK_CINT);
}

static struct AstNode* compoundStmt()
{
    struct AstNode* csn = malloc(sizeof(struct AstNode));


	while(cur != TK_RBRACE){
		advance();
		if(cur == TK_EOF)
			printf("compoundStmt error\n");
	}

    return csn;
}

static void procBody(struct AstNode* parent)
{
    parent->type = PROC_DECL;
}

struct AstNode* externalDecl()
{
	struct AstNode* e = malloc(sizeof(struct AstNode));

	typeSpec(e);
    struct AstNode* d = declarator();

    if(cur == TK_LPAREN)

	while(cur != TK_EOF){
		switch(cur){
            case TK_LBRACE:
                procBody(e);
                break;
            case TK_ASSIGN:
                initializer();
                continue;
            case TK_COMMA:
                skip();
                declarator();
                continue;
            case TK_SEMICOLON:
                skip();
                break;
            case TK_EOF:
                printf("externalDecl error\n");
                exit(1);
            default:
                printf("unexpect token: [%s] \n",kwArray[cur]);
                exit(1);
        }
        break;
	}

	return e;
}

struct AstNode* translationUnit()
{
    advance();
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
    struct AstNode* programAst = translationUnit();
    printf("\n\nparse success!\n");
}




