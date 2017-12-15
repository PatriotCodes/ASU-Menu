#include "filewriter.h"

FileWriter::FileWriter()
{}

void FileWriter::append(QString filePath, QString message)
{
    QFile file(filePath);
    if(file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QTextStream outStream(&file);
        outStream << message;
        file.close();
    }
}

void FileWriter::write(QString filePath, QString message)
{
    QFile file(filePath);
    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream outStream(&file);
        outStream << message;
        file.close();
    }
}

QStringList FileWriter::read(QString filePath)
{
    QFile file(filePath);
    QStringList result;
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            result.append(line);
        }
        file.close();
    }
    return result;
}

bool FileWriter::exists(QString filePath)
{
    QFileInfo checkFile(filePath);
    return (checkFile.exists() && checkFile.isFile());
}
