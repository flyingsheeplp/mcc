/*
*    Micro C compiler
*    Copyright(c) flyingsheep.lp@gmail.com
*
*/

#include <stdio.h>
#include "global.h"

extern FILE* f;

char nextChar()
{
	return fgetc(f);
}

void pushBackChar(c)
{
	ungetc(c,f);
}


static void skipBlank()
{
	char c;
	do{
		c = nextChar();
	}while(c == ' ' || c == '\t' || c == '\n' || c == '\r');

	pushBackChar(c);
}


MCC_TOKEN scanToken()
{
	MCC_TOKEN t;

	skipBlank();

	char c = nextChar();

	switch(c){
		case 'a':case 'b':
			t = parseId();
			break;
		case '1':case '2':
			parseNumber();
			t = TK_CINT;
			break;
		case '(':
			t = TK_LPAREN;
			break;
		case ')':
			t = TK_RPAREN;
			break;
		case '[':
			t = TK_LBRACKET;
			break;
		case ']':
			t = TK_RBRACKET;
			break;
		case '+':
			t = TK_PLUS;
			break;
		// check [-] or [->]
		case '-':
			if((c = nextChar()) == '>'){
				t = TK_POINTSTO;
			}else{
				t = TK_MINUS;
				pushBackChar(c);
			}


		case '*':
			t = TK_STAR;
			break;
		case '/':
			if((c = nextChar()) == '*'){
				skipComment();
			}else{
				t = TK_DIVIDE;
				pushBackChar(c);
			}
			break;
		case EOF:
			t = TK_EOF;
			break;

		default:
			fprintf(stderr, "unknown token: %d \n", c);
			break;
	}

	return t;
}


