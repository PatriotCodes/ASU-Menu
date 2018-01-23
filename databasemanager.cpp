#include "databasemanager.h"

DatabaseManager::DatabaseManager()
{}

DatabaseManager::~DatabaseManager() {
    db.close();
}

bool DatabaseManager::instantiateConnection(QString dsnName, QString hostname, int port, QString username, QString password) {
    db = QSqlDatabase::addDatabase("QODBC");
    db.setHostName(hostname);
    db.setDatabaseName(dsnName);
    db.setPort(port);
    db.setUserName(username);
    db.setPassword(password);
    return db.open();
}

QStringList DatabaseManager::getDataFromTable(QString tableName, QString data) {
    QSqlQuery query("SELECT * FROM " + tableName);
    QStringList result;
    int fieldNo = query.record().indexOf(data);
    while (query.next()) {
            result.append(query.value(fieldNo).toString());
        }
    return result;
}
