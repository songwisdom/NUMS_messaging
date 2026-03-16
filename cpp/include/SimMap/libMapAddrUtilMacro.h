/**
 * =====================================================================================
 *
 *       @file  libMapUtilAddrMacro.h
 *
 *    	 @brief 
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  04/27/09 09:34:05
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

#ifndef _H_LIB_MAP_ADDR_UTIL_MACRO_
#define _H_LIB_MAP_ADDR_UTIL_MACRO_
#include "flk_type.h"

/* =============================================
 * Define  Vaule
 * ============================================= */

#define ASCII_STAR 0x2A 
#define DIGIT_STAR 0x0D
#define GSM_STAR 0x0A
#define CDMA_STAR DIGIT_STAR
#define CB_STAR 0x0B

#define ASCII_SHARP 0x23
#define DIGIT_SHARP 0x0E
#define GSM_SHARP 0x0B
#define CDMA_SHARP DIGIT_SHARP
#define CB_SHARP 0x0C

#define CB_ZERO 0x0A
#define DIGIT_NULL 0xFF
#define DIGIT_MSB_4BIT 0x0F
#define DIGIT_LSB_4BIT 0xF0
#define ASCII_NULL 0x00


/* ==============================================
 * Define Macro
 * ============================================== */

/* -------------------------------------------
 * Check for Value 
 * ------------------------------------------- */
#define IS_ASCII_NUMER(x) ((x) >= 0x30 && x <= 0x39) ? true : false
#define IS_DIGIT_NUM(x) ( (char)(x) >= 0 && x <= 0x09 ) ? true : false
#define IS_CB_NUM(x) ( (x) > 0 && (x) <= 0x0A ) ? true : false

#define IS_ASCII_STAR(x) ((x) == ASCII_STAR) ? true : false
#define IS_DIGIT_STAR(x) ((x) == DIGIT_STAR) ? true : false
#define IS_GSM_STAR(x) ((x) == GSM_STAR) ? true : false
#define IS_CDMA_STAR(x) IS_DIGIT_STAR((x))
#define IS_CB_STAR(x) ((x) == CB_STAR) ? true : false 

#define IS_ASCII_SHARP(x) ((x) == ASCII_SHARP) ? true : false
#define IS_DIGIT_SHARP(x) ((x) == DIGIT_SHARP) ? true : false 
#define IS_GSM_SHARP(x) ((x) == GSM_SHARP) ? true : false
#define IS_CDMA_SHARP(x) IS_DIGIT_SHARP((x))
#define IS_CB_SHARP(x) ((x) == CB_SHARP) ? true : false

/* Check for StringAddress */
#define IS_ASCII_STRING(x) (IS_ASCII_NUMER((x))) ? true : \
	(IS_ASCII_STAR((x))) ? true : (IS_ASCII_SHARP((x))) ? true : false

/* -------------------------------------------
 * Convert A to B 
 * -------------------------------------------- */
// Convert Char(0x30 ~ 0x39) to Digit(0x00 ~ 0x09) 
#define CONV_NUM_CHAR2DIGIT(x) (IS_ASCII_NUMER((x))) ? ((x) - 0x30) : (x)
// Convert Digit(0x00 ~ 0x09) to Char(0x30 ~ 0x39) 
#define CONV_NUM_DIGIT2CHAR(x) (IS_DIGIT_NUM((x))) ? ((x) + 0x30) : (x)
// Convert Char(0x30 ~ 0x39, 0x23, 0x2A, 0x00) -> Digit (0x00~0x09, 0x0D, 0x0E, 0x0F )
#define CONV_CHAR2DIGIT(x) (IS_ASCII_NUMER((x))) ? ((x) - 0x30) : \
		(IS_ASCII_STAR((x))) ? DIGIT_STAR : \
		(IS_ASCII_SHARP((x))) ? DIGIT_SHARP : DIGIT_MSB_4BIT
// Convert  Digit (0x00~0x09, 0x0D, 0x0E, 0x0F ) ->Char(0x30 ~ 0x39, 0x23, 0x2A, 0x00) 
#define CONV_DIGIT2CHAR(x) (IS_DIGIT_NUM((x))) ? ((x) + 0x30) : \
	(IS_DIGIT_STAR((x))) ? ASCII_STAR : \
	(IS_DIGIT_SHARP((x))) ? ASCII_SHARP : ASCII_NULL
/* ---------------------------------------------
 * Bit Handler
 * --------------------------------------------- */
// Get 0xFF return 0xF0
#define GET_LSB_4BIT(x) ((x) & DIGIT_LSB_4BIT)
// Get 0xFF return 0x0F
#define GET_MSB_4BIT(x) ((x) & DIGIT_MSB_4BIT)

