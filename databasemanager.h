#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QSqlDatabase>
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
    QStringList getDataFromTable(QString tableName, QString data);
private:
    QSqlDatabase db;
};

#endif // DATABASEMANAGER_H
