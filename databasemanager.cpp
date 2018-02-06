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

// returns -1 if no worker with such username is found
int DatabaseManager::userIdByName(QString username) {
    QSqlQuery query("SELECT workerID FROM worker WHERE username = '" + username + "';");
    int id = -1;
    if (query.next()) {
            QSqlRecord record = query.record();
            id = (query.value(record.indexOf("workerID")).toString()).toInt();
    }
    return id;
}

QList<int> DatabaseManager::userActions(int userID) {
    QList<int> results;
    QSqlQuery query("select * from appointment where workerID = " + QString::number(userID) + ";");
    while (query.next()) {
        QSqlRecord record = query.record();
        QDate dateFrom = query.value(record.indexOf("fromDate")).toDate();
        QDate dateTo = query.value(record.indexOf("toDate")).toDate();
        if (assertDate(dateFrom,dateTo)) {
            results.append((query.value(record.indexOf("actionID")).toString()).toInt());
        }
    }
    return results;
}

bool DatabaseManager::assertDate(QDate dateFrom, QDate dateTo) {
    QDate currentDate = QDate::currentDate();
    if (dateFrom > currentDate)
        return false;
    if (dateFrom <= currentDate) {
        if (dateTo.isNull()) {
            return true;
        } else {
            if (dateTo > currentDate)
                return true;
        }
    }
    return false;
}

QList<IniSection> DatabaseManager::initialiseData(QList<int> actions) {
    QList<IniSection> result;
    for (int action : actions) {
        QSqlQuery query("select * from workAction where actionID = " + QString::number(action) + ";");
        while (query.next()) {
            QSqlRecord record = query.record();
            int catID = query.value(record.indexOf("categoryID")).toInt();
            QSqlQuery catNameQuery("select categoryName from actionCategory where categoryID = " + QString::number(catID) + ";");
            while (catNameQuery.next()) {
                int catIndex = indexByVal(result,catNameQuery.value(0).toString());
                if (catIndex == 0 || catIndex == result.count()) {
                    IniSection section;
                    section.sectionName = catNameQuery.value(0).toString();
                    result.append(section);
                }
                QString buttonName = query.value(record.indexOf("actionName")).toString();
                QString buttonAction = query.value(record.indexOf("actionString")).toString();
                QString args = query.value(record.indexOf("actionArguments")).toString();
                result[catIndex].itemList.append(new IniItem(buttonName,buttonAction,args));
            }
        }
    }
    return result;
}

int DatabaseManager::indexByVal(QList<IniSection> list, QString val) {
    int index = 0;
    for (IniSection item : list) {
        if (item.sectionName == val)
            return index;
        index++;
    }
    return index;
}

QString DatabaseManager::getError() {
    return db.lastError().text();
}
