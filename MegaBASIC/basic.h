#ifndef _BASIC_H
#define _BASIC_H

#include <stdint.h>
#include "host.h"
//#include "arduBasic_config.h"

// keep this in sync with tokenTable in basic.c
#define TOKEN_EOL		0
#define TOKEN_IDENT		1	// special case - identifier follows
#define TOKEN_INTEGER	        2	// special case - integer follows (line numbers only)
#define TOKEN_NUMBER	        3	// special case - number follows
#define TOKEN_STRING	        4	// special case - string follows

#define TOKEN_LBRACKET	        8
#define TOKEN_RBRACKET	        9
#define TOKEN_PLUS	    	10
#define TOKEN_MINUS		11
#define TOKEN_MULT		12
#define TOKEN_DIV		13
#define TOKEN_EQUALS	        14
#define TOKEN_GT		15
#define TOKEN_LT		16
#define TOKEN_NOT_EQ	        17
#define TOKEN_GT_EQ		18
#define TOKEN_LT_EQ		19
#define TOKEN_CMD_SEP	        20
#define TOKEN_SEMICOLON	        21
#define TOKEN_COMMA		22
#define TOKEN_AND		23	// FIRST_IDENT_TOKEN
#define TOKEN_OR		24
#define TOKEN_NOT		25
#define TOKEN_PRINT		26
#define TOKEN_LET		27
#define TOKEN_LIST		28
#define TOKEN_RUN		29
#define TOKEN_GOTO		30
#define TOKEN_REM		31
#define TOKEN_STOP		32
#define TOKEN_INPUT	        33
#define TOKEN_CONT		34
#define TOKEN_IF		35
#define TOKEN_THEN		36
#define TOKEN_LEN		37
#define TOKEN_VAL		38
#define TOKEN_RND		39
#define TOKEN_INT		40
#define TOKEN_STR		41
#define TOKEN_FOR		42
#define TOKEN_TO		43
#define TOKEN_STEP		44
#define TOKEN_NEXT		45
#define TOKEN_MOD		46
#define TOKEN_NEW		47
#define TOKEN_GOSUB		48
#define TOKEN_RETURN	        49
#define TOKEN_DIM		50
#define TOKEN_LEFT		51
#define TOKEN_RIGHT	        52
#define TOKEN_MID		53
#define TOKEN_CLS               54
#define TOKEN_PAUSE             55
#define TOKEN_POSITION          56
#define TOKEN_PIN               57
#define TOKEN_PINMODE           58
#define TOKEN_INKEY             59
#define TOKEN_SAVE              60
#define TOKEN_LOAD              61
#define TOKEN_PINREAD           62
#define TOKEN_ANALOGRD          63
#define TOKEN_DIR               64
#define TOKEN_DELETE            65
#define TOKEN_ASC				66
#define TOKEN_CHR				67
#define TOKEN_FREE				68
#define TOKEN_MSTIME			69
#define TOKEN_SIN				70
#define TOKEN_COS				71
#define TOKEN_TAN				72
#define TOKEN_PLOT				73
#define TOKEN_COLOR				74


#define FIRST_IDENT_TOKEN 23
#define LAST_IDENT_TOKEN 74

#define FIRST_NON_ALPHA_TOKEN    8
#define LAST_NON_ALPHA_TOKEN    22

