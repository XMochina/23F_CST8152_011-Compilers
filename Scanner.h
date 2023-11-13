/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2023
* Author: TO_DO
* Professors: Paulo Sousa
************************************************************
=---------------------------------------=
|  COMPILERS - ALGONQUIN COLLEGE (F23)  |
=---------------------------------------=
|										|
|                                       |
=---------------------------------------=
*/

/*
************************************************************
* File name: Scanner.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A22, A32.
* Date: May 01 2022
* Purpose: This file is the main header for Scanner (.h)
* Function list: (...).
*************************************************************/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Reader.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*#pragma warning(1:4001) */	/*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*/	/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 20  /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 5   /* maximum number of digits for IL */

#define RTE_CODE 1  /* Value for run-time error */

/* TO_DO: Define the number of tokens */
#define NUM_TOKENS 22

/* TO_DO: Define Token codes - Create your token classes */
enum TOKENS {
	ERR_T,		/*  0: Error token */
	FNI_T,		/*  1: Function name identifier token */
	INL_T,		/*  2: Integer literal token */
	FLL_T,		/*	3: Float literal toekn */
	STL_T,		/*  4: String literal token */
	LPR_T,		/*  5: Left parenthesis token */
	RPR_T,		/*  6: Right parenthesis token */
	LBR_T,		/*  7: Left brace token */
	RBR_T,		/*  8: Right brace token */
	KW_T,		/*  9: Keyword token */
	EOS_T,		/* 10: End of statement (semicolon) */
	RTE_T,		/* 11: Run-time error token */
	SEOF_T,		/* 12: Source end-of-file token */
	COM_T,		/* 13: Comma token */
	VI_T,		/* 14: Variable identifier token */
	AVI_T,		/* 15: Alphanumeric variable identifier token */
	NVI_T,		/* 16: Numeric variable identifier token */	
	AROP_T,		/* 17: Arithmetic operator token */
	REOP_T,		/* 18: Relational operator token */
	LOOP_T,		/* 19: Logical operator token */
	ASOP_T,		/* 20: Assignment operator token */
	UNOP_T		/* 21: Unary operator token */
};

/* TO_DO: Define the list of keywords */
static string tokenStrTable[NUM_TOKENS] = {
	"ERR_T",
	"FNI_T",
	"INL_T",
	"FLL_T",
	"STL_T",
	"LPR_T",
	"RPR_T",
	"LBR_T",
	"RBR_T",
	"KW_T",
	"EOS_T",
	"RTE_T",
	"SEOF_T",
	"COM_T",
	"VI_T",
	"AVI_T",
	"NVI_T",
	"AROP_T",
	"REOP_T",
	"LOOP_T",
	"ASOP_T",
	"UNOP_T"
};

/* TO_DO: Operators token attributes */
typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_POW, OP_DIV, OP_MOD } AriOperator; // +,-,*,/,%
typedef enum UnaryOperators { OP_UAD, OP_USU } UnaOperator; // ++,--
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_LT, OP_GE, OP_LE } RelOperator; // =,!=,>,<,>=,<=
typedef enum AssignemntOperators { OP_AAS, OP_AAD, OP_ASU, OP_AMU, OP_ADI, OP_ASH } AssOperator; // =,+=,-=,*=,/=,:=
typedef enum LogicalOperators { OP_AND, OP_OR, OP_NOT } LogOperator; // &&,||,!
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;

/* TO_DO: Data structures for declaring the token and its attributes */
typedef union TokenAttribute {
	int32 codeType;						/* integer attributes accessor */
	AriOperator arithmeticOperator;		/* arithmetic operator attribute code */
	RelOperator relationalOperator;		/* relational operator attribute code */
	LogOperator logicalOperator;		/* logical operator attribute code */
	AssOperator assignmentOperator;		/* assignment operator attribute code */
	UnaOperator unaryOperator;			/* unary operator attribute code */
	EofOperator seofType;				/* source-end-of-file attribute code */
	int32 intValue;						/* integer literal attribute (value) */
	int32 keywordIndex;					/* keyword index in the keyword table */
	int32 contentString;				/* string literal offset from the beginning of the string literal buffer (stringLiteralTable->content) */
	float32 floatValue;					/* floating-point literal attribute (value) */
	rune idLexeme[VID_LEN + 1];			/* variable identifier token attribute */
	rune errLexeme[ERR_LEN + 1];		/* error token attribite */
} TokenAttribute;

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct idAttibutes {
	flag flags;			/* Flags information */
	union {
		int32 intValue;				/* Integer value */
		float32 floatValue;			/* Float value */
		string stringContent;		/* String value */
	} values;
} IdAttibutes;

/* Token declaration */
typedef struct Token {
	int32 code;					/* token code */
	TokenAttribute attribute;	/* token attribute */
	IdAttibutes   idAttribute;	/* not used in this scanner implementation - for further use */
} Token;

/* Scanner */
typedef struct scannerData {
	int32 scanHistogram[NUM_TOKENS];	/* Statistics of chars */
} ScannerData, * pScanData;

///////////////////////////////////////////////////////////////////////////////////////////////////////

/* EOF definitions */
#define CHARSEOF0 '\0'
#define CHARSEOF255 0xFF

