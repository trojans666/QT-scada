
#include "tsys.h"

TSYS::TSYS(int argi,char **argb,char **env)
{

}

TSYS::~TSYS()
{

}

/**
qDebug() <<  TSYS::strNoSpace(" jskl  fjskl ");
int off = 1;
qDebug() << TSYS::strSepParse("122.34.56.78",0,'.',&off);
qDebug() << QString(".").size();
qDebug() << TSYS::strParse(".122.34.56.78",1,".",NULL,true);
qDebug() <<TSYS::pathLev("/xx/sjfkld",0);
qDebug() << TSYS::path2sepstr("/xx/xx1/xx2/",'a');
qDebug() << TSYS::sepstr2path("xx/xx.xx1");
qDebug() << TSYS::strLine("sjk\r\njsfk",0);

*/
/**
 * @brief 删除开头和结尾的空格
 * @param val
 * @return
 */
QString TSYS::strNoSpace( const QString &val )
{
    int beg = -1, end = -1;

    for(int i_s = 0; i_s < val.size(); i_s++)
    if(val[i_s] != ' ' && val[i_s] != '\n' && val[i_s] != '\t')
    {
        if(beg < 0) beg = i_s;
        end = i_s;
    }

    return (beg>=0) ? val.mid(beg,end-beg+1) : "";
}

/**
 * @brief 对path进行分割，分隔符是sep,level控制是第几个0.1.2.3 off是偏移，表示从path的第几个字符开始解析
 * @param path
 * @param level
 * @param sep
 * @param off
 * @return
 *@eg
 * int off = 1;
   qDebug() << TSYS::strSepParse("122.34.56.78",0,'.',&off);
   ->22
   off=0
   ->123
 */
QString TSYS::strSepParse( const QString &path, int level, char sep, int *off)
{
    int an_dir = off ? *off : 0;
    int t_lev = 0;
    int t_dir;

    if( an_dir >= path.size() ) return "";
    while(true)
    {
        t_dir = path.indexOf(sep,an_dir);
        if( t_dir == -1 )
        {
            if( off ) *off = path.size();
            return (t_lev == level) ? path.mid(an_dir) : "";
        }
        else if( t_lev == level )
        {
            if( off ) *off = t_dir+1;
            return path.mid(an_dir,t_dir-an_dir);
        }
        an_dir = t_dir+1;
        t_lev++;
    }
    return "";
}

/**
 * @brief strSepParse的扩展版 sep可以是字符串分割了 merge允许合并
 * @param path
 * @param level
 * @param sep
 * @param off
 * @param mergeSepSymb
 * @return
 */
QString TSYS::strParse( const QString &path, int level, const QString &sep, int *off, bool mergeSepSymb)
{
    int an_dir = off ? *off : 0;
    int t_lev = 0;
    int t_dir;

    if( an_dir >= path.size() || sep.isEmpty() ) return "";
    while(true)
    {
        t_dir = path.indexOf(sep,an_dir);
        if( t_dir == -1 )
        {
            if( off ) *off = path.size();
            return (t_lev == level) ? path.mid(an_dir) : "";
        }
        else if( t_lev == level )
        {
            if( off ) *off = t_dir+sep.size();
            return path.mid(an_dir,t_dir-an_dir);
        }
        if( mergeSepSymb && sep.size() == 1 )
            for( an_dir = t_dir; an_dir < path.size() && path[an_dir] == sep[0]; ) an_dir++;
        else an_dir = t_dir+sep.size();
        t_lev++;
    }
    return "";
}

/**
 * @brief 获取一行 相当于对str中找\r\n
 * @param str
 * @param level
 * @param off
 * @return
 */
QString TSYS::strLine( const QString &str, int level, int *off)
{
    int an_dir = off ? *off : 0;
    int t_lev = 0, edLnSmbSz = 1;
    size_t t_dir;

    if(an_dir >= (int)str.size()) return "";
    while(true)
    {
        for(t_dir = an_dir; t_dir < str.size(); t_dir++)
            if(str[t_dir] == '\x0D' || str[t_dir] == '\x0A')
            { edLnSmbSz = (str[t_dir] == '\x0D' && ((t_dir+1) < str.size()) && str[t_dir+1] == '\x0A') ? 2 : 1; break; }
        if(t_dir >= str.size())
        {
            if(off) *off = str.size();
            return (t_lev==level) ? str.mid(an_dir) : "";
        }
        else if(t_lev == level)
        {
            if(off) *off = t_dir+edLnSmbSz;
            return str.mid(an_dir,t_dir-an_dir);
        }
        an_dir = t_dir+edLnSmbSz;
        t_lev++;
    }
    return "";
}

