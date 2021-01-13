#include <stdio.h>
#include <stdarg.h>

#include "terror.h"

TError::TError( const char *icat, const char *fmt, ... )
{
    char str[1024];                  //!!!!
    va_list argptr;

    va_start(argptr,fmt);
    vsnprintf(str,sizeof(str),fmt,argptr);
    va_end(argptr);

    cod  = 0;
    mess = str;
    cat  = icat;
}

TError::TError( int icod, const char *icat, const char *fmt, ... )
{
    char str[1024];                  //!!!!
    va_list argptr;

    va_start(argptr,fmt);
    vsnprintf(str,sizeof(str),fmt,argptr);
    va_end(argptr);

    cod  = icod;
    mess = str;
    cat  = icat;
}
