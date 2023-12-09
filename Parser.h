/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2023
* Author: Nathan Chen
* Professors: Paulo Sousa
************************************************************
=--------------------------------------------=
|    COMPILERS - ALGONQUIN COLLEGE (F23)     |
=--------------------------------------------=
|                                            |
|       _                                    |
|      | |                                   |
|      | | ___  _   _ _ __ _ __   ___ _   _  |
|  _   | |/ _ \| | | | '__| '_ \ / _ \ | | | |
| | |__| | (_) | |_| | |  | | | |  __/ |_| | |
|  \____/ \___/ \__,_|_|  |_| |_|\___|\__, | |
|                                      __/ | |
|                                     |___/  |
|                                            |
=--------------------------------------------=
*/

/*
************************************************************
* File name: Parser.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: 011
* Assignment: A32.
* Date: May 01 2023
* Professor: Paulo Sousa
* Purpose: This file is the main header for Parser (.h)
************************************************************
*/

#ifndef PARSER_H_
#define PARSER_H_

/* Inclusion section */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif
#ifndef READER_H_
#include "Reader.h"
#endif
#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/* Global vars */
static Token			lookahead;
extern Buffer*			stringLiteralTable;
extern int32			line;
extern Token			tokenizer(func);
extern string			keywordTable[KWT_SIZE];
static int32			syntaxErrorNumber = 0;

#define LANG_WRTE		"fmt.Println"
#define LANG_READ		"fmt.Scanln"
#define LANG_MAIN		"main"

/* TO_DO: Create ALL constants for keywords (sequence given in table.h) */

/* Constants */
enum KEYWORDS {
	NO_ATTR = -1,
	KW_return,
	KW_break,
	KW_struct,
	kW_func,
	KW_else,
	KW_for,
	KW_const,
	KW_if,
	KW_defer,
	KW_goto,
	KW_var,
	KW_rune,
	KW_string,
	KW_int32,
	KW_float32,
	KW_boolean,
	KW_flag,
	KW_int64,
	KW_float64
};

/* TO_DO: Define the number of BNF rules */
#define NUM_BNF_RULES 36

/* Parser */
typedef struct parserData {
	int32 parsHistogram[NUM_BNF_RULES];	/* Number of BNF Statements */
} ParserData, * pParsData;

/* Number of errors */
int32 numParserErrors;

/* Scanner data */
ParserData psData;

/* Function definitions */
func startParser();
func matchToken(int32, int32);
func syncErrorHandler(int32);
func printError();
func printBNFData(ParserData psData);

/* List of BNF statements */
enum BNF_RULES {
	BNF_error,										/*  0: Error token */
	BNF_codeSession,								/*  1 */
	BNF_comment,									/*  2 */
	BNF_dataSession,								/*  3 */
	BNF_optVarListDeclarations,						/*  4 */
	BNF_optionalStatements,							/*  5 */
	BNF_outputStatement,							/*  6 */
	BNF_outputVariableList,							/*  7 */
	BNF_program,									/*  8 */
	BNF_statement,									/*  9 */
	BNF_statements,									/* 10 */
	BNF_statementsPrime,							/* 11 */
	BNF_varListDeclarations,						/* 12 */
	BNF_varListDeclaration,							/* 13 */
	BNF_varListDeclarationPrime,					/* 14 */
	BNF_integerVarlistDeclaration,					/* 15 */
	BNF_integerVariableList,						/* 16 */
	BNF_integerDeclaration,							/* 17 */
	BNF_integerListExtended,						/* 18 */
	BNF_integerVariable,							/* 19 */
	BNF_integerAssignment,							/* 20 */
	BNF_floatVarlistDeclaration,					/* 21 */
	BNF_floatVariableList,							/* 22 */
	BNF_floatDeclaration,							/* 23 */
	BNF_floatListExtended,							/* 24 */
	BNF_floatVariable,								/* 25 */
	BNF_floatAssignment,							/* 26 */
	BNF_assignmentStatement,						/* 27 */
	BNF_assignmentExpression,						/* 28 */
	BNF_arithmeticExpression,						/* 29 */
	BNF_arithmeticExpressionPrime,					/* 30 */
	BNF_arithmeticTerm,								/* 31 */
	BNF_arithmeticTermPrime,						/* 32 */
	BNF_arithmeticFactor,							/* 33 */
	BNF_inputStatement,								/* 34 */
	BNF_inputVariableList							/* 35 */
};


/* TO_DO: Define the list of keywords */
static string BNFStrTable[NUM_BNF_RULES] = {
	"BNF_error",
	"BNF_codeSession",
	"BNF_comment",
	"BNF_dataSession",
	"BNF_optVarListDeclarations",
	"BNF_optionalStatements",
	"BNF_outputStatement",
	"BNF_outputVariableList",
	"BNF_program",
	"BNF_statement",
	"BNF_statements",
	"BNF_statementsPrime",
	"BNF_varListDeclarations",
	"BNF_varListDeclaration",
	"BNF_varListDeclarationPrime",
	"BNF_integerVarlistDeclaration",
	"BNF_integerVariableList",
	"BNF_integerDeclaration",
	"BNF_integerListExtended",
	"BNF_integerVariable",
	"BNF_integerAssignment",
	"BNF_floatVarlistDeclaration",
	"BNF_floatVariableList",
	"BNF_floatDeclaration",
	"BNF_floatListExtended",
	"BNF_floatVariable",
	"BNF_floatAssignment",
	"BNF_assignmentStatement",
	"BNF_assignmentExpression",
	"BNF_arithmeticExpression",
	"BNF_arithmeticExpressionPrime",
	"BNF_arithmeticTerm",
	"BNF_arithmeticTermPrime",
	"BNF_arithmeticFactor",
	"BNF_inputStatement",
	"BNF_inputVariableList"
};

/* TO_DO: Place ALL non-terminal function declarations */
func codeSession();
func dataSession();
func optVarListDeclarations();
func optionalStatements();
func outputStatement();
func outputVariableList();
func program();
func statement();
func statements();
func statementsPrime();
func varListDeclarations();
func varListDeclaration();
func varListDeclarationPrime();
func integerVarlistDeclaration();
func integerVariableList();
func integerDeclaration();
func integerListExtended();
func integerVariable();
func integerAssignment();
func floatVarlistDeclaration();
func floatVariableList();
func floatDeclaration();
func floatListExtended();
func floatVariable();
func floatAssignment();
func assignmentStatement();
func assignmentExpression();
func arithmeticExpression();
func arithmeticExpressionPrime();
func arithmeticTerm();
func arithmeticTermPrime();
func arithmeticFactor();
func inputStatement();
func inputVariableList();

#endif
