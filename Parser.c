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
* File name: Parser.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A32.
* Date: May 01 2023
* Purpose: This file contains all functionalities from Parser.
* Function list: (...).
************************************************************
*/

/* TO_DO: Adjust the function header */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef PARSER_H_
#include "Parser.h"
#endif

/* Parser data */
extern ParserData psData; /* BNF statistics */

/*
************************************************************
 * Process Parser
 ***********************************************************
 */
/* TO_DO: This is the function to start the parser - check your program definition */

func startParser() {
	/* TO_DO: Initialize Parser data */
	int32 i = 0;
	for (i = 0; i < NUM_BNF_RULES; i++) {
		psData.parsHistogram[i] = 0;
	}
	/* Proceed parser */
	lookahead = tokenizer();
	if (lookahead.code != SEOF_T) {
		program();
	}
	matchToken(SEOF_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Source file parsed");
}


/*
 ************************************************************
 * Match Token
 ***********************************************************
 */
/* TO_DO: This is the main code for match - check your definition */
func matchToken(int32 tokenCode, int32 tokenAttribute) {
	int32 matchFlag = 1;
	switch (lookahead.code) {
	case KW_T:
	case VI_T:
	default:
		if (lookahead.code != tokenCode)
			matchFlag = 0;
	}
	if (matchFlag && lookahead.code == SEOF_T)
		return;
	if (matchFlag) {
		lookahead = tokenizer();
		if (lookahead.code == ERR_T) {
			printError();
			lookahead = tokenizer();
			syntaxErrorNumber++;
		}
	}
	else
		syncErrorHandler(tokenCode);
}

/*
 ************************************************************
 * Syncronize Error Handler
 ***********************************************************
 */
/* TO_DO: This is the function to handler error - adjust basically datatypes */
func syncErrorHandler(int32 syncTokenCode) {
	printError();
	syntaxErrorNumber++;
	while (lookahead.code != syncTokenCode) {
		if (lookahead.code == SEOF_T)
			exit(syntaxErrorNumber);
		lookahead = tokenizer();
	}
	if (lookahead.code != SEOF_T)
		lookahead = tokenizer();
}

/*
 ************************************************************
 * Print Error
 ***********************************************************
 */
/* TO_DO: This is the function to error printing - adjust basically datatypes */
func printError() {
	extern numParserErrors;			/* link to number of errors (defined in Parser.h) */
	Token t = lookahead;
	printf("%s%s%3d\n", STR_LANGNAME, ": Syntax error:  Line:", line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch (t.code) {
	case ERR_T:
		printf("*ERROR*: %s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case FNI_T:
		printf("MNID_T:\t\t%s\t\n", t.attribute.idLexeme);
		break;
	case STL_T:
		printf("STR_T: %s\n", readerGetContent(stringLiteralTable, t.attribute.contentString));
		break;
	case KW_T:
		printf("KW_T: %s\n", keywordTable[t.attribute.codeType]);
		break;
	case LPR_T:
	case RPR_T:
	case LBR_T:
	case RBR_T:
	case EOS_T:
		printf("NA\n");
		break;
	default:
		printf("%s%s%d\n", STR_LANGNAME, ": Scanner error: invalid token code: ", t.code);
		numParserErrors++; // Updated parser error
	}
}

/*
 ************************************************************
 * Program statement
 * BNF: <program> -> main& { <opt_statements> }
 * FIRST(<program>)= {CMT_T, MNID_T (main&), SEOF_T}.
 ***********************************************************
 */
func program() {
	/* Update program statistics */
	psData.parsHistogram[BNF_program]++;
	/* Program code */
	matchToken(KW_T, kW_func);
	switch (lookahead.code) {
	case FNI_T:
		if (strncmp(lookahead.attribute.idLexeme, LANG_MAIN, 4) == 0) {
			matchToken(FNI_T, NO_ATTR);
			matchToken(LBR_T, NO_ATTR);
			matchToken(RPR_T, NO_ATTR);
			matchToken(LBR_T, NO_ATTR);
			dataSession();
			codeSession();
			matchToken(RBR_T, NO_ATTR);
			break;
		}
		else {
			printError();
		}
	case SEOF_T:
		; // Empty
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Program parsed");
}

/*
 ************************************************************
 * dataSession
 * BNF: <dataSession> -> data { <opt_varlist_declarations> }
 * FIRST(<program>)= {KW_T (KW_data)}.
 ***********************************************************
 */
func dataSession() {
	psData.parsHistogram[BNF_dataSession]++;
	optVarListDeclarations();
	printf("%s%s\n", STR_LANGNAME, ": Data Session parsed");
}

/*
 ************************************************************
 * Optional Var List Declarations
 * BNF: <opt_varlist_declarations> -> <varlist_declarations> | e
 * FIRST(<opt_varlist_declarations>) = { e, KW_T (KW_int), KW_T (KW_real), KW_T (KW_string)}.
 ***********************************************************
 */
func optVarListDeclarations() {
	psData.parsHistogram[BNF_optVarListDeclarations]++;
	switch (lookahead.code) {
	case KW_T:
		switch (lookahead.attribute.codeType) {
		case KW_int32:
		case KW_float32:
		case KW_string:
			varListDeclarations();
			break;
		}
	default:
		; // Empty
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional Variable List Declarations parsed");
}

func varListDeclarations() {
	psData.parsHistogram[BNF_varListDeclarations]++;
	varListDeclaration();
	varListDeclarationPrime();
	printf("%s%s\n", STR_LANGNAME, ": Vaiable List Declarations parsed");
}

func varListDeclaration() {
	psData.parsHistogram[BNF_varListDeclaration]++;
	switch (lookahead.code) {
	case KW_T:
		switch (lookahead.attribute.codeType) {
		case KW_int32:
			integerVarlistDeclaration();
			break;
		case KW_float32:
			floatVarlistDeclaration();
			break;
		case KW_string:
			break;
		}
	default:
		; // Empty
	}
	printf("%s%s\n", STR_LANGNAME, ": Vaiable List Declarations parsed");
}

func varListDeclarationPrime() {
	psData.parsHistogram[BNF_varListDeclarationPrime]++;
	switch (lookahead.code) {
	case KW_T:
		switch (lookahead.attribute.codeType) {
		case KW_int32:
		case KW_float32:
		case KW_string:
			varListDeclaration();
			varListDeclarationPrime();
		default:
			;
		}
	default:
		;
	}
	printf("%s%s\n", STR_LANGNAME, ": Variable List Declaration Prime parsed");
}

func integerVarlistDeclaration() {
	psData.parsHistogram[BNF_integerVarlistDeclaration]++;
	matchToken(KW_T, KW_int32);
	integerVariableList();
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Integer Varlist Declaration parsed");
}

func integerVariableList() {
	psData.parsHistogram[BNF_integerVariableList]++;
	integerDeclaration();
	integerListExtended();
	printf("%s%s\n", STR_LANGNAME, ": Integer Variable list parsed");
}

func integerDeclaration() {
	psData.parsHistogram[BNF_integerDeclaration]++;
	switch (lookahead.code) {
	case VI_T:
		integerVariable();
		if (lookahead.code == ASOP_T) {
			integerAssignment();
		}
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Integer Declaration parsed");
}

func integerListExtended() {
	psData.parsHistogram[BNF_integerListExtended]++;
	switch (lookahead.code) {
	case COM_T:
		matchToken(COM_T, NO_ATTR);
		integerDeclaration();
		integerListExtended();
	default:
		;
	}
	printf("%s%s\n", STR_LANGNAME, ": Integer List Extended parsed");
}

func integerVariable() {
	psData.parsHistogram[BNF_integerVariable]++;
	printf("Parsing AVID_T named: %s\n", lookahead.attribute.idLexeme);
	matchToken(VI_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Integer Variable parsed");
}

func integerAssignment() {
	psData.parsHistogram[BNF_integerAssignment]++;
	matchToken(ASOP_T, OP_AAS);
	matchToken(INL_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Integer Assignment parsed");
}

func floatVarlistDeclaration() {
	psData.parsHistogram[BNF_floatVarlistDeclaration]++;
	matchToken(KW_T, KW_float32);
	floatVariableList();
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Float Varlist Declaration parsed");
}

func floatVariableList() {
	psData.parsHistogram[BNF_floatVariableList]++;
	floatDeclaration();
	floatListExtended();
	printf("%s%s\n", STR_LANGNAME, ": Float Variable list parsed");
}

func floatDeclaration() {
	psData.parsHistogram[BNF_floatDeclaration]++;
	switch (lookahead.code) {
	case VI_T:
		floatVariable();
		if (lookahead.code == ASOP_T) {
			floatAssignment();
		}
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Float Declaration parsed");
}

func floatListExtended() {
	psData.parsHistogram[BNF_floatListExtended]++;
	switch (lookahead.code) {
	case COM_T:
		matchToken(COM_T, NO_ATTR);
		floatDeclaration();
		floatListExtended();
	default:
		;
	}
	printf("%s%s\n", STR_LANGNAME, ": Float List Extended parsed");
}

func floatVariable() {
	psData.parsHistogram[BNF_floatVariable]++;
	printf("Parsing AVID_T named: %s\n", lookahead.attribute.idLexeme);
	matchToken(VI_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Float Variable parsed");
}

func floatAssignment() {
	psData.parsHistogram[BNF_floatAssignment]++;
	matchToken(ASOP_T, OP_AAS);
	matchToken(FLL_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Float Assignment parsed");
}

/*
 ************************************************************
 * codeSession statement
 * BNF: <codeSession> -> code { <opt_statements> }
 * FIRST(<codeSession>)= {KW_T (KW_code)}.
 ***********************************************************
 */
func codeSession() {
	psData.parsHistogram[BNF_codeSession]++;
	optionalStatements();
	printf("%s%s\n", STR_LANGNAME, ": Code Section parsed");
}

/* TO_DO: Continue the development (all non-terminal functions) */

/*
 ************************************************************
 * Optional statement
 * BNF: <opt_statements> -> <statements> | ϵ
 * FIRST(<opt_statements>) = { ϵ , IVID_T, FVID_T, SVID_T, KW_T(KW_if),
 *				KW_T(KW_while), MNID_T(print&), MNID_T(input&) }
 ***********************************************************
 */
func optionalStatements() {
	psData.parsHistogram[BNF_optionalStatements]++;
	switch (lookahead.code) {
	case FNI_T:
	case VI_T:
	case KW_T:
		statements();
		break;
	default:
		; // Empty
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional statements parsed");
}

/*
 ************************************************************
 * Statements
 * BNF: <statements> -> <statement><statementsPrime>
 * FIRST(<statements>) = { IVID_T, FVID_T, SVID_T, KW_T(KW_if),
 *		KW_T(KW_while), MNID_T(input&), MNID_T(print&) }
 ***********************************************************
 */
func statements() {
	psData.parsHistogram[BNF_statements]++;
	statement();
	statementsPrime();
	printf("%s%s\n", STR_LANGNAME, ": Statements parsed");
}

/*
 ************************************************************
 * Single statement
 * BNF: <statement> -> <assignment statement> | <selection statement> |
 *	<iteration statement> | <input statement> | <output statement>
 * FIRST(<statement>) = { IVID_T, FVID_T, SVID_T, KW_T(KW_if), KW_T(KW_while),
 *			MNID_T(input&), MNID_T(print&) }
 ***********************************************************
 */
func statement() {
	psData.parsHistogram[BNF_statement]++;
	switch (lookahead.code) {
	case KW_T:
		switch (lookahead.attribute.codeType) {
		case KW_if:
			break;
		case KW_for:
			break;
		default:
			printError();
		}
		break;
	case VI_T:
		assignmentStatement();
		break;
	case FNI_T:
		if (strncmp(lookahead.attribute.idLexeme, "write", 5) == 0) {
			outputStatement();
		}
		else if (strncmp(lookahead.attribute.idLexeme, "read", 4) == 0) {
			inputStatement();
		}
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Statement parsed");
}

/*
 ************************************************************
 * Statements Prime
 * BNF: <statementsPrime> -> <statement><statementsPrime> | ϵ
 * FIRST(<statementsPrime>) = { ϵ , IVID_T, FVID_T, SVID_T, 
 *		KW_T(KW_if), KW_T(KW_while), MNID_T(input&), MNID_T(print&) }
 ***********************************************************
 */
func statementsPrime() {
	psData.parsHistogram[BNF_statementsPrime]++;
	switch (lookahead.code) {
	case FNI_T:
	case VI_T:
	case KW_T:
		statement();
		statementsPrime();
	default:
		; //empty string
	}
}

func assignmentStatement() {
	psData.parsHistogram[BNF_assignmentStatement]++;
	assignmentExpression();
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Assignment statement parsed");
}

func assignmentExpression() {
	psData.parsHistogram[BNF_assignmentExpression]++;
	switch (lookahead.code) {
	case VI_T:
		matchToken(VI_T, NO_ATTR);
		matchToken(ASOP_T, OP_AAS);
		arithmeticExpression();
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Assignment expression parsed");
}

func arithmeticExpression() {
	psData.parsHistogram[BNF_arithmeticExpression]++;
	switch (lookahead.code) {
	case VI_T:
	case FLL_T:
	case INL_T:
	case LPR_T:
		arithmeticTerm();
		arithmeticExpressionPrime();
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Arithematic expression parsed");
}

func arithmeticExpressionPrime() {
	psData.parsHistogram[BNF_arithmeticExpressionPrime]++;
	if (lookahead.code == AROP_T) {
		switch (lookahead.attribute.arithmeticOperator) {
		case OP_ADD:
			matchToken(AROP_T, OP_ADD);
			arithmeticTerm();
			arithmeticExpressionPrime();
			break;
		case OP_SUB:
			matchToken(AROP_T, OP_SUB);
			arithmeticTerm();
			arithmeticExpressionPrime();
			break;
		default:
			;
		}
	}
	printf("%s%s\n", STR_LANGNAME, ": Arithematic expression prime parsed");
}

func arithmeticTerm() {
	psData.parsHistogram[BNF_arithmeticTerm]++;
	arithmeticFactor();
	arithmeticTermPrime();
	printf("%s%s\n", STR_LANGNAME, ": Arithematic Term parsed");
}

func arithmeticTermPrime() {
	psData.parsHistogram[BNF_arithmeticTermPrime]++;
	if (lookahead.code == AROP_T) {
		switch (lookahead.attribute.arithmeticOperator) {
		case OP_MUL:
			matchToken(AROP_T, OP_MUL);
			arithmeticFactor();
			arithmeticTermPrime();
			break;
		case OP_DIV:
			matchToken(AROP_T, OP_DIV);
			arithmeticFactor();
			arithmeticTermPrime();
			break;
		case OP_MOD:
			matchToken(AROP_T, OP_MOD);
			arithmeticFactor();
			arithmeticTermPrime();
			break;
		case OP_POW:
			matchToken(AROP_T, OP_POW);
			arithmeticFactor();
			arithmeticTermPrime();
			break;
		default:
			;
		}
	}
	printf("%s%s\n", STR_LANGNAME, ": Arithematic Term Prime parsed");
}

func arithmeticFactor() {
	psData.parsHistogram[BNF_arithmeticFactor]++;
	switch (lookahead.code) {
	case VI_T:
		matchToken(VI_T, NO_ATTR);
		break;
	case FLL_T:
		matchToken(FLL_T, NO_ATTR);
		break;
	case INL_T:
		matchToken(INL_T, NO_ATTR);
		break;
	case LPR_T:
		matchToken(LPR_T, NO_ATTR);
		arithmeticExpression();
		matchToken(RPR_T, NO_ATTR);
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Arithematic Factor parsed");
}

func outputStatement() {
	psData.parsHistogram[BNF_outputStatement]++;
	matchToken(FNI_T, NO_ATTR);
	matchToken(LPR_T, NO_ATTR);
	outputVariableList();
	matchToken(RPR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Output statement parsed");
}

/*
 ************************************************************
 * Output Variable List
 * BNF: <opt_variable list> -> <variable list> | ϵ
 * FIRST(<opt_variable_list>) = { IVID_T, FVID_T, SVID_T, ϵ }
 ***********************************************************
 */
func outputVariableList() {
	psData.parsHistogram[BNF_outputVariableList]++;
	switch (lookahead.code) {
	case STL_T:
		printf("%s: \"%s\"\n", "Journey: Output list (string literal) parsed", readerGetContent(stringLiteralTable, (short)lookahead.attribute.codeType));
		matchToken(STL_T, NO_ATTR);
		break;
	case VI_T:
		matchToken(VI_T, NO_ATTR);
	default:
		printf("%s\n", "Journey: Output list (empty) parsed");
		;
	}
	printf("%s%s\n", STR_LANGNAME, ": Output variable list parsed");
}

func inputStatement() {
	psData.parsHistogram[BNF_inputStatement]++;
	matchToken(FNI_T, NO_ATTR);
	matchToken(LPR_T, NO_ATTR);
	inputVariableList();
	matchToken(RPR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Input statement parsed");
}

func inputVariableList() {
	psData.parsHistogram[BNF_inputVariableList]++;
	switch (lookahead.code) {
	case VI_T:
		matchToken(VI_T, NO_ATTR);
		break;
	default:
		printError();
		;
	}
	printf("%s%s\n", STR_LANGNAME, ": Input variable list parsed");
}

/*
 ************************************************************
 * The function prints statistics of BNF rules
 * Param:
 *	- Parser data
 * Return:
 *	- Void (procedure)
 ***********************************************************
 */
/*
func printBNFData(ParserData psData) {
}
*/
func printBNFData(ParserData psData) {
	/* Print Parser statistics */
	printf("Statistics:\n");
	printf("----------------------------------\n");
	int cont = 0;
	for (cont = 0; cont < NUM_BNF_RULES; cont++) {
		if (psData.parsHistogram[cont] > 0)
			printf("%s%s%s%d%s", "Token[", BNFStrTable[cont], "]=", psData.parsHistogram[cont], "\n");
	}
	printf("----------------------------------\n");
}
