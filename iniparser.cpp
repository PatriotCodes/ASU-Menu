#include "iniparser.h"

IniParser::IniParser()
{}

QList<IniSection> IniParser::parse(QString fileName) { // TODO: add checks, key value strings format
    QStringList iniLines = FileWriter::read(fileName);
    QList<IniSection> result;
    for (int i = 0; i < iniLines.count(); i++) {
        QString line = iniLines[i];
        if (line[0] == ';') {
            continue;
        }
        if (line[0] == '[') {
            IniSection section;
            section.sectionName = line;
            result.append(section);
            continue;
        }
        if (line.contains('=')) {
            QStringList keyValue = line.split('=');
            result.back().itemList.append(new KeyValue(keyValue[0],keyValue[1])); // TODO: needs Destructor
        }
    }
    return result;
}
