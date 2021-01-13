#ifndef TSYS_H
#define TSYS_H

#include <QString>

class TSYS //: public TCntrNode
{
public:
    enum Code	{ PathEl,Custom,FormatPrint};
    enum IntView	{ Dec, Oct, Hex };

    //> Path and string parse
    static QString strNoSpace( const QString &val );
    static QString strSepParse( const QString &str, int level, char sep, int *off = NULL );
    static QString strParse( const QString &str, int level, const QString &sep, int *off = NULL, bool mergeSepSymb = false );
    static QString strLine( const QString &str, int level, int *off = NULL );
    static QString pathLev( const QString &path, int level, bool encode = true, int *off = NULL );
    static QString path2sepstr( const QString &path, char sep = '.' );
    static QString sepstr2path( const QString &str, char sep = '.' );
    static QString strEncode( const QString &in, Code tp, const QString &symb = " \t\n");
    static QString strDecode( const QString &in, Code tp = Custom );

protected:

private:

};


#endif
