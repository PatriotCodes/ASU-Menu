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

private:
    inline static QString removeTrailingLeadingSpaces(QString);
    inline static QString removeParenthesis(QString);
};

#endif // INIPARSER_H
