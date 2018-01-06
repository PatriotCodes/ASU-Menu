#ifndef INISECTION_H
#define INISECTION_H

#include "keyvalue.h"
#include <QString>
#include <QVector>


struct IniSection {
    QString sectionName;
    QVector<KeyValue*> itemList;
    QStringList args;
};

#endif // INISECTION_H
