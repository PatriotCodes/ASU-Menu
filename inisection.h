#ifndef INISECTION_H
#define INISECTION_H

#include <QString>
#include <QStringList>


struct IniSection {
    QString sectionName;
    QStringList itemList;
};

#endif // INISECTION_H
