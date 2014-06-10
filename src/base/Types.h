/*
 * type.h
 *
 *  Created on: 2013-11-03
 *      Author: kin
 */

#ifndef __TYPE_H__
#define __TYPE_H__

#define _LINUX_

#if defined(_LINUX_) || defined(_FREEBSD_) || defined(_CYGWIN_) || defined(_DARWIN_)
#define _UNIX_
#endif


#ifdef _UNIX_
	#if (((defined _DEBUG) || (defined DEBUG)) && !(defined _DEBUG_))
	  # define _DEBUG_
	#endif
  #ifndef _GNU_SOURCE
  #define _GNU_SOURCE
  #endif


  #include <stdlib.h>
  #include <sys/types.h>
  #include <ctype.h>

  typedef int8_t    int8;
  typedef int16_t   int16;
  typedef int32_t   int32;
  typedef int64_t   int64;
  typedef u_int8_t  uint8;
  typedef u_int16_t uint16;
  typedef u_int32_t uint32;
  typedef u_int64_t uint64;


  #define NUI_API

#endif // _UNIX_


#endif /* __TYPE_H__ */