// Get 0xAB return 0x0A
#define GET_MOVE_LSB2MSB(x) ((GET_LSB_4BIT((x))) >> 4)
// Get 0xAB return 0xB0
#define GET_MOVE_MSB2LSB(x) ((GET_MSB_4BIT((x))) << 4)
// Convert Digit Position (0xAB -> 0xBA)
#define CHANGE_DIGIT_POS(x) ( GET_MOVE_LSB2MSB((x)) | GET_MOVE_MSB2LSB((x)) )

/* ---------------------------------------------
 *  CONVERT Digit Address for 2G
 * ---------------------------------------------*/
#define CONV_OCT_DIGIT2CDMA(x) CHANGE_DIGIT_POS((x))
#define CONV_OCT_CDMA2DIGIT(x) CHANGE_DIGIT_POS((x))


/* ----------------------------------------------
 * Convert Digit Address for 3G
 * Digit (0x00 ~ 0x09, 0x0D, 0x0E, 0x0F) => GSM (0x00 ~ 0x09, 0x0A, 0x0B, 0x0F)
 * ---------------------------------------------- */

// 1Digit Handler
#define CONV_DIGIT2GSM(x) (IS_DIGIT_NUM((x))) ? (x) : \
	(IS_DIGIT_STAR((x))) ? (GSM_STAR) : (IS_DIGIT_SHARP((x))) ? GSM_SHARP : DIGIT_MSB_4BIT
#define CONV_GSM2DIGIT(x) (IS_DIGIT_NUM((x))) ? (x) : \
	(IS_GSM_STAR((x))) ? (DIGIT_STAR) : (IS_GSM_SHARP((x))) ? DIGIT_SHARP : DIGIT_MSB_4BIT

/* 2Digit Handler*/
#define CONV_OCT_DIGIT2GSM(x) ( (CONV_DIGIT2GSM((GET_MOVE_LSB2MSB((x))))) | \
	(((CONV_DIGIT2GSM((GET_MSB_4BIT((x))))) << 4)) )
#define	CONV_OCT_GSM2DIGIT(x) ( ((CONV_GSM2DIGIT((GET_MSB_4BIT((x))))) << 4) | \
		(CONV_GSM2DIGIT((GET_MOVE_LSB2MSB((x))))) )

/* ----------------------------------------------
 * Convert Digit Address for Callback
 * Digit (0x00, 0x01 ~ 0x09, 0x0D 0x0E 0x0F) => Callback(0x0A, 0x01 ~ 0x09, 0x0B, 0x0C, 0x0F)
 * --------------------------------------------- */
// 1Digit Handler
#define CONV_DIGIT2CB(x) ( (IS_CB_NUM(x)) ? (x) : \
	((x) == 0x00) ? (CB_ZERO) : \
	(IS_DIGIT_STAR((x))) ? (CB_STAR) : \
	(IS_DIGIT_SHARP((x))) ? (CB_SHARP) : (DIGIT_MSB_4BIT) )
	
#define CONV_CB2DIGIT(x) ( (IS_DIGIT_NUM(x)) ? (x) : \
	((x) == (CB_ZERO)) ? 0x00 : \
	(IS_CB_STAR((x))) ? (DIGIT_STAR) : \
	(IS_CB_SHARP((x))) ? (DIGIT_SHARP) : (DIGIT_MSB_4BIT) )
	
// 2Digit Handler
#define CONV_OCT_DIGIT2CB(x) ( (CONV_DIGIT2CB((GET_MOVE_LSB2MSB((x))))) | \
	(((CONV_DIGIT2CB((GET_MSB_4BIT((x))))) << 4)) )
	
#define	CONV_OCT_CB2DIGIT(x) ( ((CONV_CB2DIGIT((GET_MSB_4BIT((x))))) << 4) | \
	(CONV_CB2DIGIT((GET_MOVE_LSB2MSB((x))))) )

/* ----------------------------------------------
 * Get Address Length
 * ---------------------------------------------- */
#define GET_LENGTH_BCD_TO_OCT(x) ( ((x)/2) + ((x)%2) )
#define GET_LENGTH_OCT_TO_MAPBCD(x) (((x) * 2) - ((((x) & 0xF0) == 0xF0) ? 1 : 0))
#define GET_LENGTH_OCT_TO_BCD(x) (((x) * 2) - ((((x) & 0x0F) == 0x0F) ? 1 : 0)) 

/* -----------------------------------------------
 * Util Macro 
 * ---------------------------------------------- */
#define IS_AMPLE_BCD_BUFFER(bcd, str) ( bcd < GET_LENGTH_BCD_TO_OCT(str) ) ? bcd
#define GET_HIGH_NUM(x, y) ((x) > (y)) ? (x) : (y)
#define GET_LOW_NUM(x, y) ((x) < (y)) ? (x) : (y)

#endif
