#include "filewriter.h"

FileWriter::FileWriter()
{}

void FileWriter::append(QFile* file, QString message)
{
    if(file->open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QTextStream outStream(file);
        outStream << message;
        file->close();
    }
}

void FileWriter::write(QFile* file, QString message)
{
    if(file->open(QIODevice::WriteOnly))
    {
        QTextStream outStream(file);
        outStream << message;
        file->close();
    }
}

QStringList FileWriter::read(QFile* file)
{
    QStringList result;
    if (file->open(QIODevice::ReadOnly))
    {
        QTextStream in(file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            result.append(line);
        }
        file->close();
    }
    return result;
}

bool FileWriter::exists(QString path)
{
    QFileInfo checkFile(path);
    return (checkFile.exists() && checkFile.isFile());
}
