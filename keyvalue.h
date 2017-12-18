#ifndef KEYVALUE_H
#define KEYVALUE_H

#include <QString>

struct KeyValue {
    QString key;
    QString value;

    KeyValue(QString key, QString value) {
        this->key = key;
        this->value = value;
    }
};

#endif // KEYVALUE_H
