#ifndef _MAKE_MSGC_syncplat_h_ID_
#define _MAKE_MSGC_syncplat_h_ID_
#if defined(__sgi) && defined(_SGI_COMPILER_VERSION)
#pragma weak _MAKE_MSGC_syncplat_h_id_w = _MAKE_MSGC_syncplat_h_id_
#endif /* __sgi */
static const char* _MAKE_MSGC_syncplat_h_id_ 
#if defined(__GNUC__)
__attribute__ ((used,unused)) 
#endif /* __GNUC__ */
= "@(#)$Id: syncplat.h 55141 2009-08-18 21:17:30Z wguan@syncsort.com $";
#endif

/******************************************************************************
 
                      Syncsort CLIENT-SERVER BACKUP
 
                            COPYRIGHT(C) 1994
 
        Module Name             : gen
 
        Component Name          : include
 
        Program Filename        : syncplat.h
 
        General Description     : define hierarchical macros for systems
 
        System Dependencies     : NONE
 
        History Information     : Provided by the RCS package in the following.
 
 * $Log: not supported by cvs2svn $
 * Revision 1.1  2008/07/18 05:00:46  hsong
 * Initial revision
 *
 * Revision 1.34  2008-06-20 09:31:02-04  wtwong
 * I..5-028: support hp11 v23 64 bits platforms
 *
 * Revision 1.33  2008/05/31 22:05:39  hsong
 * I..5-133: Bug 2130: fixed typedef of in_addr_t.
 *
 * Revision 1.32  2008/05/27 19:40:55  wtwong
 * I..5-028: support hp11-i23 and sun58 platform
 *
 * Revision 1.31  2007/10/29 20:39:36  ycao
 * I..4-902: intel mac is little endian
 *
 * Revision 1.30  2007/10/26 15:57:26  ycao
 * I..4-902: Support Intel Mac
 *
 * Revision 1.29  2007/10/09 20:21:43  hsong
 * I..4-875: Build platform Linuxoes-amd64
 *
 * Revision 1.28  2007/04/27 15:05:10  hyan
 * I..4-540: Build sun10-amd64 M/S
 *
 * Revision 1.27  2007/04/19 20:53:48  hyan
 * I..4-540: Build sun10-amd64 M/S
 *
 * Revision 1.26  2007/03/16 14:04:54  wguan
 * T...-269: y
 *
 * Revision 1.25  2006/10/17 15:05:31  xhu
 * I..4-118 introduce new platform win-amd64 to support master
 *
 * Revision 1.24  2006/10/09 15:26:40  ajesh
 * I..4-101: _SYNC_WINXP defines _SYNC_WIN2K
 *
 * Revision 1.23  2006/05/15 13:45:32  dzaninov
 * I..3-677: VXFS support integration into snap and nibbler
 *
 * Revision 1.22  2006/05/09 20:10:13  xhu
 * I..3-754 port Linux AMD 64 master support to dev
 *
 * Revision 1.21  2006/01/05 19:18:18  hyan
 * I..3-255: support build db on Linux-oes
 *
 * Revision 1.20  2005/03/02 18:29:22  pu
 * removes SYNC_HPUX_11_IA64 from 64bit_OPSYS
 *
 * Revision 1.19  2005/03/02 16:52:59  pu
 * removes SYNC_HPUX_11_IA64 from 64bit_OPSYS
 *
 * Revision 1.18  2005/02/16 21:29:35  pu
 * Add SYNC_HPUX_11_IA64
 *
 * Revision 1.17  2005/01/18 20:41:37  xhu
 * port to linux ia64 platform
 *
 * Revision 1.16  2004/12/17 16:06:06  dzaninov
 * I..2-672: DB2 restore does not work on AIX 5 64-bit
 *
 * Revision 1.15  2004/09/08 20:06:13  xhu
 * Support for Mac OS X
 *
 * Revision 1.14  2003/10/08 19:16:00  dzaninov
 * I..1-617: Importing 215D changes into DEV
 *
 * Revision 1.13  2003/07/25 18:32:25  urano
 * I..1-484: dev fixes
 *
 * Revision 1.12  2003/07/15 18:29:51  urano
 * I...-674: Device Management Module
 *
 * Revision 1.11  2003/05/19 20:37:26  ranjith
 * I..1-444: Windows 64 bit support
 *
 * Revision 1.10  2003/05/19 20:34:15  ranjith
 * I..1-444: Windows 64 bit support
 *
 * Revision 1.9  2003/05/08 17:01:49  dzaninov
 * I..1-617: Importing 215D changes into DEV
 *
 * Revision 1.8  2002/12/16 20:59:55  georgel
 * I..1-329: Reserve/Release for Tape IO
 *
 * Revision 1.7  2002/06/13 15:31:02  urano
 * I..1-270: Port original dev release issues to new dev (R215 base)
 *
 * Revision 1.6  2002/06/07 19:42:48  urano
 * I..1-264: Port original dev release issues to new dev (R215 base)
 *
 * Revision 1.5  2000/10/04 19:41:50  ranjith
 * I...-771: Port win2k changes from 212 to dev
 *
 * Revision 1.4  2000/09/28 15:44:40  ranjith
 * I...-829: FreeBSD Support for Backup Express
 *
 * Revision 1.3  1999/10/26 18:15:46  jay
 * I...-590: Raw backup: No data selected on IRIX 6.4 and above
 *
 * Revision 1.2  1999/09/13 22:42:55  urano
 * I...-592: cleaning syncsort.h
 *
*****************************************************************************/

