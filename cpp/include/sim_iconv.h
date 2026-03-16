#ifndef _H_FLK_ICONV_
#define _H_FLK_ICONV_ 

#include <sys/types.h> 

#define FLK_ICONV_UCS2 "UCS-2"
#define FLK_ICONV_EUCKR "EUC-KR"
#define FLK_ICONV_CP949 "CP949" // KSC5601 Extension 
#define FLK_ICONV_UTF8 "UTF8"

typedef enum {
    FLK_ICONV_TYPE_UCS2,
    FLK_ICONV_TYPE_EUCKR,
	FLK_ICONV_TYPE_CP949,
	FLK_ICONV_TYPE_UTF8,
    FLK_ICONV_TYPE_MAX
}FLK_ICONV_TYPE;

typedef struct flk_iconv_type_name {
	int type;
	char name[32];
}flk_iconv_type_name_st;

typedef void* flk_iconv_t;
/* Our own notion of wide character, as UCS-4, according to ISO-10646-1. */
typedef unsigned int flk_ucs4_t;

/* State used by a conversion. 0 denotes the initial state. */
typedef unsigned int flk_state_t;
/* flk_iconv_t is an opaque type. This is the real flk_iconv_t type. */
typedef struct flk_conv_struct * flk_conv_t;

/*
 * Data type for conversion multibyte -> unicode
 */
struct flk_mbtowc_funcs {
  int (*xxx_mbtowc) (flk_conv_t conv, flk_ucs4_t *pwc, unsigned char const *s, int n);
  /*
   * int xxx_mbtowc (flk_conv_t conv, flk_ucs4_t *pwc, unsigned char const *s, int n)
   * converts the byte sequence starting at s to a wide character. Up to n bytes
   * are available at s. n is >= 1.
   * Result is number of bytes consumed (if a wide character was read),
   * or -1 if invalid, or -2 if n too small, or -2-(number of bytes consumed)
   * if only a shift sequence was read.
   */
  int (*xxx_flushwc) (flk_conv_t conv, flk_ucs4_t *pwc);
  /*
   * int xxx_flushwc (flk_conv_t conv, flk_ucs4_t *pwc)
   * returns to the initial state and stores the pending wide character, if any.
   * Result is 1 (if a wide character was read) or 0 if none was pending.
   */
};

/*
 * Data type for conversion unicode -> multibyte
 */
struct flk_wctomb_funcs {
  int (*xxx_wctomb) (flk_conv_t conv, unsigned char *r, flk_ucs4_t wc, int n);
  /*
   * int xxx_wctomb (flk_conv_t conv, unsigned char *r, flk_ucs4_t wc, int n)
   * converts the wide character wc to the character set xxx, and stores the
   * result beginning at r. Up to n bytes may be written at r. n is >= 1.
   * Result is number of bytes written, or -1 if invalid, or -2 if n too small.
   */
  int (*xxx_reset) (flk_conv_t conv, unsigned char *r, int n);
  /*
   * int xxx_reset (flk_conv_t conv, unsigned char *r, int n)
   * stores a shift sequences returning to the initial state beginning at r.
   * Up to n bytes may be written at r. n is >= 0.
   * Result is number of bytes written, or -2 if n too small.
   */
};

/*
 * Data type for general conversion loop.
 */
struct loop_funcs {
  size_t (*loop_convert) (flk_iconv_t icd,
                          const char* * inbuf, size_t *inbytesleft,
                          char* * outbuf, size_t *outbytesleft);
  size_t (*loop_reset) (flk_iconv_t icd,
                        char* * outbuf, size_t *outbytesleft);
};

/*
 * Contents of a conversion descriptor.
 */

#define ICONV_EXT_CHAR 0x7F

#define IS_ICONV_EXT_OPTION(x) ((x->ext_flag) == ICONV_EXT_FLAG_ON ) 

#define CHARSET_CP949   0x84
#define CHARSET_UCS2    0x08

typedef enum { ICONV_EXT_FLAG_OFF, ICONV_EXT_FLAG_ON }ICONV_EXT_FLAG;

struct flk_conv_struct {
	int inconv;
	int outconv;
	struct loop_funcs lfuncs;
	/* Input (conversion multibyte -> unicode) */
	struct flk_mbtowc_funcs ifuncs;
	flk_state_t istate;
	/* Output (conversion unicode -> multibyte) */
	struct flk_wctomb_funcs ofuncs;
	flk_state_t ostate;
	int 	conv_tb_match_failed;

	/* FLK Ext Flag Process On/Off Flag */
	// Can't Convert Character Processing
	// OFF -> '?'
	// ON -> EXT_FLAG + Character
	ICONV_EXT_FLAG 	ext_flag;
	int ext_length;
};


struct flk_iconv_encoding {
  struct flk_mbtowc_funcs ifuncs; /* conversion multibyte -> unicode */
  struct flk_wctomb_funcs ofuncs; /* conversion unicode -> multibyte */
};

#define FLK_ICONV_RET_TOOFEW(n)  (-2-(n))
/* define flk_iconv error code */
typedef enum {
	FLK_ICONV_WARN_TB_NOT_MATCH = -3,
	FLK_ICONV_ERR_FEW = -2,
	FLK_ICONV_ERR_INVALID = -1
}FLK_ICONV_ERR_TYPE;


/* define function */
/**************************************************
 * Function Name : ConvertContnts
 * for Feelingk  
 * Warnning : cpTData Size fix 256Bytes
 ********************************************** */
#ifdef __cplusplus
extern "C" {
#endif
int flk_convert_contents (
		const char 	*cpCType, 
		const char	*cpCData,
		size_t 		nCDataLen, 
		const char 	*cpTType, 
		char		*cpTData,
		size_t 		*npTDataLen,
		int 		ext_flag, 
		int 		*ext_length);
/************************************************************
 * Function Name : convert_ucs2_to_ksc5601 for Feelingk 
 * Function Name : convert_ksc5601_to_ucs2 for Feelingk 
 * warnning : outbytesleft set outbuf size
 ***********************************************************/
size_t convert_ucs2_to_ksc5601 (  const char **inbuf, size_t inbytesleft,
		char **outbuf, size_t *outbytesleft );
size_t convert_ksc5601_to_ucs2 ( const char **inbuf, size_t inbytesleft,
		char **outbuf, size_t *outbytesleft );

size_t flk_iconv (flk_conv_t icd,
              const char** inbuf, size_t inbytesleft,
              char* * outbuf, size_t *outbytesleft);
flk_conv_t flk_iconv_open ( FLK_ICONV_TYPE from, FLK_ICONV_TYPE to );
void flk_iconv_close ( flk_conv_t cd );
int flk_iconv_cp949_to_utf8(const char *org, int org_len, char *target, int *target_len);
int flk_iconv_utf8_to_cp949(const char *org, int org_len, char *target, int *target_len);
int flk_iconv_ucs2_to_utf8(const char *org, int org_len, char *target, int *target_len);
int flk_iconv_utf8_to_ucs2(const char *org, int org_len, char *target, int *target_len);
int flk_iconv_ucs2_to_cp949(const char *org, int org_len, char *target, int *target_len);
FLK_ICONV_TYPE get_flk_iconv_type ( const char *cpType );
#ifdef __cplusplus
}
#endif
#endif

