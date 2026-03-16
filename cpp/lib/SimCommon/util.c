#include	<time.h>
#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	<sys/stat.h>
#include	<unistd.h>
#include	<pthread.h>
#include	<errno.h>
#include	<dirent.h>
#include 	<ctype.h>
#include "flk_type.h"
#include "libutil.h"

#define		DEFAULT_PERMIT	0755
bool isAlphaNum ( char *str ) 
{
	int idx = 0;

	if (str == NULL )
		return false;
	for ( idx = 0; idx < strlen(str); ++idx ) {
		if (  str[idx] < '0' || str[idx] > '9' ) {
			return false;
		}
	}
	return true;
}


char *get_arg ( char ** cpBuff )
{
    char *ptr = NULL;

    if ( *cpBuff == NULL || **cpBuff ==  0x0 )
        return NULL;

    for ( ;**cpBuff == ASCII_SP || **cpBuff == ASCII_TAB; (*cpBuff)++ ) {
        if ( **cpBuff == 0x0 )
            return NULL;
    }

   ptr = *cpBuff;

    for ( ; **cpBuff != ASCII_SP && **cpBuff != ASCII_TAB && **cpBuff != '\n'; (*cpBuff)++ ) {
        if ( **cpBuff == 0x0 )
            break;
    }

    **cpBuff = '\0';
	if ( strlen(ptr) == 0 )
		ptr = NULL;
	else 
		++(*cpBuff);

    return ptr;
}
void printHex(u_char *Data, int len)
{
	while ( len-- ) {
		printf("%02X", *Data++);
	}
	return;
}
void printHexArgs(char *Comment, u_char *Data, int len)
{
	printf("%s[", Comment);
	printHex(Data, len);
	printf("]\n");
}
void printHexData (u_char *Data, int len )
{
	int i = 0;
	for ( i = 0; i<len; ++i) {
		if ( i % 10 == 0 )
			puts("");
		printf("[%02X]", Data[i]);
	}
	puts("");
}

/*
int convert_str_to_bcd ( char *str, char *bcd )
{
	int idx = 0;
	int str_length = (int)strlen(str);
	int bcd_len = 0;
	int flag  =  0;

	if ( str == NULL || bcd == NULL )
		return FLK_FAIL;

	for ( idx = 0; idx < str_length; ++idx ) {
		flag = idx % 2;

		if ( str[idx] == 0x0 || str[idx] == 0xFF || isalnum(str[idx]) != 0 ) {
			if ( flag == 0 ) 
				bcd [ bcd_len++ ] |= 0x0F;
			break;
		}

		if ( flag == 0 ) {
			bcd [ bcd_len ] = ( (str[idx] & 0x0F) << 4 );
		}
		else {
			bcd [ bcd_len++ ] = str[idx] & 0x0F;
		}

	}

	return bcd_len;
}
*/

bool exists_file(char * f)
{
	if ( access(f, F_OK) == 0 ) return TRUE;
	return FALSE;
}

bool exists_dir(char * dir)
{
	struct stat	statbuf;

	if ( access(dir, F_OK) != 0 ) {
		dbg(9, "ERR:exists_dir(%s) access failed", dir);
		return FALSE;
	}
	
	return ( (( stat(dir, &statbuf) == 0 ) && ( statbuf.st_mode & S_IFDIR )) ? TRUE : FALSE);
}


char * rtrim(char * s)
{
	char	* r = s + strlen(s) - 1;

	while ( (*r ==  SPACE) || (*r == TAB) ) {
		*r = 0x00;
		r--;
	}
	
	return s;
}
char * ltrim(char * s)
{
    char    * r = s;

    if ( s == NULL ) return NULL;

    while ( (*r ==  ASCII_SP) || (*r == ASCII_TAB) )
        r++;

    while( (*s++ = *r++) != '\0');

    return s;
}
char * trim(char * s)
{
	if (s == NULL) return NULL;
	ltrim(s);
	rtrim(s);

	return s;
}

void upper_case(char *p)
{
	while( (*p = toupper(*p)) && *p++ != '\0' );
}