#ifndef _SYNC_PLAT_H
#define _SYNC_PLAT_H

/* Define hierarchical macro for systems */

#ifdef _SYNC_HPUX_11_IA64
#define _SYNC_HPUX_11
#endif

/* I909 */
#ifdef _SYNC_HPUX_11
#define _SYNC_HPUX_1020
#endif

/* I-1259 support AIX 5 64 bits APPLICATION */
#if (defined _SYNC_AIX_32 || defined _SYNC_AIX_41 || defined _SYNC_AIX_43 || defined _SYNC_AIX_43_64 || defined _SYNC_AIX_5_64)
#define _SYNC_AIX
#endif

#if (defined _SYNC_HPUX_904 || defined _SYNC_HPUX_1001 || defined _SYNC_HPUX_1020 || defined _SYNC_HPUX_11 || defined _SYNC_HPUX_11_64 || defined _SYNC_HPUX_11_IA64 )
#define _SYNC_HPUX
#endif

#if (defined _SYNC_DGUX_54) || (defined _SYNC_DGUX_INTEL) 
#define _SYNC_DGUX
#endif

#ifdef _SYNC_OSF_51   /* I-1329 */
#define _SYNC_OSF_50
#endif

#ifdef _SYNC_OSF_50   /* I989 */
#define _SYNC_OSF_40
#endif

#if (defined _SYNC_OSF_32 || defined _SYNC_OSF_40)
#define _SYNC_OSF
#endif


#if (defined _SYNC_SCO_32)
#define _SYNC_SCO
#endif

#if (defined _SYNC_SCO_32)
#define _SYNC_SCO
#endif

#ifdef _SYNC_SOLARIS_58
#define _SYNC_SOLARIS_56
#endif

#ifdef _SYNC_OES_LINUX_AMD64
#define _SYNC_OES_LINUX
#endif
#if (defined _SYNC_SOLARIS_53 || defined _SYNC_SOLARIS_55 || defined _SYNC_SOLARIS_56 || defined _SYNC_SOLARIS_57 || defined _SYNC_SOLARIS_58 || defined _SYNC_SOLARIS_58_64)
#define _SYNC_SOLARIS_5
#endif


#if (defined _SYNC_SOLARIS_5 || defined _SYNC_SUNOS_41 || defined _SYNC_SOLARIS_PC || defined _SYNC_SOLARIS_10_AMD64)
#define _SYNC_SOLARIS
#endif


/* For other MIPS machines to come: 
 * add macros corresponding to other OpSys than IRIX_62:
 *	1- SINIX  from SIEMENS-NIXDORF
 */
#ifdef _SYNC_IRIX_65     /* I590 */
#define _SYNC_IRIX_62
#endif

#if (defined _SYNC_IRIX_53 || defined _SYNC_IRIX_62 || defined _SYNC_IRIX_65_64)
#define _SYNC_IRIX
#endif

#if (defined _SYNC_IRIX || defined _SYNC_SINIX)
#define _SYNC_MIPS
#endif


/* The following define is used to distinguish SUNOS from Solaris,
   but we still keep the above definition for general SUN workstation.
   ***** so please be very careful not to think _SYNC_SOLARIS only
         represent Solaris systems. *****                         */

#if (defined _SYNC_SUNOS_41)
#define _SYNC_SUNOS
#endif

#ifdef _SYNC_LINUX_24	/* I1266 */
#define _SYNC_LINUX
#endif

