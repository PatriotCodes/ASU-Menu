#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <QFile>
#include <QFileInfo>
#include <QStringList>
#include <QTextStream>


class FileWriter
{
public:
    FileWriter();
    static void write(QString, QString);
    static void append(QString, QString);
    static QStringList read(QString);
    static bool exists(QString);
};

#endif // FILEWRITER_H
