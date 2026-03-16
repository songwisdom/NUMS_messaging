// begin_is
//----------------------------------------------------------------------------
//
//                               MODULE   
//                               ----------------------
//
//
//  PURPOSE: 
//
//  NOTES and CAUTIONS:
//
//  FILE : 
//____________________________________________________________________________
//
// Copyright (c) 1996 Hewlett-Packard Company, All Rights Reserved.
// RESTRICTED RIGHTS LEGEND Use, duplication, or disclosure by the U.S.
// Government is subject to restrictions as set forth in sub-paragraph
// (c)(1)(ii) of the Rights in Technical Data and Computer Software
// clause in DFARS 252.227-7013.
//
// Hewlett-Packard Company
// 3000 Hanover Street
// Palo Alto, CA 94304 U.S.A.
// Rights for non-DOD U.S. Government Departments and Agencies are as
// set forth in FAR 52.227-19(c)(1,2).
//____________________________________________________________________________
//
//
//  AUTHOR:
//                                               DATE: 
//
//  Rev: $Header: /home/cvs/smsc/skt-smsc/include/ksig7/PlugIn.h,v 1.1 2008/07/21 05:12:47 prowoni Exp $
//       $Source: /home/cvs/smsc/skt-smsc/include/ksig7/PlugIn.h,v $
//       $Revision: 1.1 $
//       $Locker:  $
//----------------------------------------------------------------------------
//  HISTORY:
//
//  Date       Modification Location              Summary              Name
//  ----       ---------------------              -------              ----
//                First Version                                             
//----------------------------------------------------------------------------
// end_is

#if !defined(PLUGIN_H)
#define PLUGIN_H



#include <ainCommon.h>
#include <ainCppCommon.h>
#include <PLUGIN_Common.h>

/*
__________________________________________________________________________

              C L A S S     P l u g i n
__________________________________________________________________________
*/


/**
 * Main plug-in execution interface class.
 * This class is divided into virtual members that are intended to be re-fined
 * by the plug-in developer to implement the scheduling interface of the
 * plug-in, and non virtual members that offer some basic services to the
 * plug-in.
 * Only one object from that class is allocated by the user-defined piSetup()
 * function.
 *
 * @short Plugin execution interface.
 */
class PlugIn {
public:
     
   enum RequestState { EMPTY = 0, IDLE, PROCESSING , FAULT};
   
   virtual ~PlugIn();

 protected:
   PlugIn();

public:
       
   virtual int init(const int P_param = 0); // P_param is reserved for future use 
   virtual int selectMasks(
			   int * const P_maxFd,
			   fd_set* const P_rm,
			   fd_set* const P_wm,
			   fd_set* const P_em,
			   struct timeval* const P_timeout
			   ); // return -1 for FAULT
   
   virtual int connectionHandler(
				 const int P_nfds,
				 const fd_set * const  P_rm,
				 const fd_set * const  P_wm,
				 const fd_set * const  P_em
				 );  // -1 for FAULT
   virtual RequestState pendingRequest();   
   virtual RequestState processRequest(const int P_nRequests);  
   virtual RequestState newPicState(const PLUGIN::ProcessState P_newState);
   /* should return the same Req for ack, or FAULT for error */   
   
public:
   
   //void fault();
   const char * getProcessName();

};

// <<< Version number - WARNING: heading '0' are meaningful

#define PIC_PLUGIN_VERSION(s) s ## 10000001

#define PlugInSetup PIC_PLUGIN_VERSION(PlugInSetup)

extern "C" {

   /**
    * Plug-in main entry-point.
    * This function is to be defined for each plug-in.
    * It allocates the user plug-in object inheriting from the above PIC_Plugin
    * class.
    *
    * @return pointer to the user PIC_plugin object, or NULL in case of failure
    */
   extern PlugIn* PlugInSetup();
};


#endif // __PIC_PLUGIN_H
