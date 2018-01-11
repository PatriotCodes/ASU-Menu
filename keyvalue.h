#ifndef KEYVALUE_H
#define KEYVALUE_H

#include <QObject>

template <typename TKey, typename TValue>
struct KeyValue {
    TKey key;
    TValue value;

    KeyValue(TKey key, TValue value) {
        this->key = key;
        this->value = value;
    }
};

#endif // KEYVALUE_H
