// begin_is
//----------------------------------------------------------------------------
//
//                               MODULE   PLUGIN_Common
//                               ----------------------
//
//
//  PURPOSE: 
//
//  NOTES and CAUTIONS:
//
//  FILE : PLUGIN_Common.h
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
//  Rev: $Header: /home/cvs/smsc/skt-smsc/include/ksig7/PLUGIN_Common.h,v 1.1 2008/07/21 05:12:49 prowoni Exp $
//       $Source: /home/cvs/smsc/skt-smsc/include/ksig7/PLUGIN_Common.h,v $
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


#ifndef __PLUGIN_COMMON_H
#define __PLUGIN_COMMON_H


class PLUGIN {
public:
   //--------------------------------------------------------------------------
   // Process states as reported to and by the Fault Tolerant Controller
   //--------------------------------------------------------------------------
   
   enum ProcessState {
      FTC_UNKNOWN=0,
      FTC_DOWN,
      FTC_BOOTING,
      FTC_COLD_STANDBY,
      FTC_SYNCHRONISING,
      FTC_HOT_STANDBY,
      FTC_SWITCHING,
      FTC_STOPPING,
      FTC_ACTIVE,
      FTC_MIXED,
      FTC_INVALID_STATE
   };

   static char* string(ProcessState P_state)
   {
      switch (P_state) {
         case FTC_UNKNOWN:       return "UNKNOWN";
         case FTC_DOWN:          return "DOWN";
         case FTC_BOOTING:       return "BOOTING";
         case FTC_COLD_STANDBY:  return "COLD STANDBY";
         case FTC_SYNCHRONISING: return "SYNCHRONISING";
         case FTC_HOT_STANDBY:   return "HOT STANDBY";
         case FTC_SWITCHING:     return "SWITCHING";
         case FTC_STOPPING:      return "STOPPING";
         case FTC_ACTIVE:        return "ACTIVE";
         case FTC_MIXED:         return "MIXED";
         case FTC_INVALID_STATE: return "INVALID";
         default:                return "ERROR";
      }
   }
};

#endif // __PLUGIN_COMMON_H

