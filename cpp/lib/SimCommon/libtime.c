#include 	<stdarg.h>
#include 	<stdio.h>
#include 	<sys/stat.h>
#include 	<unistd.h>
#include	<sys/time.h>
#include	<time.h>
#include 	<ctype.h>
#include <signal.h>
#include 	"FLKSimBaseDef.h"
int 	__alarm_flag;

#define		SECOND	1
#define		MINUTE	(60 * (SECOND));
#define		HOUR	((MINUTE) * (MINUTE))
#define		DAY	(24 * (HOUR))

double get_milli_sec_2()
{
	double	now;
	struct timeval	tv;

	gettimeofday(&tv, NULL); 

	now = (double)(((double)tv.tv_sec * 1000)  + ((double)tv.tv_usec / 1000));
	return now;
}

long get_milli_sec_1()
{
	long	now;
	struct timeval	tv;

	gettimeofday(&tv, NULL); 

	now = (long)(((long)tv.tv_sec * 1000)  + ((long)tv.tv_usec / 1000));
	return now;
}

long get_milli_sec()
{
	long	now;
	struct timeval	tv;

	gettimeofday(&tv, NULL); 

	now = (long)((long)tv.tv_sec);
	return now;
}

int get_datetime_str(char *dt_str)
{
    time_t  timeval;
    struct  tm *tmp;

    time(&timeval);
    tmp = localtime(&timeval);
    sprintf(dt_str, "%04d%02d%02d%02d%02d%02d",
        tmp->tm_year+1900,
        tmp->tm_mon+1,
        tmp->tm_mday,
        tmp->tm_hour,
        tmp->tm_min,
        tmp->tm_sec);

    return (int)0;
}

void get_today(char *dt_str)
{
    time_t  timeval;
    struct  tm *tmp;

    time(&timeval);
    tmp = localtime(&timeval);
    sprintf(dt_str, "%04d%02d%02d",
        tmp->tm_year+1900,
        tmp->tm_mon+1,
        tmp->tm_mday);

    return ;
}

long Get_M()
{
	struct timeval tstruct;
	gettimeofday(&tstruct, NULL); 
	return tstruct.tv_sec * 1000  + tstruct.tv_usec / 1000;
}

int GetDateTime(char *sdate)
{
    time_t  timeval;
    struct  tm *tmp;
    long r_sec;

    r_sec = time(&timeval);
    tmp = localtime(&r_sec);
    sprintf(sdate, "%04d%02d%02d%02d%02d%02d\n",
        tmp->tm_year+1900,
        tmp->tm_mon+1,
        tmp->tm_mday,
        tmp->tm_hour,
        tmp->tm_min,
        tmp->tm_sec);

    return (int)0;
}

int Getsecond(int sec)
{
    time_t  timeval;
    long r_sec;

    r_sec = time(&timeval) + sec;
    return (int)r_sec;
}

time_t get_gmt_datetime(int day)
{
	int	sign;
	time_t	t;

	t = time(NULL);
	if ( day == 0 ) return t;

	sign = ( day < 0 ) ? -1 : 1;

	t = t + ( (time_t)day * 86400 );
	return t;
}

void gmt_to_localtime(time_t * ttime, char * buff)
{
	struct	tm * tmbuf;
	tmbuf = localtime(ttime);

	sprintf(buff,"%04d:%02d:%02d", tmbuf->tm_year + 1900, tmbuf->tm_mon + 1, tmbuf->tm_mday);
	return;
}

char * format_datetime(char * r, const char * fmt)
{
	char	* fPtr, * fp;

	int	len = (int)strlen(fmt);
	time_t	t;

	struct	tm * tmbuf;

	/* format check : Y - 2 or 4, M - 2, D -2... */

	fp = (char *)calloc(len + 1 , sizeof(char));
	if (fp == NULL) return NULL;

	fPtr = fp;
	strncpy(fPtr, fmt, len);
	upper_case(fPtr);

	time(&t);
	tmbuf = localtime(&t);

	len = 0;
	while( *fPtr != '\0' ) {
		len = 2;
		switch(*fPtr) {
			case 'Y' :
				len = !(strncmp(fPtr, "YYYY", 4)) ? 4 : 2;
				if (len == 4) 
					sprintf(r, "%04d", tmbuf->tm_year + 1900);
				else
					sprintf(r, "%02d", (tmbuf->tm_year + 1900) - 2000);
				break;
			case 'M' :
				sprintf(r, "%02d", tmbuf->tm_mon + 1);
				break;
			case 'D' :
				sprintf(r, "%02d", tmbuf->tm_mday);
				break;
			case 'H' :
				sprintf(r, "%02d", tmbuf->tm_hour);
				break;
			case 'N' :
				sprintf(r, "%02d", tmbuf->tm_min);
				break;
			case 'S' :
				sprintf(r, "%02d", tmbuf->tm_sec);
				break;
			default :
				*r++ = *fPtr++;
				continue;
		}

		fPtr += len;
		r += len;
	}
	free(fp);
	return r;	
}

time_t str_to_utp_time(char * st)
{
	char	buff[10];
	struct tm 	tmbuf;

	memset((char *)&tmbuf, 0x0, sizeof(tmbuf));

	memset(buff, 0x0, sizeof(buff));
	strncpy( buff, st, 4 );	

	tmbuf.tm_year = atoi(buff) - 1900;

	memset(buff, 0x0, sizeof(buff));
	strncpy(buff, st + 4, 2 );
	tmbuf.tm_mon = atoi(buff) -1;

	memset(buff, 0x0, sizeof(buff));
	strncpy(buff, st + 6, 2 );
	tmbuf.tm_mday = atoi(buff);

	memset(buff, 0x0, sizeof(buff));
	strncpy(buff, st + 8, 2 );
	tmbuf.tm_hour = atoi(buff);

	memset(buff, 0x0, sizeof(buff));
	strncpy(buff, st + 10, 2 );
	tmbuf.tm_min = atoi(buff);

	memset(buff, 0x0, sizeof(buff));
	strncpy(buff, st + 12, 2 );

	tmbuf.tm_sec = atoi(buff);
	tmbuf.tm_isdst = 0;

	return mktime(&tmbuf); 
}


int timeout()
{
	return __alarm_flag;
}

void __alarm_rtn(int s)
{
	__alarm_flag = 1;
	alarm(0);
}

int set_timeout(int sec)
{
	signal(14, __alarm_rtn);
	alarm(sec);
	__alarm_flag = 0;
	return 0;
}
