#ifndef __TOKEN_H_
#define __TOKEN_H_

#include <stddef.h>
#include <string>

/* 
*  all tokens including keywords, identifiers, constant, operators, punctuators
*/

typedef enum token {
    TK_BEGIN,

    //keywords
    TK_AUTO,		     // "auto"
    TK_EXTERN,		     // "extern"
    TK_REGISTER,	     // "register"
    TK_STATIC,		     // "static"
    TK_TYPEDEF,		     // "typedef"
    TK_CONST,		     //	"const"
    TK_VOLATILE,	     //	"volatile"
    TK_SIGNED,		     //	"signed"
    TK_UNSIGNED,	     //	"unsigned"
    TK_SHORT,		     //	"short"
    TK_LONG,		     //	"long"
    TK_CHAR,		     //	"char"
    TK_INT,		     //	"int"
    TK_INT64,		     //	"__int64"
    TK_FLOAT,		     //	"float"
    TK_DOUBLE,		     //	"double"
    TK_ENUM,		     //	"enum"
    TK_STRUCT,		     //	"struct"
    TK_UNION,		     //	"union"
    TK_VOID,		     //	"void"
    TK_BREAK,		     //	"break"
    TK_CASE,		     //	"case"
    TK_CONTINUE,	     //	"continue"
    TK_DEFAULT,		     //	"default"
    TK_DO,		     //	"do"
    TK_ELSE,		     //	"else"
    TK_FOR,		     //	"for"
    TK_GOTO,		     //	"goto"
    TK_IF,		     //	"if"
    TK_RETURN,		     //	"return"
    TK_SWITCH,		     //	"switch"
    TK_WHILE,		     //	"while"
    TK_SIZEOF,		     //	"sizeof"

    //identifier
    TK_ID,		     //	"ID"

    //constant
    TK_INTCONST,	     //	"int"
    TK_UINTCONST,	     //	"unsigned int"
    TK_LONGCONST,	     //	"long"
    TK_ULONGCONST,	     //	"unsigned long"
    TK_LLONGCONST,	     //	"long long"
    TK_ULLONGCONST,	     //	"unsigned long long"
    TK_FLOATCONST,	     //	"float"
    TK_DOUBLECONST,	     //	"double"
    TK_LDOUBLECONST,	     //	"long double"
    TK_STRING,		     //	"STR"
    TK_WIDESTRING,	     //	"WSTR"

    //operators
    TK_COMMA,		     // ","
    TK_QUESTION,	     // "?"
    TK_COLON,		     // ":"
    TK_ASSIGN,		     // "="
    TK_BITOR_ASSIGN,	     // "|="
    TK_BITXOR_ASSIGN,	     // "^="
    TK_BITAND_ASSIGN,	     // "&="
    TK_LSHIFT_ASSIGN,	     // "<<="
    TK_RSHIFT_ASSIGN,	     // ">>="
    TK_ADD_ASSIGN,	     // "+="
    TK_SUB_ASSIGN,	     // "-="
    TK_MUL_ASSIGN,	     // "*="
    TK_DIV_ASSIGN,	     // "/="
    TK_MOD_ASSIGN,	     // "%="
    TK_OR,		     // "||"
    TK_AND,		     // "&&"
    TK_BITOR,		     // "|"
    TK_BITXOR,		     // "^"
    TK_BITAND,		     // "&"
    TK_EQUAL,		     // "=="
    TK_UNEQUAL,		     // "!="
    TK_GREAT,		     // ">"
    TK_LESS,		     // "<"
    TK_GREAT_EQ,	     // ">="
    TK_LESS_EQ,		     // "<="
    TK_LSHIFT,		     // "<<"
    TK_RSHIFT,		     // ">>"
    TK_ADD,		     // "+"
    TK_SUB,		     // "-"
    TK_MUL,		     // "*"
    TK_DIV,		     // "/"
    TK_MOD,		     // "%"
    TK_INC,		     // "++"
    TK_DEC,		     // "--"
    TK_NOT,		     // "!"
    TK_COMP,		     // "~"
    TK_DOT,		     // "."
    TK_POINTER,		     // "->"
    TK_LPAREN,		     // "("
    TK_RPAREN,		     // ""
    TK_LBRACKET,	     // "["
    TK_RBRACKET,	     // "]"

    //punctuators
    TK_LBRACE,		     // "{"
    TK_RBRACE,		     // "}"
    TK_SEMICOLON,	     // ";"
    TK_ELLIPSIS,	     //  "..."
    TK_POUND,		     // "#"
    TK_NEWLINE,		     // "\n"

    TK_END,		     // "EOF"
} token;

struct tokenMap {
  token tokn;
  union value {
    int i[2];
    float f;
    double d;
    std::string *p;
  };
};

