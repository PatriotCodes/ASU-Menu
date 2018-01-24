#ifndef INIPARSER_H
#define INIPARSER_H

#include "filewriter.h"
#include "inisection.h"
#include "keyvalue.h"
#include <QList>
#include <QStringList>

class IniParser
{
public:
    IniParser();
    static QList<IniSection> parse(QString);
    static QString valueByKey(QList<IniSection>,QString);
    static QString createIniString(QList<IniSection> sections);

private:
    static QString removeTrailingLeadingSpaces(QString);
    static QString removeParenthesis(QString);
};

#endif // INIPARSER_H
