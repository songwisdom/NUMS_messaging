/***********************************************************
*
* $Header: /home/cvs/smsc/skt-smsc/include/ksig7/ainCppCommon.h,v 1.1 2008/07/21 05:12:48 prowoni Exp $
*
* $Source: /home/cvs/smsc/skt-smsc/include/ksig7/ainCppCommon.h,v $
* $Revision: 1.1 $
* $Locker:  $
*
***********************************************************/
/* basic definitions of types and manifest constants for C++ objects */

#ifndef AINCPPCOMMON_H
#define AINCPPCOMMON_H

#ifdef _WIN32
#include <windows.h>
#undef FAR 
#endif /* _WIN32 */

#include <sys/types.h>


#ifdef __cplusplus
#include <iostream.h>

#ifdef _WIN32
#  ifndef HPAIN_HPAINAPI
#      define HPAIN_HPAINAPI __declspec(dllimport)
#  endif
#else
#  define HPAIN_HPAINAPI
#endif /* _WIN32 */

// Class scope used until namescopes available
class HPAIN_HPAINAPI HPAIN
{
  public:
  //--------------------------------------------------------------------------
  // Common C++ AIN types for HPAIN APIs
  //--------------------------------------------------------------------------

  typedef short		Int16;
  typedef int		Int32;
  typedef unsigned char	Uint8;
  typedef unsigned short	Uint16;
// GREhp18411 : Keep Uint32 32 bits wide even in 64 bits.
#ifdef __LP64__
  typedef unsigned int	Uint32;
#else
  typedef unsigned long	Uint32;
#endif /* __LP64__ */
// end GREhp18411
  typedef unsigned char	Byte;
  typedef struct iovec 	IoVec;

  enum Boolean { bFalse=0, bTrue=1 };

  //--------------------------------------------------------------------------
  // Status base class
  //--------------------------------------------------------------------------

  // Note the implicit 
  // int/enum casts cunningly hidden in the StatusValue typedef
  class StatusBase 
	  {
		public:
		typedef int StatusValue;
#ifdef _WIN32
#  ifdef NO_ERROR
#     if ((NO_ERROR) != 0L) && ((NO_ERROR) != 0)
#       error NO_ERROR must be set to 0
#     else
#       undef NO_ERROR
#     endif
#  endif
#endif /* _WIN32 */
		enum Status {NO_ERROR=0};
	
		// get enum value (used by derived classes too)
		StatusValue getStatus() const {return theStatus;}
		// is this status a temporary setback (ie an error and not fatal)?
		// If the derived  status object only has fatal errors it can just use this version
		virtual Boolean isRecoverable() {return bFalse; }
		// is this status an error? (note: Derived classes should redefine this)
		virtual Boolean isOk() const {return (theStatus==NO_ERROR) ? bTrue : bFalse;}

                // allow to reset the status value
                virtual void reset() {theStatus=NO_ERROR;}
	
		// o/p of localised user readable error string
		friend ostream& operator<< (ostream& os, StatusBase) { os << "No error\n"; return os; }

		protected:
		// only derived classes can make it
		StatusBase(StatusValue s): theStatus(s) { }
		// Default ctor (to allow passing in to calls)
		StatusBase(): theStatus(NO_ERROR) {}		
		// all base classes should have virtual dtors
		virtual ~StatusBase() {}
		// only derived classes can explicitly set the value
		// This is virtual as apis will set the status in passed derived objects
		// by using it. To create special behaviour (eg stacks), it is possible
		// to inherit from the api's status object and make this fn perform the
		// specific behaviour required.
		virtual void setStatus(StatusValue s) { theStatus=s; }

		private:
		// derived classes must use the ctor to set this,and the
		// accessor to get it.
		StatusValue theStatus;
	  };

  //--------------------------------------------------------------------------
  // Schedulable base class
  //--------------------------------------------------------------------------

  class  Schedulable 
	  {
           public:
           Schedulable() {};
           virtual ~Schedulable() {};

           typedef int Work;
           enum {UNLIMITED=-2,URGENT=-1,NONE=0};

           // schedular i/f 
#ifdef _WIN32
           virtual int preWaitForMultipleObjects( DWORD * numHdl,
                                   DWORD,
                                   HANDLE *, 
                                   struct timeval *&) { return *numHdl; };

           virtual Work handler( DWORD ,
								 HANDLE *,
								 DWORD) { return NONE; }
#else
           virtual int selectMask( int maxFd,
                                   fd_set *,
                                   fd_set *,
                                   fd_set *,
                                   struct timeval *& ) { return maxFd; }

           virtual Work handler( int ,
                                 fd_set *,
                                 fd_set *,
                                 fd_set * ) { return NONE; }
#endif /* _WIN32 */

           virtual Work doWork( Work P_work ) = 0;
		
           private:

           Schedulable(const Schedulable&);		// No copy ctor
           const Schedulable& operator=(const Schedulable&);
          };

};		// end of HPAIN class

#endif /* __cplusplus */
#endif           /* AINCPPCOMMON_H */