/**
 * @brief 对path路径分割 /path1/path2/path3 以'/'分割
 * @param path
 * @param level
 * @param encode
 * @param off
 * @return
 */
QString TSYS::pathLev( const QString &path, int level, bool encode, int *off)
{
    int an_dir = off ? *off : 0;
    int t_lev = 0;
    int t_dir;

    //- First separators pass -
    while( an_dir<path.size() && path[an_dir]=='/' ) an_dir++;
    if( an_dir >= path.size() ) return "";
    //- Path level process -
    while(true)
    {
        t_dir = path.indexOf(QLatin1String("/"),an_dir);
        if( t_dir == -1 )
        {
            if( off ) *off = path.size();
            return (t_lev == level) ? ( encode ? TSYS::strDecode(path.mid(an_dir),TSYS::PathEl) : path.mid(an_dir) ) : "";
        }
        else if( t_lev == level )
        {
            if( off ) *off = t_dir;
            return encode ? TSYS::strDecode(path.mid(an_dir,t_dir-an_dir),TSYS::PathEl) : path.mid(an_dir,t_dir-an_dir);
        }
        an_dir = t_dir;
        t_lev++;
        while( an_dir<path.size() && path[an_dir]=='/' ) an_dir++;
    }
}

/**
 * @brief 将path转换为sep分割的字符串 例如 /xx1/xx2/xx3 sep='.' -> xx1.xx2.xx3
 * @param path
 * @param sep
 * @return
 */
QString TSYS::path2sepstr( const QString &path, char sep)
{
    QString rez, curv;
    int off = 0;
    while( !(curv=TSYS::pathLev(path,0,false,&off)).isEmpty() )
        rez+=curv+sep;
    if(!rez.isEmpty())	rez.resize(rez.size()-1);

    return rez;
}

/**
 * @brief 将str转换成path格式
 * @param str
 * @param sep
 * @return
 */
QString TSYS::sepstr2path( const QString &str, char sep)
{
    QString rez, curv;
    int off = 0;
    while( !(curv=TSYS::strSepParse(str,0,sep,&off)).isEmpty() )
        rez+="/"+curv;

    return rez;
}

QString TSYS::strEncode( const QString &in, Code tp, const QString &symb)
{
    int i_sz;
    QString sout;

    switch(tp)
    {
    case TSYS::PathEl:
        sout = in;
        for( i_sz = 0; i_sz < sout.size(); i_sz++ )
        switch(int(sout.at(i_sz).toAscii())) //int(sout[i_sz]) )
        {
            case '/': sout.replace(i_sz,1,"%2f"); i_sz+=2; break;
            case '%': sout.replace(i_sz,1,"%25"); i_sz+=2; break;
        }
        break;
    case TSYS::Custom:
        sout.reserve(in.size()+10);
        for( i_sz = 0; i_sz < in.size(); i_sz++ )
        {
        int i_smb;
        for( i_smb = 0; i_smb < symb.size(); i_smb++ )
            if( in[i_sz] == symb[i_smb] )
            {
            char buf[4];
            //sprintf(buf,"%%%02X",(unsigned char)in[i_sz]);
            sprintf(buf,"%%%02X",(unsigned char)in.at(i_sz).toLatin1());
            sout += buf;
            break;
            }
        if( i_smb >= symb.size() ) sout+=in[i_sz];
        }
        break;

    case TSYS::FormatPrint:
        sout = in;
        for( i_sz = 0; i_sz < sout.size(); i_sz++ )
        if( sout[i_sz] == '%' ) { sout.replace(i_sz,1,"%%"); i_sz++; }
        break;
    }
    return sout;
}

QString TSYS::strDecode( const QString &in, Code tp)
{
    int i_sz;
    QString sout;

    switch(tp)
    {
    case TSYS::PathEl:
    case TSYS::Custom:
        sout.reserve(in.size());
        for( i_sz = 0; i_sz < in.size(); i_sz++ )
            switch(int(in.at(i_sz).toAscii())) //int(in[i_sz]) )
            {
            case '%':
                if( i_sz+2 < in.size() )
                {
                    sout+=(char)strtol(in.mid(i_sz+1,2).toUtf8().data(),NULL,16);
                    i_sz+=2;
                }else sout+=in[i_sz];
                break;
            default: sout+=in[i_sz];
            }
        break;
    }

    return sout;
}
