#ifndef INIITEM_H
#define INIITEM_H

#include <QString>
#include <QStringList>

struct IniItem {
    QString buttonName;
    QString buttonAction;
    QStringList args;

    IniItem(QString buttonName, QString buttonAction, QStringList args) {
        this->buttonName = buttonName;
        this->buttonAction = buttonAction;
        this->args = args;
    }
};

#endif // INIITEM_H
