/*
 * Copyright (c) 1990, 2007 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
/* doc in sprintf.c */
/* This code created by modifying sprintf.c so copyright inherited. */

#include <_ansi.h>
#include <reent.h>
#include <stdio.h>
#ifdef _HAVE_STDC
#include <stdarg.h>
#else
#include <varargs.h>
#endif
#include <limits.h>
#include <errno.h>
#include "local.h"

int
#ifdef _HAVE_STDC
_DEFUN(_snprintf_r, (ptr, str, size, fmt),
       struct _reent *ptr _AND
       char *str          _AND
       size_t size        _AND
       _CONST char *fmt _DOTS)
#else
_snprintf_r(ptr, str, size, fmt, va_alist)
            struct _reent *ptr;
            char *str;
            size_t size;
            _CONST char *fmt;
            va_dcl
#endif
{
  int ret;
  va_list ap;
  FILE f;

  if (size > INT_MAX)
    {
      ptr->_errno = EOVERFLOW;
      return EOF;
    }
  f._flags = __SWR | __SSTR;
  f._bf._base = f._p = (unsigned char *) str;
  f._bf._size = f._w = (size > 0 ? size - 1 : 0);
  f._file = -1;  /* No file. */
#ifdef _HAVE_STDC
  va_start (ap, fmt);
#else
  va_start (ap);
#endif
  ret = _svfprintf_r (ptr, &f, fmt, ap);
  va_end (ap);
  if (ret < EOF)
    ptr->_errno = EOVERFLOW;
  if (size > 0)
    *f._p = 0;
  return (ret);
}

int
_EXFUN(_sniprintf_r, (struct _reent *, char *, size_t, const char *, ...)
       _ATTRIBUTE ((alias("_snprintf_r"))));
#ifndef _REENT_ONLY

int
#ifdef _HAVE_STDC
_DEFUN(snprintf, (str, size, fmt),
       char *str   _AND
       size_t size _AND
       _CONST char *fmt _DOTS)
#else
snprintf(str, size, fmt, va_alist)
         char *str;
         size_t size;
         _CONST char *fmt;
         va_dcl
#endif
{
  int ret;
  va_list ap;
  FILE f;
  struct _reent *ptr = _REENT;

  if (size > INT_MAX)
    {
      ptr->_errno = EOVERFLOW;
      return EOF;
    }
  f._flags = __SWR | __SSTR;
  f._bf._base = f._p = (unsigned char *) str;
  f._bf._size = f._w = (size > 0 ? size - 1 : 0);
  f._file = -1;  /* No file. */
#ifdef _HAVE_STDC
  va_start (ap, fmt);
#else
  va_start (ap);
#endif
  ret = _svfprintf_r (ptr, &f, fmt, ap);
  va_end (ap);
  if (ret < EOF)
    ptr->_errno = EOVERFLOW;
  if (size > 0)
    *f._p = 0;
  return (ret);
}

int
_EXFUN(sniprintf, (char *, size_t, const char *, ...)
       _ATTRIBUTE ((alias("snprintf"))));
#endif
