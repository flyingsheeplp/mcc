/*
*    Micro C compiler
*    Copyright(c) flyingsheep.lp@gmail.com
*
*/

#ifndef MCC_GLOBAL_H
#define MCC_GLOBAL_H

#define DEBUG_SCANNER 1


#define MAX_STRING_SIZE 64

typedef enum{
	KW_CHAR,
	KW_SHORT,
	KW_INT,
	KW_VOID,
	KW_STRUCT,
	KW_IF,
	KW_ELSE,
	KW_FOR,
	KW_CONTINUE,
	KW_BREAK,
	KW_RETURN,
	KW_SIZEOF,

	TK_ID,
	TK_LPAREN,  // (
	TK_RPAREN,  // )
	TK_LBRACKET, // [
	TK_RBRACKET, // ]
	TK_LBRACE,   // {
	TK_RBRACE,   // }
	TK_PLUS,  // +
	TK_MINUS, // -
	TK_STAR, // *
	TK_DIVIDE, // /
	TK_MOD, // %
	TK_NEG, // !
	TK_EQ, // ==
	TK_NEQ, // !=
	TK_LT, // <
	TK_LE, // <=
	TK_GT, // >
	TK_GE, // >=
	TK_ASSIGN, // =
	TK_COMMA, // ,
	TK_SEMICOLON, // ;
	TK_ELLIPSIS, // ...
	TK_DOT, // .
	TK_POINTSTO, // ->

	TK_CINT,  // integer constant
	TK_CCHAR, // char constant
	TK_CSTR,  // C style string constant

	TK_EOF    //End of file
}MCC_TOKEN;

//source file
extern FILE* f;

//processing position
extern int line;
extern int pos;
extern char* kwArray[];

//Scan
MCC_TOKEN scanToken();

//Parse
#define MAX_CHILDREN_NR 3
#define MAX_ATTR_NR  3
struct AstNode{
	struct AstNode* next;
	struct AstNode* child[MAX_CHILDREN_NR];
	int attr[MAX_ATTR_NR];
};
#endif