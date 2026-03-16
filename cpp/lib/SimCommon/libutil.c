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

#include 	"comlib.h"
//#include	"constant.h"
//#include	"basic_type.h"
//#include	"libfile.h"

#define		DEFAULT_PERMIT	0755

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

char * ltrim(char * s)
{
	char	* r = s;

	if ( s == NULL ) return NULL;
	
	while ( (*r ==  SPACE) || (*r == TAB) )
		r++;
 
	while( (*s++ = *r++) != '\0');

	return s;
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
	int	sz = (int)strlen(apname); 
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
	int	rv;
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

void Sleep(long lMilliseconds)
{
	if (lMilliseconds <= 0) 
		return;

	sleep((unsigned int)(lMilliseconds / 1000));

	usleep(lMilliseconds % 1000 * 1000);
}

bool CopyFile(char *pSrc, char *pDes, bool bFailIfExists)
{
	char szStr[256], szDesDir[256];
	struct stat statBuff;
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

int get_record_len(unsigned char *buff, unsigned int ptr, unsigned int f_len, unsigned char cdv)
{
    int cnt = 0;
    int i;

    for(i=ptr; ; i++) 
	{
        if( buff[i] == cdv )
            break;
        if( i >= f_len )
            break;
        cnt++;
    }

    return(cnt);
}

int get_record_len_back(unsigned char *buff, unsigned int ptr, unsigned int f_len, unsigned char cdv)
{
    int cnt = 0;
    int i;

    for(i=ptr; ; i--) 
	{
        if( buff[i] == cdv )
            break;
        if( i > f_len )
            break;
        cnt++;
    }

    return(cnt);
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
	unsigned int   num;
	unsigned int   i;

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
}

void bin2hex(const char *bin, char *hex, int size)
{
	static char conv_u[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9' \
                   , 'A', 'B', 'C', 'D', 'E', 'F' };
	static char conv_l[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9' \
                   , 'a', 'b', 'c', 'd', 'e', 'f' };
	char *conv;
	int i, j;

	conv = conv_u;
	for ( i = 0; i < size; i++)
	{
		hex[0] = conv[(bin[i] >> 4) & 0x0F];
		hex[1] = conv[bin[i] & 0x0F];
		hex += 2;
	}
  hex[0] = '\0';
}

int hex2bin(const char *hex, char bin, int size)
{
}

void n_strncpy( char *str1, char *str2, int n )
{
    if( n <= 0 )
        return;
    strncpy( str1, str2 ,n );
    str1[ n ] = '\0';
}

#ifndef MAX_ARGS_SIZE
#define MAX_ARGS_SIZE	50
#endif
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

bool file_copy(const char *src, const char *dest, bool overwrite)
{
	char	dest_path[256];
	char	command[512];
	char *p;

	if ( !file_exist(src) )
		return FALSE;

	if ( file_exist(dest) && !overwrite )
		return FALSE;
	
	p = strrchr(dest, '/');
	if ( !p )
		return FALSE;

	memset(dest_path, 0x00, sizeof(dest_path));
	strncpy(dest_path, dest, p - dest);

	if ( 0 != make_full_path(dest_path) )
		return FALSE;

	memset(command, 0x00, sizeof(command));
	sprintf(command, "cp %s %s", src, dest);
	
	system(command);

	return TRUE;
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

int make_full_path(const char *path)
{
	char  dbuff[MAX_PATH];
	int off;
	int blen, clen;
	char  aa[100];
	char  buff[MAX_PATH];

	strcpy(buff, path);

	off = 0;

	blen = (int)strlen(buff);

	if ( buff[blen-1] != '/' )
	{
		buff[blen] = '/';
		buff[++blen] = '\0';
	}

	while (1)
	{
		clen = get_record_len(buff, off, blen, '/');
		buff_seek_read(buff, dbuff, &off, clen);

		if ( off >= blen )
			break;

		off++;

		n_strncpy(aa, buff, off);
		if ( make_path(aa) != 0 )
			break;
	}

	return 0;
}
