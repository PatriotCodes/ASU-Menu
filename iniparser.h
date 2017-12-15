#ifndef INIPARSER_H
#define INIPARSER_H

#include "filewriter.h"
#include "inisection.h"
#include <QList>
#include <QStringList>

class IniParser
{
public:
    IniParser();
    static QList<IniSection> parse(QString);
};

#endif // INIPARSER_H