int get_field(const char * src, char ch, char (*buff)[MAX_ARGS_SIZE])
{
	int	r = 0;

	char	*p1 , *p2, *p3;

	p1 = (char *)malloc(strlen(src) + 1);
	if ( p1 == NULL ) return -1;
	p3 = p1;
	
	memcpy((void *)p1, (void *)src, strlen(src));

	if ( src[strlen(src) - 1] == ch ) 
		*(p1 + strlen(src) - 1) = 0x0;

	*(p1+ strlen(src)) = 0x0;

	while(*p1) {
		while( (*p1 == SPACE) || (*p1 == TAB) )
			p1++;

		p2 = strchr(p1, ch);
		if ( p2 == NULL ) {
			strcpy(*buff, p1);
			free(p3);
			dbg(1, "return =%d", r+1);
			return ( r + 1 );
		}

		*p2 = 0x0;
		strcpy(*buff, p1);
		buff++;
		p1 = p2 + 1;
		r++;
	}

	free(p3);
	return r;
}

void null_2_space(void * src, int sz)
{
	int	i = 0;
	char * r = (char *) src;

	for ( ; i < sz; i++ ) {
		if ( r[i] == 0x0 ) 
			r[i] = ' ';
	}
}

void null_to_space(void * src, int sz)
{
	null_2_space(src, sz);
}

char * extract_app_name(char * apname, char * aname) 
{
	char	* p;

	p = strrchr(apname, '/');
	if ( p == NULL ) {
		strcpy(aname, apname);
		return NULL;
	}

	strcpy(aname, p+1);
	return apname;
}

void remove_new_line(char * s)
{
	int	len;

	char	*p;

	dbg(1, "START:remove_new_line(%s)", s);

	if ( s == NULL ) return;

	len = (int)strlen(s);
	p = s + len;
	while( len ) {
		if ( (p[len] != CR) && (p[len] != LF) )	
			break;
		
		p[len] = 0x0;
		len--;
	}
}

int get_thread_id()
{
#ifdef PTHREAD_DEBUG
	return 	pthread_self();
#else
	return getpid();
#endif
}


bool adjust_dir(char * path)
{
	int	i;
	int	depth = 0;

	bool root_flag = FALSE;
	bool	result = FALSE;

	char 	* p;
	char	tmp_dir [ 256 ] ={0x0,};
	char	args [ MAX_ARG_CNT ][ MAX_ARGS_SIZE ];

	char 	* func = "adjust_dir";
	
	int	l = (int)strlen(path);

	dbg(1, "START:adjust_dir(%s)", path);

	if ( ( l == 0 ) || ( path == NULL ) ) 
		return result;

	if ( ( !strcmp(path, ".") ) || ( !strcmp(path, ".." ) ) ) {
		dbg(5, "WARN:path is . or ..");
		return TRUE;
	}

	if ( exists_dir(path) ) {
		dbg(5, "WARN:adjust_dir is already exists.(%s)", path);
		return TRUE;
	}

	root_flag =  (path[0] == '/') ?  TRUE : FALSE;
	if ( root_flag ) {
		dbg(1, "root_flag is TRUE");
		p = path + 1;
		tmp_dir[0] = '/';
	} else 
		p = path;

	depth = get_field(p, '/', args);
	if ( depth  <= 0 ) {
		dbg(9, "ERR:%s invalid path(%s)(%d)", func, path, depth);
		return FALSE;
	}

	for ( i = 0; i < depth; i++ ) {
		sprintf(tmp_dir, "%s%s/", tmp_dir, args[i]);
		l = (int)strlen(tmp_dir);
		if ( tmp_dir[l - 1] == '/' ) tmp_dir[l-1] = 0x0; 
		dbg(5, "dir path=%s", tmp_dir);

		if ( exists_dir(tmp_dir) ) {
			dbg(5, "WARN:adjust_dir is already exists.(%s)", tmp_dir);
			tmp_dir[l-1] = '/';
			continue;
		}

		dbg(5, "WARN:dir(%s) not found", tmp_dir);

		if ( mkdir(tmp_dir, DEFAULT_PERMIT) == -1 ) {
			dbg(9, "ERR:%s make directory (%d)(%s)", func, errno, tmp_dir);
			return FALSE;
		}

		dbg(5, "OK:dir(%s) maked", tmp_dir);
		tmp_dir[l-1] = '/';
	}

	return TRUE;
}

void Sleep(int lMilliseconds)
{
	if (lMilliseconds <= 0) 
		return;

	sleep((unsigned int)(lMilliseconds / 1000));

	usleep(lMilliseconds % 1000 * 1000);
}

