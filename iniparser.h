#ifndef INIPARSER_H
#define INIPARSER_H

#include "inisection.h"
#include <QList>

class IniParser
{
public:
    IniParser();
    static QList<IniSection> parse(QString);
};

#endif // INIPARSER_H