#define ERROR_NONE				0
// parse errors
#define ERROR_LEXER_BAD_NUM			1
#define ERROR_LEXER_TOO_LONG			2
#define ERROR_LEXER_UNEXPECTED_INPUT	        3
#define ERROR_LEXER_UNTERMINATED_STRING         4
#define ERROR_EXPR_MISSING_BRACKET		5
#define ERROR_UNEXPECTED_TOKEN			6
#define ERROR_EXPR_EXPECTED_NUM			7
#define ERROR_EXPR_EXPECTED_STR			8
#define ERROR_LINE_NUM_TOO_BIG			9
// runtime errors
#define ERROR_OUT_OF_MEMORY			10
#define ERROR_EXPR_DIV_ZERO			11
#define ERROR_VARIABLE_NOT_FOUND		12
#define ERROR_UNEXPECTED_CMD			13
#define ERROR_BAD_LINE_NUM			14
#define ERROR_BREAK_PRESSED			15
#define ERROR_NEXT_WITHOUT_FOR			16
#define ERROR_STOP_STATEMENT			17
#define ERROR_MISSING_THEN			18
#define ERROR_RETURN_WITHOUT_GOSUB		19
#define ERROR_WRONG_ARRAY_DIMENSIONS	        20
#define ERROR_ARRAY_SUBSCRIPT_OUT_RANGE	        21
#define ERROR_STR_SUBSCRIPT_OUT_RANGE	        22
#define ERROR_IN_VAL_INPUT			23
#define ERROR_BAD_PARAMETER                     24

#define MAX_IDENT_LEN	8
#define MAX_NUMBER_LEN	10

extern unsigned char mem[];

extern uint16_t lineNumber;	// 0 = input buffer

typedef struct {
    float val;
    float step;
    float end;
    uint16_t lineNumber;
    uint16_t stmtNumber;
} 
ForNextData;

typedef struct {
    char const *token;
    uint8_t format;
} 
TokenTableEntry;

//extern const char *errorTable[];
extern const char* const errorTable[];


namespace tinybasic {

// private functions
void printTokens(unsigned char *p);
void listProg(uint16_t first, uint16_t last);
unsigned char *findProgLine(uint16_t targetLineNumber);
void deleteProgLine(unsigned char *p);
int doProgLine(uint16_t lineNumber, unsigned char* tokenPtr, int tokensLength);
int stackPushNum(float val);
float stackPopNum();
int stackPushStr(char *str);
char *stackGetStr();
char *stackPopStr();
void stackAdd2Strs();
void stackLeftOrRightStr(int len, int mode);
void stackMidStr(int start, int len);
unsigned char *findVariable(char *searchName, int searchMask);
void deleteVariableAt(unsigned char *pos);
int storeNumVariable(char *name, float val);
int storeForNextVariable(char *name, float start, float step, float end, uint16_t lineNum, uint16_t stmtNum);
int storeStrVariable(char *name, char *val);
int createArray(char *name, int isString);
int _getArrayElemOffset(unsigned char **p, int *pOffset);
int setNumArrayElem(char *name, float val);
int setStrArrayElem(char *name);
float lookupNumArrayElem(char *name, int *error);
char *lookupStrArrayElem(char *name, int *error);
float lookupNumVariable(char *name);
char *lookupStrVariable(char *name);
ForNextData lookupForNextVariable(char *name);
int gosubStackPush(int lineNumber,int stmtNumber);
int gosubStackPop(int *lineNumber, int *stmtNumber);
int nextToken();
int getNextToken();
int parseNumberExpr();
int parseSubscriptExpr();
int parseFnCallExpr();
int parseIdentifierExpr();
int parseStringExpr();
int parseParenExpr();
int parse_RND();
int parse_MSTIME();
int parse_INKEY();
int parseUnaryNumExp();
int parsePrimary();
int getTokPrecedence();
int parseBinOpRHS(int ExprPrec, int lhsVal);
int parseExpression();
int expectNumber();
int parse_RUN();
int parse_GOTO();
int parse_PAUSE();
int parse_LIST();
int parse_PRINT();
int parseMultIntCmd();
int parseAssignment(bool inputStmt);
int parse_IF();
int parse_FOR();
int parse_NEXT();
int parse_GOSUB();
int parseLoadSaveCmd();
int parseSimpleCmd();
int parse_DIM();
int parseStmts();
// public
void init();
int tokenize(unsigned char *input, unsigned char *output, int outputSize);
void reset();
int getFreeMem(void);
int getProgSize(void);
int processInput(unsigned char *tokenBuf);
uint16_t getLineNumber();



}	// namespace tinybasic


#endif	// _BASIC_H

