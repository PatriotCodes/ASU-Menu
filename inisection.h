#ifndef INISECTION_H
#define INISECTION_H

#include <QString>
#include <QMap>


struct IniSection {
    QString sectionName;
    QMap<QString, QString> itemList;
};

#endif // INISECTION_H
