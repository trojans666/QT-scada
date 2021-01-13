#ifndef TERROR_H
#define TERROR_H

#include <QString>

class TError
{
    public:
    //Methods
    TError( const char *icat, const char *fmt, ... );
    TError( int icod, const char *icat, const char *fmt, ... );

    //Attributes
    int	cod;
    QString	cat;
    QString	mess;
};

#endif
