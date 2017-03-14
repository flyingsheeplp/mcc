/*
*    Micro C compiler
*    Copyright(c) flyingsheep.lp@gmail.com
*
*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"
#include "log.h"

#define isBlank(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define isEOF(c) ((c) == EOF)

static char savedTokenStr[255] = {0};

//debug
static int saved = 0;

int line = 0;
int pos = 0;


char* kwArray[] = {
	"KW_CHAR",
	"KW_SHORT",
	"KW_INT",
	"KW_VOID",
	"KW_STRUCT",
	"KW_IF",
	"KW_ELSE",
	"KW_FOR",
	"KW_CONTINUE",
	"KW_BREAK",
	"KW_RETURN",
	"KW_SIZEOF",
	"TK_ID",
	"TK_LPAREN",  // (
	"TK_RPAREN",  // )
	"TK_LBRACKET", // [
	"TK_RBRACKET", // ]
	"TK_LBRACE",   // {
	"TK_RBRACE",   // }
	"TK_PLUS",  // +
	"TK_MINUS", // -
	"TK_STAR", // *
	"TK_DIVIDE", // /
	"TK_MOD", // %
	"TK_NEG", // !
	"TK_EQ", // ==
	"TK_NEQ", // !=
	"TK_LT", // <
	"TK_LE", // <=
	"TK_GT", // >
	"TK_GE", // >=
	"TK_ASSIGN", // =
	"TK_COMMA", // ,
	"TK_SEMICOLON", // ;
	"TK_ELLIPSIS", // ...
	"TK_DOT", // .
	"TK_POINTSTO", // ->
	"TK_CINT",  // integer constant
	"TK_CCHAR", // char constant
	"TK_CSTR",  // C style string constant
	"TK_EOF"    //End of file
};

char nextChar()
{
	char c = fgetc(f);
	return c;
}

void pushBackChar(int c)
{
	int r = ungetc(c,f);
}


static void skipBlank()
{
	char c;
	do{
		c = nextChar();
		if(c ==  '\n'){
			line++;
#if DEBUG_SCANNER
			printf("\n");
#endif
		}
	}while(isBlank(c));

	pushBackChar(c);
}

static void skipComment()
{
	char c;
	while((c=nextChar()) != EOF){
		if(c == '*'){
			c = nextChar();
			if(c == '/')
				return;
			else
				pushBackChar(c);
		}
	}

	loge("Cannot find end of comment until EOF");
	exit(1);
}

struct kw_token{
	MCC_TOKEN t;
	char* kws;
} keywords[] = {
	{KW_CHAR, "char"},
	{KW_SHORT,"short"},
	{KW_INT, "int"},
	{KW_VOID, "void"},
	{KW_STRUCT,"struct"},
	{KW_IF, "if"},
	{KW_ELSE, "else"},
	{KW_FOR, "for"},
	{KW_CONTINUE, "continue"},
	{KW_BREAK, "break"},
	{KW_RETURN, "return"},
	{KW_SIZEOF, "sizeof"}
};


static MCC_TOKEN scanId(int first)
{
	MCC_TOKEN t = TK_ID;
	int i = 0;
	char c = first;
	while(isalpha(c)){
		savedTokenStr[i++] = c;
		c = nextChar();
	}

	pushBackChar(c);
	savedTokenStr[i] = 0;

	for(i=0;i<sizeof(keywords)/sizeof(struct kw_token);i++){
		if(!strcmp(keywords[i].kws,savedTokenStr)){
			t = keywords[i].t;
			break;
		}
	}

	saved = 1;
	return t;
}

static void scanNumber(int first)
{
	int i = 0;

	char c = first;

	while(isdigit(c)){
		savedTokenStr[i++] = c;
		c = nextChar();
	}

	savedTokenStr[i] = 0;
	pushBackChar(c);

	saved = 1;
}

static void scanStringLiteral(char l)
{
	char c;
	int i=0;
	while((c=nextChar()) != l)
		savedTokenStr[i++] = c;

	savedTokenStr[i] = 0;

	saved = 1;
}


MCC_TOKEN scanToken()
{
	MCC_TOKEN t;

	saved = 0;
	skipBlank();

	char c = nextChar();

	switch(c){
		case 'a':case 'b':case 'c':case 'd':case 'e':case 'f':case 'g':case 'h':case 'i':case 'j':case 'k':case 'l':case 'm':
		case 'n':case 'o':case 'p':case 'q':case 'r':case 's':case 't':case 'u':case 'v':case 'w':case 'x':case 'y':case 'z':
		case 'A':case 'B':case 'C':case 'D':case 'E':case 'F':case 'G':case 'H':case 'I':case 'J':case 'K':case 'L':case 'M':
		case 'N':case 'O':case 'P':case 'Q':case 'R':case 'S':case 'T':case 'U':case 'V':case 'W':case 'X':case 'Y':case 'Z':
			t = scanId(c);
			break;
		case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
			scanNumber(c);
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

		case '{':
			t = TK_LBRACE;
			break;
		case '}':
			t = TK_RBRACE;
			break;
		case '+':
			t = TK_PLUS;
			break;
		// [-] or [->]
		case '-':
			if((c = nextChar()) == '>'){
				t = TK_POINTSTO;
			}else{
				t = TK_MINUS;
				pushBackChar(c);
			}
			break;
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

		case '%':
			t = TK_MOD;
			break;
		// dot or ellipsis
		case '.':
			if((c=nextChar()) == '.'){

			}
			else{
				t = TK_DOT;
				pushBackChar(c);
			}
			break;

		case '!':
			if((c=nextChar()) == '='){
				t = TK_NEQ; // !=
			}
			else{
				t = TK_NEG;
				pushBackChar(c);
			}
			break;
		//LT or LE
		case '<':
			if((c=nextChar()) == '=')
				t = TK_LE;
			else{
				t = TK_LT;
				pushBackChar(c);
			}
			break;
		//GT or GE
		case '>':
			if((c=nextChar()) == '=')
				t = TK_GE;
			else{
				t = TK_GT;
				pushBackChar(c);
			}
			break;
		//assign or eq
		case '=':
			if((c=nextChar()) == '=')
				t = TK_EQ;
			else{
				t = TK_ASSIGN, // =
				pushBackChar(c);
			}
			break;
		case ',':
			t = TK_COMMA; // ,
			break;
		case ';':
			t = TK_SEMICOLON; // ;
			break;

		case '\'':
			scanStringLiteral('\'');
			t = TK_CCHAR;
			break;
		case '\"':
			scanStringLiteral('\"');
			t = TK_CSTR;
			break;
		case EOF:
			t = TK_EOF;
			break;
		default:
			fprintf(stderr, "unknown token: %d \n", c);
			break;
	}

#if DEBUG_SCANNER
	if(saved)
		printf("%s[%s] ",kwArray[t],savedTokenStr);
	else
		printf("%s ",kwArray[t]);
#endif

	return t;
}


