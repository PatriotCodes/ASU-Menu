#ifndef INISECTION_H
#define INISECTION_H

#include "iniitem.h"
#include <QString>
#include <QVector>


struct IniSection {
    QString sectionName;
    QVector<IniItem*> itemList;
};

#endif // INISECTION_H
