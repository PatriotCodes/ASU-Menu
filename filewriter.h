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
    static void write(QFile* file, QString message);
    static void append(QFile* file, QString message);
    static QStringList read(QFile* file);
    static bool exists(QString path);
};

#endif // FILEWRITER_H
