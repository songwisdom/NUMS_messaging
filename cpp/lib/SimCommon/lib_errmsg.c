#include <stdio.h>
#include <string.h>
#include <errno.h>
/*
char * get_err_msg(int eno)
{
	static	char	errmsg [ 50 ];

	memset(errmsg, 0x0, sizeof(errmsg));
	switch ( eno ) {
		case EPERM		:strcpy(errmsg, " Operation not permitted "); break;
		case ENOENT		:strcpy(errmsg, " No such file or directory "); break;
		case ESRCH		:strcpy(errmsg, " No such process "); break;
		case EINTR		:strcpy(errmsg, " Interrupted system call "); break;
		case EIO		:strcpy(errmsg, " I/O error "); break;
		case ENXIO		:strcpy(errmsg, " No such device or address "); break;
		case E2BIG		:strcpy(errmsg, " Arg list too long "); break;
		case ENOEXEC		:strcpy(errmsg, " Exec format error "); break;
		case EBADF		:strcpy(errmsg, " Bad file number "); break;
		case ECHILD		:strcpy(errmsg, " No child processes "); break;
		case EAGAIN		:strcpy(errmsg, " Try again "); break;
		case ENOMEM		:strcpy(errmsg, " Out of memory "); break;
		case EACCES		:strcpy(errmsg, " Permission denied "); break;
		case EFAULT		:strcpy(errmsg, " Bad address "); break;
		case ENOTBLK		:strcpy(errmsg, " Block device required "); break;
		case EBUSY		:strcpy(errmsg, " Device or resource busy "); break;
		case EEXIST		:strcpy(errmsg, " File exists "); break;
		case EXDEV		:strcpy(errmsg, " Cross-device link "); break;
		case ENODEV		:strcpy(errmsg, " No such device "); break;
		case ENOTDIR		:strcpy(errmsg, " Not a directory "); break;
		case EISDIR		:strcpy(errmsg, " Is a directory "); break;
		case EINVAL		:strcpy(errmsg, " Invalid argument "); break;
		case ENFILE		:strcpy(errmsg, " File table overflow "); break;
		case EMFILE		:strcpy(errmsg, " Too many open files "); break;
		case ENOTTY		:strcpy(errmsg, " Not a typewriter "); break;
		case ETXTBSY		:strcpy(errmsg, " Text file busy "); break;
		case EFBIG		:strcpy(errmsg, " File too large "); break;
		case ENOSPC		:strcpy(errmsg, " No space left on device "); break;
		case ESPIPE		:strcpy(errmsg, " Illegal seek "); break;
		case EROFS		:strcpy(errmsg, " Read-only file system "); break;
		case EMLINK		:strcpy(errmsg, " Too many links "); break;
		case EPIPE		:strcpy(errmsg, " Broken pipe "); break;
		case EDOM		:strcpy(errmsg, " Math argument out of domain of func "); break;
		case ERANGE		:strcpy(errmsg, " Math result not representable "); break;
		case EDEADLK		:strcpy(errmsg, " Resource deadlock would occur "); break;
		case ENAMETOOLONG	:strcpy(errmsg, " File name too long "); break;
		case ENOLCK		:strcpy(errmsg, " No record locks available "); break;
		case ENOSYS		:strcpy(errmsg, " Function not implemented "); break;
		case ENOTEMPTY		:strcpy(errmsg, " Directory not empty "); break;
		case ELOOP		:strcpy(errmsg, " Too many symbolic links encountered "); break;
//		case EWOULDBLOCK	:strcpy(errmsg, " Operation would block "); break;
		case ENOMSG		:strcpy(errmsg, " No message of desired type "); break;
		case EIDRM		:strcpy(errmsg, " Identifier removed "); break;
		case ECHRNG		:strcpy(errmsg, " Channel number out of range "); break;
		case EL2NSYNC		:strcpy(errmsg, " Level 2 not synchronized "); break;
		case EL3HLT		:strcpy(errmsg, " Level 3 halted "); break;
		case EL3RST		:strcpy(errmsg, " Level 3 reset "); break;
		case ELNRNG		:strcpy(errmsg, " Link number out of range "); break;
		case EUNATCH		:strcpy(errmsg, " Protocol driver not attached "); break;
		case ENOCSI		:strcpy(errmsg, " No CSI structure available "); break;
		case EL2HLT		:strcpy(errmsg, " Level 2 halted "); break;
		case EBADE		:strcpy(errmsg, " Invalid exchange "); break;
		case EBADR		:strcpy(errmsg, " Invalid request descriptor "); break;
		case EXFULL		:strcpy(errmsg, " Exchange full "); break;
		case ENOANO		:strcpy(errmsg, " No anode "); break;
		case EBADRQC		:strcpy(errmsg, " Invalid request code "); break;
		case EBADSLT		:strcpy(errmsg, " Invalid slot "); break;
//		case EDEADLOCK		:strcpy(errmsg, " EDEADLOCK "); break;
		case EBFONT		:strcpy(errmsg, " Bad font file format "); break;
		case ENOSTR		:strcpy(errmsg, " Device not a stream "); break;
		case ENODATA		:strcpy(errmsg, " No data available "); break;
		case ETIME		:strcpy(errmsg, " Timer expired "); break;
		case ENOSR		:strcpy(errmsg, " Out of streams resources "); break;
		case ENONET		:strcpy(errmsg, " Machine is not on the network "); break;
		case ENOPKG		:strcpy(errmsg, " Package not installed "); break;
		case EREMOTE		:strcpy(errmsg, " Object is remote "); break;
		case ENOLINK		:strcpy(errmsg, " Link has been severed "); break;
		case EADV		:strcpy(errmsg, " Advertise error "); break;
		case ESRMNT		:strcpy(errmsg, " Srmount error "); break;
		case ECOMM		:strcpy(errmsg, " Communication error on send "); break;
		case EPROTO		:strcpy(errmsg, " Protocol error "); break;
		case EMULTIHOP		:strcpy(errmsg, " Multihop attempted "); break;
		case EDOTDOT		:strcpy(errmsg, " RFS specific error "); break;
		case EBADMSG		:strcpy(errmsg, " Not a data message "); break;
		case EOVERFLOW		:strcpy(errmsg, " Value too large for defined data type "); break;
		case ENOTUNIQ		:strcpy(errmsg, " Name not unique on network "); break;
		case EBADFD		:strcpy(errmsg, " File descriptor in bad state "); break;
		case EREMCHG		:strcpy(errmsg, " Remote address changed "); break;
		case ELIBACC		:strcpy(errmsg, " Can not access a needed shared library "); break;
		case ELIBBAD		:strcpy(errmsg, " Accessing a corrupted shared library "); break;
		case ELIBSCN		:strcpy(errmsg, " .lib section in a.out corrupted "); break;
		case ELIBMAX		:strcpy(errmsg, " Attempting to link in too many shared libraries "); break;
		case ELIBEXEC		:strcpy(errmsg, " Cannot exec a shared library directly "); break;
		case EILSEQ		:strcpy(errmsg, " Illegal byte sequence "); break;
		case ERESTART		:strcpy(errmsg, " Interrupted system call should be restarted "); break;
		case ESTRPIPE		:strcpy(errmsg, " Streams pipe error "); break;
		case EUSERS		:strcpy(errmsg, " Too many users "); break;
		case ENOTSOCK		:strcpy(errmsg, " Socket operation on non-socket "); break;
		case EDESTADDRREQ	:strcpy(errmsg, " Destination address required "); break;
		case EMSGSIZE		:strcpy(errmsg, " Message too long "); break;
		case EPROTOTYPE		:strcpy(errmsg, " Protocol wrong type for socket "); break;
		case ENOPROTOOPT	:strcpy(errmsg, " Protocol not available "); break;
		case EPROTONOSUPPORT	:strcpy(errmsg, " Protocol not supported "); break;
		case ESOCKTNOSUPPORT	:strcpy(errmsg, " Socket type not supported "); break;
		case EOPNOTSUPP		:strcpy(errmsg, " Operation not supported on transport endpoint "); break;
		case EPFNOSUPPORT	:strcpy(errmsg, " Protocol family not supported "); break;
		case EAFNOSUPPORT	:strcpy(errmsg, " Address family not supported by protocol "); break;
		case EADDRINUSE		:strcpy(errmsg, " Address already in use "); break;
		case EADDRNOTAVAIL	:strcpy(errmsg, " Cannot assign requested address "); break;
		case ENETDOWN		:strcpy(errmsg, " Network is down "); break;
		case ENETUNREACH	:strcpy(errmsg, " Network is unreachable "); break;
		case ENETRESET		:strcpy(errmsg, " Network dropped connection because of reset "); break;
		case ECONNABORTED	:strcpy(errmsg, " Software caused connection abort "); break;
		case ECONNRESET		:strcpy(errmsg, " Connection reset by peer "); break;
		case ENOBUFS		:strcpy(errmsg, " No buffer space available "); break;
		case EISCONN		:strcpy(errmsg, " Transport endpoint is already connected "); break;
		case ENOTCONN		:strcpy(errmsg, " Transport endpoint is not connected "); break;
		case ESHUTDOWN		:strcpy(errmsg, " Cannot send after transport endpoint shutdown "); break;
		case ETOOMANYREFS	:strcpy(errmsg, " Too many references: cannot splice "); break;
		case ETIMEDOUT		:strcpy(errmsg, " Connection timed out "); break;
		case ECONNREFUSED	:strcpy(errmsg, " Connection refused "); break;
		case EHOSTDOWN		:strcpy(errmsg, " Host is down "); break;
		case EHOSTUNREACH	:strcpy(errmsg, " No route to host "); break;
		case EALREADY		:strcpy(errmsg, " Operation already in progress "); break;
		case EINPROGRESS	:strcpy(errmsg, " Operation now in progress "); break;
		case ESTALE		:strcpy(errmsg, " Stale NFS file handle "); break;
		case EUCLEAN		:strcpy(errmsg, " Structure needs cleaning "); break;
		case ENOTNAM		:strcpy(errmsg, " Not a XENIX named type file "); break;
		case ENAVAIL		:strcpy(errmsg, " No XENIX semaphores available "); break;
		case EISNAM		:strcpy(errmsg, " Is a named type file "); break;
		case EREMOTEIO		:strcpy(errmsg, " Remote I/O error "); break;
		case EDQUOT		:strcpy(errmsg, " Quota exceeded "); break;
		case ENOMEDIUM		:strcpy(errmsg, " No medium found "); break;
		case EMEDIUMTYPE	:strcpy(errmsg, " Wrong medium type "); break;
		default : strcpy(errmsg, "Unknown Error Message");
	}

	return errmsg;
}
*/
char * get_err_msg(int eno)
{
#if 0
	static	char	errmsg [ 128 ];

	memset(errmsg, 0x0, sizeof(errmsg));
	switch ( eno ) {
#ifdef _INCLUDE__STDC__
		case  	EDOM			: strcpy( errmsg, "Math arg out of domain of func"); break; 
		case  	ERANGE		: strcpy( errmsg, "Math result not representable"); break; 
#endif /* _INCLUDE__STDC__ */

#ifdef _INCLUDE_POSIX_SOURCE
		case 	EPERM			: strcpy( errmsg, "Not super-user	"); break;	
		case 	ENOENT			: strcpy( errmsg, "No such file or directory"); break;	
		case 	ESRCH			: strcpy( errmsg, "No such process	"); break;	
		case 	EINTR			: strcpy( errmsg, "interrupted system call"); break;	
		case 	EIO				: strcpy( errmsg, "I/O error		"); break;	
		case 	ENXIO		: strcpy( errmsg, "No such device or address"); break;	
		case 	E2BIG		   	: strcpy( errmsg, "Arg list too long	"); break;	
		case 	ENOEXEC		: strcpy( errmsg, "Exec format error	"); break;	
		case 	EBADF		: strcpy( errmsg, "Bad file number	"); break;	
		case 	ECHILD		: strcpy( errmsg, "No children		"); break;	
		case 	EAGAIN		: strcpy( errmsg, "No more processes	"); break;	
		case 	ENOMEM		: strcpy( errmsg, "Not enough core	"); break;	
		case 	EACCES		    : strcpy( errmsg, "Permission denied	"); break;	
		case 	EFAULT		: strcpy( errmsg, "Bad address		"); break;	
		case 	EBUSY		: strcpy( errmsg, "Mount device busy	"); break;	
		case 	EEXIST		: strcpy( errmsg, "File exists		"); break;	
		case 	EXDEV		: strcpy( errmsg, "Cross-device link	"); break;	
		case 	ENODEV		: strcpy( errmsg, "No such device	"); break;	
		case 	ENOTDIR		: strcpy( errmsg, "Not a directory	"); break;	
		case 	EISDIR		: strcpy( errmsg, "Is a directory	"); break;	
		case 	EINVAL		: strcpy( errmsg, "Invalid argument	"); break;	
		case 	ENFILE		: strcpy( errmsg, "File table overflow	"); break;	
		case 	EMFILE		: strcpy( errmsg, "Too many open files	"); break;	
		case 	ENOTTY		: strcpy( errmsg, "Not a typewriter	"); break;	
		case 	EFBIG		: strcpy( errmsg, "File too large	"); break;	
		case 	ENOSPC		: strcpy( errmsg, "No space left on device"); break;	
		case 	ESPIPE		: strcpy( errmsg, "Illegal seek		"); break;	
		case 	EROFS		: strcpy( errmsg, "Read only file system"); break;	
		case 	EMLINK		: strcpy( errmsg, "Too many links	"); break;	
		case 	EPIPE		: strcpy( errmsg, "Broken pipe		"); break;	
		case  EDEADLK 		: strcpy( errmsg, "A deadlock would occur"); break;	
		case 	ENOLCK  	: strcpy( errmsg, "System record lock table was full"); break; 
		case  EILSEQ		: strcpy( errmsg, "Illegal byte sequence       "); break; 
		case 	ENOTEMPTY   	: strcpy( errmsg, "Directory not empty         "); break; 
		case 	ENAMETOOLONG 	: strcpy( errmsg, "File name too long          "); break; 
		case 	ENOSYS 	  	     : strcpy( errmsg, "Function not implemented    "); break; 
#endif /* _INCLUDE_POSIX_SOURCE */

/* Things in POSIX.4 not in POSIX or ANSI C */
#ifdef _INCLUDE_POSIX4_SOURCE
		case  ENOTSUP			: strcpy( errmsg, "Function not implemented    "); break; 
		case  ECANCELED		: strcpy( errmsg, "aio request was canceled before complete"); break; 
#endif /* _INCLUDE_POSIX4_SOURCE */

/* Things in XPG3 not in POSIX or ANSI C */
#ifdef _INCLUDE_XOPEN_SOURCE
		case 	ENOTBLK			: strcpy( errmsg, "Block device required"); break;	
		case 	ETXTBSY			: strcpy( errmsg, "Text file busy	"); break;	
#if defined( _INCLUDE_XOPEN_SOURCE_EXTENDED) || ! defined( _INCLUDE_AES_SOURCE)
		case 	ENOMSG  	      : strcpy( errmsg, "No message of desired type  "); break; 
		case 	EIDRM			: strcpy( errmsg, "Identifier removed	"); break;	
#endif /* defined( _INCLUDE_XOPEN_SOURCE_EXTENDED) || ! defined( _INCLUDE_AES_SOURCE) */
#endif /* _INCLUDE_XOPEN_SOURCE */

/* Things in AES not in  XPG3, POSIX or ANSI C */
#if defined( _INCLUDE_AES_SOURCE) || defined( _INCLUDE_XOPEN_SOURCE)
		case 	ELOOP	    		: strcpy( errmsg, "Too many levels of symbolic links"); break; 
#endif /* _INCLUDE_AES_SOURCE */

#ifdef _INCLUDE_XOPEN_SOURCE_EXTENDED 
/* ipc/network software */

/* argument errors */
		case  ENOTSOCK			: strcpy( errmsg, "Socket operation on non-socket"); break; 
		case  EDESTADDRREQ		: strcpy( errmsg, "Destination address required"); break; 
		case  EMSGSIZE			: strcpy( errmsg, "Message too long"); break; 
		case  EPROTOTYPE			: strcpy( errmsg, "Protocol wrong type for socket"); break; 
		case  ENOPROTOOPT			: strcpy( errmsg, "Protocol not available"); break; 
		case  EPROTONOSUPPORT		: strcpy( errmsg, "Protocol not supported"); break; 
		case  EOPNOTSUPP	 		: strcpy( errmsg, "Operation not supported"); break; 
		case  EAFNOSUPPORT 		 	: strcpy( errmsg,"Address family not supported by protocol famil"); break;
		case  EADDRINUSE			: strcpy( errmsg, "Address already in use"); break; 
		case  EADDRNOTAVAIL 		: strcpy( errmsg, "Can't assign requested address"); break; 

      /* operational errors */
		case  ENETDOWN			: strcpy( errmsg, "Network is down"); break; 
		case  ENETUNREACH			: strcpy( errmsg, "Network is unreachable"); break; 
		case  ECONNABORTED			: strcpy( errmsg, "Software caused connection abort"); break; 
		case  ECONNRESET			: strcpy( errmsg, "Connection reset by peer"); break; 
		case  ENOBUFS			: strcpy( errmsg, "No buffer space available"); break; 
		case  EISCONN			: strcpy( errmsg, "Socket is already connected"); break; 
		case  ENOTCONN			: strcpy( errmsg, "Socket is not connected"); break; 
		case  ETIMEDOUT			: strcpy( errmsg, "Connection timed out"); break; 
		case  ECONNREFUSED		: strcpy( errmsg, "Connection refused"); break; 
		case  EHOSTUNREACH		: strcpy( errmsg, "No route to host"); break; 

		case 	EALREADY    	: strcpy( errmsg, "Operation already in progress"); break; 
		case 	EINPROGRESS 	: strcpy( errmsg, "Operation now in progress"); break; 
		case 	EWOULDBLOCK 	: strcpy( errmsg, "Operation would block"); break; 

		case  ENODATA			: strcpy( errmsg, "no data (for no delay io)	"); break;	
		case  ETIME			: strcpy( errmsg, "timer expired		"); break;	
		case  ENOSR			: strcpy( errmsg, "out of streams resources	"); break;	
		case  ENOSTR		: strcpy( errmsg, "Device not a stream          "); break;	
		case  ENOLINK		: strcpy( errmsg, "the link has been severed"); break; 
		case  EPROTO		: strcpy( errmsg, "Protocol error		"); break;	
		case 	EMULTIHOP 	: strcpy( errmsg, "multihop attempted"); break; 
		case  EBADMSG 		: strcpy( errmsg, "trying to read unreadable message"); break;	
		case	EDQUOT	 	: strcpy( errmsg, "Disc quota exceeded 	"); break;	
		case	ESTALE		:	strcpy( errmsg, " Stale NFS file handle"); break;
/* Reserved Value - use this number when adding EOVERFLOW */
		case 	EOVERFLOW		: strcpy( errmsg,"Value too large to be stored in data type"); break; 
#endif /* _INCLUDE_XOPEN_SOURCE_EXTENDED */

/* Things in HP-UX not in XPG3, POSIX or ANSI C */
#ifdef _INCLUDE_HPUX_SOURCE
/* The error numbers between 37 and 44 are not produced by HP-UX. They
   will track whatever the UNIX(tm) system does in the future */
		case 	ECHRNG	: strcpy( errmsg, "Channel number out of range	"); break;	
		case 	EL2NSYNC 	: strcpy( errmsg, "Level 2 not synchronized	"); break;	
		case 	EL3HLT		: strcpy( errmsg, "Level 3 halted		"); break;	
		case 	EL3RST		: strcpy( errmsg, "Level 3 reset		"); break;	
		case 	ELNRNG		: strcpy( errmsg, "Link number out of range	"); break;	
		case 	EUNATCH 	: strcpy( errmsg, "Protocol driver not attached	"); break;	
		case 	ENOCSI		: strcpy( errmsg, "No CSI structure available	"); break;	
		case 	EL2HLT		: strcpy( errmsg, "Level 2 halted		"); break;	

		case  ENONET		: strcpy( errmsg, "Machine is not on the network"); break;	
		case  ENOPKG		: strcpy( errmsg, "Package not installed               "); break; 
		case  EADV			: strcpy( errmsg, "advertise error"); break; 
		case  ESRMNT	: strcpy( errmsg, "srmount error"); break; 
		case 	ECOMM	: strcpy( errmsg, "Communication error on send	"); break;	
		case 	EDOTDOT : strcpy( errmsg, "Cross mount point (not really error"); break;


		case 	ENOSYM		: strcpy( errmsg, "symbol does not exist in executable "); break; 

/* disk quotas errors */
		case 	EUSERS	 	: strcpy( errmsg, "For Sun compatibilty, will not occur"); break;

/* Network File System */
		case 	EREMOTE		: strcpy( errmsg, "Too many levels of remote in path"); break; 

/* ipc/network software */

/* argument errors */
		case  ESOCKTNOSUPPORT	: strcpy( errmsg, "Socket type not supported"); break; 
		case  EPFNOSUPPORT 		: strcpy( errmsg, "Protocol family not supported"); break; 

	/* operational errors */
		case  ENETRESET			: strcpy( errmsg, "Network dropped connection on reset"); break; 
		case  ESHUTDOWN			: strcpy( errmsg, "Can't send after socket shutdown"); break; 
		case  ETOOMANYREFS		: strcpy( errmsg, "Too many references: can't splice"); break; 
		//case  EREFUSED			: strcpy( errmsg, "Double define for NF"); break;
		case  EREMOTERELEASE	: strcpy( errmsg, "Remote peer released connection"); break; 
		case  EHOSTDOWN			: strcpy( errmsg, "Host is down"); break; 


#endif /* _INCLUDE_HPUX_SOURCE */

#ifndef NO_DLKM

/* Attempt to match UnixWare errnos where applicable */

		case  ENOLOAD		: strcpy( errmsg, "Cannot load required kernel module"); break; 
		case  ERELOC		: strcpy( errmsg, "Object file error in loading kernel module"); break; 
		case  ENOMATCH		: strcpy( errmsg, "Symbol matching given spec not found"); break; 
		case  ENOUNLD		: strcpy( errmsg, "Cannot unload kernel module"); break; 
		case  EBADVER		: strcpy( errmsg, "Version number mismatch for loadable"); break; 
		case  ECONFIG		: strcpy( errmsg, "Configured kernle resource exhausted"); break; 
		case  ENOREG		: strcpy( errmsg, "Cannot register required kernel module"); break; 
		case  ENOUNREG		: strcpy( errmsg, "Cannot unregister kernel module"); break; 

#endif /* NO_DLKM */
		default : strcpy(errmsg, "Unknown Error Message");
	}

	return errmsg;
	///

	//
#endif
	return strerror(errno);
}
