/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2023
* Author: TO_DO
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
* File name: Compilers.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 Compilers, Lab Section: 011
* Assignment: A12
* Date: May 01 2023
* Professor: Paulo Sousa
* Purpose: This file defines the functions called by main function.
* Function list: mainReader(), mainScanner(), mainParser().
*************************************************************/

#ifndef COMPILERS_H_
#define COMPILERS_H_

#define DEBUG 0

/*
 * ............................................................................
 * ADVICE 1:
 * Please check the "TODO" labels to develop your activity.
 *
 * ADVICE 2: This file must be updated according to each assignment
 * (see the progression: reader > scanner > parser).
 * ............................................................................
 */

/* TO_DO: Adjust your language (cannot be "Sofia") */

/* Language name */
#define STR_LANGNAME "Journey"

/* Logical constants - adapt for your language */
#define True  1
#define False 0

/*
------------------------------------------------------------
Data types definitions
NOTE: Some types may not be directly used by your language,
		but they can be necessary for conversions.
------------------------------------------------------------
*/


/* TO_DO: Define your typedefs */
typedef char			rune;
typedef char*			string;
typedef int				int32;
typedef float			float32;
typedef void			func;

typedef unsigned char	boolean;
typedef unsigned char	flag;

typedef long			int64;
typedef double			float64;

/*
------------------------------------------------------------
Programs:
1: Reader - invokes MainReader code
2: Scanner - invokes MainScanner code
3: Parser - invokes MainParser code
------------------------------------------------------------
*/
enum PROGRAMS {
	PGM_READER	= 'r',
	PGM_SCANNER = 's',
	PGM_PARSER	= 'p'
};

/*
------------------------------------------------------------
Main functions signatures
(Code will be updated during next assignments)
------------------------------------------------------------
*/
int32 mainReader(int32 argc, string* argv);

/* 
TO_DO: Include later mainScaner (A22) and mainParser (A32)
*/
func printLogo();

#endif