struct keyword
{
	const char *name;
	int len;
	int tok;
};

static struct keyword keywords_[] = 
{
	{"__int64", 0, TK_INT64},
	{NULL,      0, TK_ID}
};

static struct keyword keywordsA[] =
{
	{"auto", 4, TK_AUTO},
	{NULL,   0, TK_ID}
};

static struct keyword keywordsB[] = 
{
	{"break", 5, TK_BREAK},
	{NULL,    0, TK_ID}
};

static struct keyword keywordsC[] = 
{
	{"case",     4, TK_CASE},
	{"char",     4, TK_CHAR},
	{"const",    5, TK_CONST},
	{"continue", 8, TK_CONTINUE},
	{NULL,       0, TK_ID}
};

static struct keyword keywordsD[] =
{
	{"default", 7, TK_DEFAULT},
	{"do",      2, TK_DO},
	{"double",  6, TK_DOUBLE},
	{NULL,      0, TK_ID}
};

static struct keyword keywordsE[] =
{
	{"else",   4, TK_ELSE},
	{"enum",   4, TK_ENUM},
	{"extern", 6, TK_EXTERN},
	{NULL,     0, TK_ID}
};

static struct keyword keywordsF[] =
{
	{"float", 5, TK_FLOAT},
	{"for",   3, TK_FOR},
	{NULL,    0, TK_ID}
};

static struct keyword keywordsG[] = 
{
	{"goto", 4, TK_GOTO},
	{NULL,   0, TK_ID}
};

static struct keyword keywordsH[] =
{
	{NULL, 0, TK_ID}
};

static struct keyword keywordsI[] = 
{
	{"if",  2, TK_IF},
	{"int", 3, TK_INT},
	{NULL,  0, TK_ID}
};

static struct keyword keywordsJ[] = 
{
	{NULL, 0, TK_ID}
};

static struct keyword keywordsK[] = 
{
	{NULL, 0, TK_ID}
};

static struct keyword keywordsL[] = 
{
	{"long", 4,	TK_LONG},
	{NULL,   0, TK_ID}
};

static struct keyword keywordsM[] = 
{
	{NULL, 0, TK_ID}
};

static struct keyword keywordsN[] = 
{
	{NULL, 0, TK_ID}
};

static struct keyword keywordsO[] = 
{
	{NULL, 0, TK_ID}
};

static struct keyword keywordsP[] = 
{
	{NULL, 0, TK_ID}
};

static struct keyword keywordsQ[] = 
{
	{NULL, 0, TK_ID}
};

static struct keyword keywordsR[] = 
{
	{"register", 8, TK_REGISTER},
	{"return",   6, TK_RETURN},
	{NULL,       0, TK_ID}
};

static struct keyword keywordsS[] = 
{
	{"short",  5, TK_SHORT},
	{"signed", 6, TK_SIGNED},
	{"sizeof", 6, TK_SIZEOF},
	{"static", 6, TK_STATIC},
	{"struct", 6, TK_STRUCT},
	{"switch", 6, TK_SWITCH},
	{NULL,     0, TK_ID}
};

static struct keyword keywordsT[] = 
{
	{"typedef", 7, TK_TYPEDEF},
	{NULL,      0, TK_ID}
};

static struct keyword keywordsU[] = 
{
	{"union",    5, TK_UNION},
	{"unsigned", 8, TK_UNSIGNED},
	{NULL,       0, TK_ID}
};

static struct keyword keywordsV[] = 
{
	{"void",     4, TK_VOID},
	{"volatile", 8, TK_VOLATILE},
	{NULL,       0, TK_ID}
};

static struct keyword keywordsW[] = 
{
	{"while", 5, TK_WHILE },
	{NULL, 0, TK_ID}
};

static struct keyword keywordsX[] = 
{
	{NULL, 0, TK_ID}
};

static struct keyword keywordsY[] = 
{
	{NULL, 0, TK_ID}
};

static struct keyword keywordsZ[] = 
{
	{NULL, 0, TK_ID}
};
/*********************************************
 * 	classify keywords by their first letter,
 * 		to speed up comparing.
 * 			see function FindKeyword()
 * 			 *********************************************/
static struct keyword *keywords[] =
{
	keywords_, keywordsA, keywordsB, keywordsC, 
	keywordsD, keywordsE, keywordsF, keywordsG, 
	keywordsH, keywordsI, keywordsJ, keywordsK, 
	keywordsL, keywordsM, keywordsN, keywordsO, 
	keywordsP, keywordsQ, keywordsR, keywordsS, 
	keywordsT, keywordsU, keywordsV, keywordsW, 
	keywordsX, keywordsY, keywordsZ
};

#endif
