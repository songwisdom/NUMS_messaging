/**
 * =====================================================================================
 *
 *       @file  EPreProcess.h
 *
 *    	 @brief 
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  04/10/09 23:18:50
 *
 *       @author:  SMS Dev (), sms_dev@feelingk.com
 *       Company:  (c) FEELingK
 *       @attention
 *       Copyright (c) 2003 FEELINGK Co., Ltd.                                
 *
 *         All rights reserved.                                                
 *                                                                          
 *     This program contains confidential and proprietary information of    
 *     FEELINGK, and any reproduction, disclosure, or use in whole or in part 
 *     by prior written agreement or permission of FEELINGK.                 
 *                                                                          
 *                            FEELINGK Co., Ltd.                           
 *     3F Ace Techno Twin Tower 1, 212-1 Kuro-Dong, Kuro-Gu, Seoul 152-050 Korea 
 *
 * =====================================================================================
 */
#ifndef _H_EPREPROCESS_
#define _H_EPREPROCESS_
#include "FLKBaseDef.h"
#define TABSIZE 4
#define ECLEX_KEYWORD_AUTO
#define ECLEX_KEYWORD_BREAK
#define ECLEX_KEYWORD_CASE
#define ECLEX_KEYWORD_CHAR
#define ECLEX_KEYWORD_CONST
#define ECLEX_KEYWORD_CONTINUE
#define ECLEX_KEYWORD_DEFAULT
#define ECLEX_KEYWORD_DO
#define ECLEX_KEYWORD_DOUBLE
#define ECLEX_KEYWORD_ELSE
#define ECLEX_KEYWORD_ENUM
#define ECLEX_KEYWORD_EXTERN
#define ECLEX_KEYWORD_FLOAT
#define ECLEX_KEYWORD_FOR
#define ECLEX_KEYWORD_GOTO
#define ECLEX_KEYWORD_IF
#define ECLEX_KEYWORD_INT
#define ECLEX_KEYWORD_long
#define ECLEX_KEYWORD_REGISTER
#define ECLEX_KEYWORD_RETURN
#define ECLEX_KEYWORD_SHORT
#define ECLEX_KEYWORD_signed
#define ECLEX_KEYWORD_SIZEOF
#define ECLEX_KEYWORD_STATIC
#define ECLEX_KEYWORD_STRUCT
#define ECLEX_KEYWORD_SWITCH
#define ECLEX_KEYWORD_TYPEDEF
#define ECLEX_KEYWORD_UNION
#define ECLEX_KEYWORD_unsigned
#define ECLEX_KEYWORD_VOID
#define ECLEX_KEYWORD_VOLATILE
#define ECLEX_KEYWORD_WHILE

#define ECLEX_COMMENT
#define ECLEX_OPERATOR_POSTFIX
#define ECLEX_OPERATOR_ADDRESS
#define ECLEX_OPERATOR_INDIRECTION
#define ECLEX_OPERATOR_UNARY_PLUS
#define ECLEX_OPERATOR_UNARY_MINUS
#define ECLEX_OPERATOR_COMPLEMENT
#define ECLEX_OPERATOR_LOGICAL_NEGATION
#define ECLEX_OPERATOR_SIZEOF
#define ECLEX_OPERATOR_MULTIPLICATIVE
#define ECLEX_OPERATOR_ADDITIVE
#define ECLEX_OPERATOR_SHIFT
#define ECLEX_OPERATOR_RELATIONAL
#define ECLEX_OPERATOR_EQUALITY
#define ECLEX_OPERATOR_BIT_AND
#define ECLEX_OPERATOR_BIT_XOR
#define ECLEX_OPERATOR_BIT_OR
#define ECLEX_OPERATOR_LOGICAL_AND
#define ECLEX_OPERATOR_LOGICAL_OR
#define ECLEX_OPERATOR_CONDITIONAL_1
#define ECLEX_OPERATOR_CONDITIONAL_2
#define ECLEX_OPERATOR_ASSIGNMENT
#define ECLEX_OPERATOR_COMMA

#define ECLEX_DECLARATION_ARRAY
#define ECLEX_DECLARATION_FUNCTION

#define ECLEX_STATEMENT_LABELED
#define ECLEX_STATEMENT_EXPRESSION
#define ECLEX_STATEMENT_COMPOUND
#define ECLEX_STATEMENT_SELECTION
#define ECLEX_STATEMENT_ITERATION
#define ECLEX_STATEMENT_JUMP
typedef struct 
{
	int nState;
	char *cpToken;
	char *cpNextPtr;
} ECLexTokenType;

typedef struct
{
	int nTabSize;
	int bRemark;
	int bDQuotation;
	int bQuotation;
	int *cpTempRemarkBuf;
	char cpRemark[BUF_SSIZE];
	int nRemarkPos;
    char cpVar[BUF_SSIZE];
	int nVarPos;
	ECLexTokenType *spTokenPtr;
} ECLexType;
ECLexType *CreateECLex(void);
void CloseECLex(ECLexType *spCLex);
int ECLex(ECLexType *spCLex,char*cpBuf);
void ProcCLex(char *cpInFile,char *cpOutFile);
#endif