bool CopyFile(char *pSrc, char *pDes, bool bFailIfExists)
{
	char szStr[256], szDesDir[256];
	char *p;

	if (!exists_file(pSrc))
		return FALSE;

	if (bFailIfExists) 
		if (exists_file(pDes))
			return FALSE;
	
	p = strrchr(pDes, '/');
	if (p == NULL) {
		return FALSE;
	}

	memset(szDesDir, 0x00, sizeof(szDesDir));
	strncpy(szDesDir, pDes, p - pDes);		

	if (!exists_dir(szDesDir)) {
		if (mkdir(szDesDir, 0744) < 0) {
			dbg(1, "ERR: make dir failed(%s)", szDesDir);
			return FALSE;
		}
	}
	
	memset(szStr, 0x00, sizeof(szStr));
	sprintf(szStr, "cp %s %s", pSrc, pDes);

	system(szStr);
	return TRUE;
}


int buff_seek_read(unsigned char *sbuff, unsigned char *dbuff, unsigned int *off, unsigned int len)
{
    int i;

    for(i=0; i<len; i++)
        dbuff[i] = sbuff[(*off)++];

    dbuff[i] = '\0';
	return i;
}

void cuteblk(unsigned char *buff) 
{
	int   num;
	int   i;

	num = (int)strlen((char *)buff) - 1;
	for(i=num; i>=0; i--) 
	{
		if( buff[i] != 0x0d && buff[i] != 0x0a && buff[i] != ' ' && buff[i] != '\t' )
			break;
		buff[i] = '\0';
	}
}

int strutbk(char *str, char c, char _c)
{
	int	len, i;

	len = (int)strlen(str);
	for(i = 0; i < len; i++) 
	{
		if(str[i] == c) 
		{ 
			str[i] = _c;	
			continue;
		}
		str[i] = str[i];
	} 
	str[i] = '\0';
	return i;
}

static char conv_u[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9' \
	, 'A', 'B', 'C', 'D', 'E', 'F' };
//static char conv_l[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
void bin2hex(const char *bin, char *hex, int size)
{
	char *conv;
	int i;

	conv = conv_u;
	for ( i = 0; i < size; i++)
	{
		hex[0] = conv[(bin[i] >> 4) & 0x0F];
		hex[1] = conv[bin[i] & 0x0F];
		hex += 2;
	}
  hex[0] = '\0';
}

int hex2bin(const char *hex, char *bin, int size)
{
	unsigned char c = 0x00;
	int i = 0, pos = 0;
	for ( i =0 ; i < size; ++i ) {

		c = hex[i];

		if ( c >= '0' && c <='9' ) {
			c = c - 0x30;
		}
		else if ( c >='A' && c <= 'F' ) {
			c = c - ('A' + 0x0A);
		}
		else if ( c >='a' && c <= 'f' ) {
			c = c - ('a' + 0x0a);
		}
		else {
			return -1;
		}

		if ( i % 2 == 1 ) {
			bin[pos++] = bin[pos] | (c & 0x0F);
		}
		else {
			bin[pos] = c << 4;
		}
	}
	return pos;
}

void n_strncpy( char *str1, char *str2, int n )
{
    if( n <= 0 )
        return;
    strncpy( str1, str2 ,n );
    str1[ n ] = '\0';
}

int get_field_2(const char * src, char ch, char (*buff)[MAX_ARGS_SIZE])
{
	int	r = 0;
	int	l = (int)strlen(src);

	char	*p1 , *p2, *p3;

	p1 = (char *)malloc(l + 1);
	if ( p1 == NULL ) return -1;
	p3 = p1;
	
	memset(p1, 0x0, l+1);
	memcpy((void *)p1, (void *)src, l);
/*
	if ( src[l - 1] == ch ) 
		*(p1 + l - 1) = 0x0;
*/
	*(p1+ l) = 0x0;

	while(*p1) {
		while( (*p1 == 0x20) || (*p1 == 0x09) )
			p1++;

		p2 = strchr(p1, ch);
		if ( p2 == NULL ) {
			strcpy(*buff, p1);
			free(p3);
			return ( r + 1 );
		}

		*p2 = 0x0;
		strcpy(*buff, p1);
		buff++;
		p1 = p2 + 1;
		r++;
	}

	free(p3);
	return  r + 1;
}


bool file_exist(const char *filename)
{
	struct stat buf;

	return stat(filename, &buf) ? FALSE : TRUE;
}

int make_path(const char *path)
{
	DIR *dirp;

	if ( (dirp = opendir(path)) == 0 )
	{
		if ( mkdir(path, 0777) < 0 )
			return -1;
	}
	else
		closedir(dirp);

	return 0;
}