/*  Special case tokens processed separately one by one in the token-driven part of the scanner:
 *  LPR_T, RPR_T, LBR_T, RBR_T, EOS_T, SEOF_T and special chars used for tokenis include _, & and ' */

/* TO_DO: Define lexeme FIXED classes */
/* These constants will be used on nextClass */
#define CHRCOL2 '_'
#define CHRCOL3 '.'
#define CHRCOL4 '\"'
#define CHRCOL6 '#'
#define CHRCOL7 '('

/* These constants will be used on VID / MID function */
#define MNID_SUF '&'
#define COMM_SYM '#'

/* TO_DO: Error states and illegal state */
#define ESNR	11		/* Error state with no retract */
#define ESWR	12		/* Error state with retract */
#define FS		-1		/* Illegal state */

 /* TO_DO: State transition table definition */
#define NUM_STATES		11
#define CHAR_CLASSES	7

/* TO_DO: Transition table - type of states defined in separate table */
static int32 transitionTable[NUM_STATES][CHAR_CLASSES] = {
/*     func,[A-Z],[0-9],	",	  /,	*, other
	   F(0), L(1), D(2), Q(3), S(4), A(5),  O(6)  */
	{     1,	2, ESNR,	4,    6, ESNR, ESNR}, // S0: NOFS
	{  ESNR,    2, ESNR, ESNR, ESNR, ESNR, ESNR}, // S1: NOFS
	{    FS,    2,    2, ESNR, ESNR, ESNR,	  3}, // S2: FSNR (MID)
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS}, // S3: FSWR (KEY)
	{     4,    4,    4,    5,    4,    4,	  4}, // S4: NOFS
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS}, // S5: ASNR (SL)
	{  ESNR, ESNR, ESNR, ESNR,    7,    8, ESNR}, // S6: NOFS
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS}, // S7: ASNR (COM)
	{     8,    8,    8,    8,    8,    9,	  8}, // S8: NOFS
	{  ESNR, ESNR, ESNR, ESNR,   10, ESNR, ESNR}, // S9: NOFS
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS}  // S10: ASNR (COM)
};

/* Define accepting states types */
#define NOFS	0		/* not accepting state */
#define FSNR	1		/* accepting state with no retract */
#define FSWR	2		/* accepting state with retract */

/* TO_DO: Define list of acceptable states */
static int32 stateType[NUM_STATES] = {
	NOFS, /* 00 */
	NOFS, /* 01 */
	FSNR, /* 02 (MID) - Methods */
	FSWR, /* 03 (KEY) - Variables */
	NOFS, /* 04 */
	FSNR, /* 05 (SL) - String Literals*/
	NOFS, /* 06 */
	FSNR, /* 07 (COM) - Comment */
	NOFS, /* 08 */
	NOFS, /* 09 */
	FSNR  /* 10 (COM) - Comment */
};

/*
-------------------------------------------------
TO_DO: Adjust your functions'definitions
-------------------------------------------------
*/

/* Static (local) function  prototypes */
int32			startScanner(BufferPointer psc_buf);
static int32	nextClass(rune c);			/* character class function */
static int32	nextState(int32, rune);		/* state machine function */
func			printScannerData(ScannerData scData);
Token			tokenizer(func);

/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

/* TO_DO: Pointer to function (of one char * argument) returning Token */
typedef Token(*PTR_ACCFUN)(string lexeme);

/* Declare accepting states functions */
Token funcSLC	(string lexeme[]);
Token funcMLC	(string lexeme[]);
Token funcIL	(string lexeme);
Token funcID	(string lexeme);
Token funcSL	(string lexeme);
Token funcKEY	(string lexeme);
Token funcErr	(string lexeme);

/* 
 * Accepting function (action) callback table (array) definition 
 * If you do not want to use the typedef, the equvalent declaration is:
 */

/* TO_DO: Define final state table */
static PTR_ACCFUN finalStateTable[NUM_STATES] = {
	NULL,		/* -    [00] */
	NULL,		/* -    [01] */
	funcID,		/* MID	[02] */
	funcKEY,	/* KEY  [03] */
	NULL,		/* -    [04] */
	funcSL,		/* SL   [05] */
	NULL,		/* -    [06] */
	NULL,		/* -    [07] */
	NULL,		/* -    [08] */
	NULL,		/* -    [09] */
	NULL		/* -    [10] */
};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

/* TO_DO: Define the number of Keywords from the language */
#define KWT_SIZE 10

/* TO_DO: Define the list of keywords */
static string keywordTable[KWT_SIZE] = {
	"return",		/* KW00 */
	"break",		/* KW01 */
	"struct",		/* KW02 */
	"func",			/* KW03 */
	"else",			/* KW04 */
	"for",			/* KW05 */
	"const",		/* KW06 */
	"if",			/* KW07 */
	"defer",		/* KW08 */
	"goto"			/* KW09 */
};

/* NEW SECTION: About indentation */

/*
 * Scanner attributes to be used (ex: including: intendation data
 */

#define INDENT '\t'  /* Tabulation */

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct languageAttributes {
	rune indentationCharType;
	int32 indentationCurrentPos;
	/* TO_DO: Include any extra attribute to be used in your scanner (OPTIONAL and FREE) */
} LanguageAttributes;

/* Number of errors */
int32 numScannerErrors;

/* Scanner data */
ScannerData scData;

#endif