#ifdef _SYNC_LINUX_26
#define _SYNC_LINUX
#endif

#ifdef _SYNC_SOLARIS_10_AMD64  
#define _SYNC_SOLARIS_PC
#endif


/* I3754, port I3674
 * In case we find any fix for Linux IA64 which
 * does not work for Linux AMD64. We need to 
 * change this.
 */
#ifdef _SYNC_LINUX_AMD64
#define _SYNC_LINUX_IA64
#endif

#ifdef _SYNC_LINUX_IA64 /* I2149 */
#define _SYNC_LINUX
#endif

#ifdef _SYNC_IMACOSX /* I4902 */
#define _SYNC_MACOSX
#endif

/* The following one must be placed after all the unix system defines */
#if (defined _SYNC_AIX || defined _SYNC_HPUX || defined _SYNC_DGUX || defined _SYNC_LINUX || defined _SYNC_SCO || defined _SYNC_SOLARIS || defined _SYNC_OSF || defined _SYNC_NCR || defined _SYNC_MIPS || defined _SYNC_HP11 || defined _SYNC_FREE_BSD || defined_SYNC_AIX_43_64 || defined _SYNC_HPUX_11_64  || defined _SYNC_HPUX_11_IA64  || defined _SYNC_IRIX_65_64 || defined _SYNC_SOLARIS_58_64 || defined _SYNC_MACOSX) /* I-829 */
#define _SYNC_UNIX
#endif

#if (defined _SYNC_WIN31)
#define _SYNC_WINDOWS
#endif

/* I-1444, I-4118 */
#if (defined _SYNC_WIN64)  || (defined _SYNC_WIN_AMD64)
#define _SYNC_WIN2K
#endif


/* I-4101 */
#if (defined _SYNC_WINXP)
#define _SYNC_WIN2K
#endif

/* I-663 */
#if (defined _SYNC_WIN2K)
#define _SYNC_WINNT
#endif

#if (defined _SYNC_NETWARE_312) || (defined _SYNC_NETWARE_410)
#define _SYNC_NETWARE
#endif



/* Define 64bit machines */

/* I-1259 support AIX 5 64 bits APPLICATION */
#if defined (_SYNC_IRIX_62) || defined (_SYNC_HPUX_1020) || defined(_SYNC_SOLARIS_56) || defined (_SYNC_SOLARIS_58) || defined (_SYNC_SOLARIS_58_64) || defined (_SYNC_AIX_43) || defined (_SYNC_AIX_43_64) || defined (_SYNC_AIX_5_64) || defined (_SYNC_LINUX_IA64) 
#define _SYNC_64bit_OPSYS
#else
#undef  _SYNC_64bit_OPSYS

#endif

/* enforce not to use rlimit64,.... iand so on */
#ifdef _SYNC_HPUX_11_IA64
#undef  _SYNC_64bit_OPSYS
#endif


/* Define _SYNC_LIL_END for all little endian m/cs we support. */


 #if (defined _SYNC_SCO || defined _SYNC_LINUX || defined _SYNC_WINNT || defined _SYNC_WINDOWS || defined _SYNC_NETWARE || defined _SYNC_OSF || defined _SYNC_NCR || defined _SYNC_DGUX_INTEL || defined _SYNC_SOLARIS_PC || defined _SYNC_FREE_BSD || defined _SYNC_IMACOSX)

#define _SYNC_LIL_END

#endif

/* Define _SYNC_SORT_SHARED for OS using shared syncsort library */

#if (defined _SYNC_LINUX || defined _SYNC_SOLARIS_56 || defined _SYNC_SOLARIS_10_AMD64 || defined _SYNC_AIX_43 || defined _SYNC_AIX_5_64 || defined _SYNC_HPUX_11 || defined _SYNC_HPUX_1020 || defined _SYNC_WINNT || defined _SYNC_WIN2K || defined _SYNC_OSF || defined _SYNC_OES_LINUX) /* I876 & I909 & I1493 & I3255 */

#ifndef _SYNC_SORT_SHARED
#define _SYNC_SORT_SHARED
#define sort_isv_key 10759150
#endif

#endif /* I876 & I909 */

/* Define TIME_T_IS_INT for those platforms where time_t is typedef'ed as int (same as long_sy) */
#if (defined _SYNC_AIX_43 || defined _SYNC_OSF_40)
#define TIME_T_IS_INT
#endif


#endif /* _SYNC_PLAT_H */
