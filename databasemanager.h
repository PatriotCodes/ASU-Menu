#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "inisection.h"
#include <QList>
#include <QString>
#include <QSqlDatabase>
#include <QDate>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

class DatabaseManager
{
public:
    DatabaseManager();
    ~DatabaseManager();
    bool instantiateConnection(QString dsnName, QString hostname, int port, QString username, QString password);
    int userIdByName(QString username);
    QList<int> userActions(int userID);
    QList<IniSection> initialiseData(QList<int> actions);
private:
    QSqlDatabase db;
    bool assertDate(QDate dateFrom, QDate dateTo);
    int indexByVal(QList<IniSection> list, QString val);
};

#endif // DATABASEMANAGER_H
