#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"
#include "log.h"

#define advance nextToken
#define skip match

static MCC_TOKEN cur;

static void typeSpec();
static struct AstNode* declarator();

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

static void skipIfMatch(MCC_TOKEN t)
{
     if(cur == t){
          advance();
          return;
     }
}

static void structSpec()
{
	 printf("in structSpec\n");
	char* id = strdup(savedTokenStr);
    match(TK_ID);
    if(cur == TK_LBRACE){
		//jump over '{'
		advance();
        while(cur != TK_RBRACE){
            typeSpec();
            declarator();
			//jump over ';'
			advance();
        }
		//jump over '}'
		advance();
    }
}

static void typeSpec()
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

static struct AstNode* declarator()
{
    while(cur == TK_STAR){
        advance();
    }
	match(TK_ID);
	return NULL;
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
	
	skip(TK_RBRACE);
    return csn;
}

static struct AstNode* paramDecl()
{
	 printf("in paramDecl\n");
	 typeSpec();
	 declarator();
	 return NULL;
}

static struct AstNode* procParamList()
{
	 struct AstNode* p = NULL;
	 struct AstNode* pa = NULL;
	 while(cur != TK_RPAREN){
        if(!p){
            p = pa = paramDecl();
        }else{
            pa->next = paramDecl();
            pa = pa->next;
        }
    }
	 //jump over ')'
	skip(TK_RPAREN);

    return p;
}

static struct AstNode* ifStmt()
{
     match(KW_IF);
     match(TK_LPAREN);
     expression();
     match(TK_RPAREN);
     statment();
     skipIfMatch(KW_ELSE);
     statment();
     return NULL;
}

static struct AstNode* forStmt()
{
     //TODO:
     return NULL;
}

static struct AstNode* compoundStatment()
{
     
     return NULL;
}

static struct AstNode* expression()
{
     return NULL;
}

static struct AstNode* statment()
{
     switch(cur){
     case KW_IF:
          ifStmt();
          break;
     case KW_FOR:
          forStmt();
          break;
     case KW_CONTINUE:
          skip(KW_CONTINUE);
          break;
     case KW_BREAK:
          skip(KW_BREAK);
          break;
     case KW_RETURN:
          skip(KW_RETURN);
          if(cur != TK_SEMICOLON)
               expression();
          else
               skip(TK_SEMICOLON);
          break;
     case TK_LBRACE:
          compoundStatment();
          break;
     }

     return NULL;
}

static void procBody(struct AstNode* parent)
{
     printf("in ProcBody\n");
    parent->type = PROC_DECL;
	while(cur != TK_RBRACE){
         statment();
    }

    skip(TK_RBRACE);
}

struct AstNode* externalDecl()
{
	struct AstNode* e = malloc(sizeof(struct AstNode));

	typeSpec();

    if(cur == TK_SEMICOLON){
		skip(TK_SEMICOLON);
        return NULL;
    }
    struct AstNode* d = declarator();

    if(cur == TK_LPAREN){
        advance();
        procParamList();
    }

	while(cur != TK_EOF){
		switch(cur){
            case TK_LBRACE:
				skip(TK_LBRACE);
                procBody(e);
                break;
            case TK_ASSIGN:
                initializer();
                continue;
            case TK_COMMA:
                skip(TK_COMMA);
                declarator();
                continue;
            case TK_SEMICOLON:
                skip(TK_SEMICOLON);
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
        if(s->next)
            s = s->next;
    }

    return n;
}

struct AstNode* parse()
{
    struct AstNode* programAst = translationUnit();
    printf("\n\nparse success!\n");
}




