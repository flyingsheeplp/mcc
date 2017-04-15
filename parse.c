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
static struct AstNode* statment();
static struct AstNode* expression();
static struct AstNode* assignExpr();
static struct AstNode* decl();

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
		skip(TK_LBRACE);
        while(cur != TK_RBRACE){
            typeSpec();
            declarator();
			//jump over ';'
			skip(TK_SEMICOLON);
        }
		//jump over '}'
		skip(TK_RBRACE);
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
        case KW_VOID:
             match(KW_VOID);
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
        skip(TK_STAR);
    }

	skip(TK_ID);
    while(cur == TK_LBRACKET){
         skip(TK_LBRACKET);
         if(cur == TK_RBRACKET)
              skip(TK_RBRACKET);
         else{
              skip(TK_CINT);
              skip(TK_RBRACKET);
         }
    }
	return NULL;
}

static void initializer()
{
     assignExpr();
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

static struct AstNode* argumentExprList()
{
     assignExpr();
     while(cur == TK_COMMA){
          skip(TK_COMMA);
          assignExpr();
     }

     return NULL;
}

static struct AstNode* primaryExpr()
{
     switch(cur){
     case TK_ID:
     case TK_CINT:
     case TK_CCHAR:
     case TK_CSTR:
          skip(cur);
          break;
     case TK_LPAREN:
          skip(TK_LPAREN);
          expression();
          skip(TK_RPAREN);
          break;
     default:
          printf("error token in primary expression: %s\n",kwArray[cur]);
          exit(1);
     }
     
     return NULL;
}

static struct AstNode* postfixExpr()
{
     primaryExpr();
     while(1){
          if(cur == TK_LBRACKET){
               skip(TK_LBRACKET);
               expression();
               skip(TK_RBRACKET);
          }else if(cur == TK_LPAREN){
               skip(TK_LPAREN);
               if(cur == TK_RPAREN)
                    skip(TK_RPAREN);
               else{
                    expression();
                    skip(TK_RPAREN);
               }
          }else if(cur == TK_DOT){
               skip(TK_DOT);
               match(TK_ID);
          }else if(cur == TK_POINTSTO){
               skip(TK_POINTSTO);
               match(TK_ID);
          }else{
               break;
          }
     }

     return NULL;
}

static struct AstNode* unaryExpr()
{
     switch(cur){
     case TK_STAR:case TK_PLUS:case TK_MINUS:
          skip(cur);
          unaryExpr();
          break;
     case KW_SIZEOF:
          skip(TK_LPAREN);
          typeSpec();
          skip(TK_RPAREN);
          break;
     default:
          postfixExpr();
          break;
     }

     return NULL;
}

static struct AstNode* mulExpr()
{
     unaryExpr();
     while(cur == TK_STAR || cur == TK_DIVIDE || cur == TK_MOD){
          skip(cur);
          unaryExpr();
     }
     return NULL;
}

static struct AstNode* addExpr()
{
     mulExpr();
     while(cur == TK_PLUS || cur == TK_MINUS){
          skip(cur);
          mulExpr();
     }

     return NULL;
}

static struct AstNode* relExpr()
{
     addExpr();
     while(cur == TK_LT || cur == TK_LE || cur == TK_GT || cur == TK_GE){
          skip(cur);
          addExpr();
     }

     return NULL;
}

static struct AstNode* equalExpr()
{
     relExpr();
     while(cur == TK_EQ || cur == TK_NEQ){
          skip(cur);
          relExpr();
     }
     return NULL;
}

static struct AstNode* assignExpr()
{
     equalExpr();
     
     if(cur == TK_ASSIGN){
          skip(TK_ASSIGN);
          assignExpr();
     }
     
     return NULL;
}

static struct AstNode* expression()
{
     assignExpr();
     while(cur == TK_COMMA){
          skip(TK_COMMA);
          assignExpr();
     }
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

static struct AstNode* whileStmt()
{
     skip(KW_WHILE);
     skip(TK_LPAREN);
     expression();
     skip(TK_RPAREN);
     statment();
     return NULL;
}

static struct AstNode* compoundStatment()
{
     skip(TK_LBRACE);
     while(cur == KW_INT || cur == KW_CHAR || cur == KW_VOID || cur == KW_STRUCT){
          decl();
     }

     while(cur != TK_RBRACE){
          statment();
     }
     skip(TK_RBRACE);
     return NULL;
}

static struct AstNode* expStatment()
{
     if(cur == TK_SEMICOLON)
          skip(TK_SEMICOLON);
     else{
          expression();
          skip(TK_SEMICOLON);
     }
     return NULL;
}

static struct AstNode* statment()
{
     switch(cur){
     case KW_IF:
          ifStmt();
          break;
     case KW_WHILE:
          whileStmt();
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
     default:
          expStatment();
          break;
     }

     return NULL;
}

static void procBody(struct AstNode* parent)
{
    parent->type = PROC_DECL;
    compoundStatment();
}

static struct AstNode* initDeclList()
{
     if(cur == TK_ASSIGN){
          skip(TK_ASSIGN);
          initializer();
     }

     while(cur == TK_COMMA){
          skip(TK_COMMA);
          declarator();
          if(cur == TK_ASSIGN){
               skip(TK_ASSIGN);
               initializer();
          }
     }

     return NULL;
}


static struct AstNode* decl()
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

	
	switch(cur){
        case TK_LBRACE:
            procBody(e);
            break;
        
        default:
            initDeclList();
            skip(TK_SEMICOLON);
            break;
    }

	return e;
}

struct AstNode* translationUnit()
{
    advance();
    struct AstNode* n = decl();
    struct AstNode* s = n;
    while(cur != TK_EOF){
         decl();
    }

    return n;
}

struct AstNode* parse()
{
    struct AstNode* programAst = translationUnit();
    printf("\n\nparse success!\n");
}




