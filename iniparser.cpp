#include "iniparser.h"

IniParser::IniParser()
{}

QList<IniSection> IniParser::parse(QString fileName) {
    QStringList iniLines = FileWriter::read(fileName);
    QList<IniSection> result;
    for (int i = 0; i < iniLines.count(); i++) {
        QString line = iniLines[i];
        line = removeTrailingLeadingSpaces(line);
        if (line[0] == ';') {
            continue;
        }
        if (line[0] == '[') {
            IniSection section;
            section.sectionName = removeParenthesis(line);
            result.append(section);
            continue;
        }
        if (line.contains('=')) {
            QStringList keyValue = line.split('=');
            keyValue[1] = removeTrailingLeadingSpaces(keyValue[1]);
            keyValue[0] = removeTrailingLeadingSpaces(keyValue[0]);
            result.back().itemList.append(new KeyValue(keyValue[0],removeParenthesis(keyValue[1]))); // TODO: needs Destructor
        }
    }
    return result;
}

inline QString IniParser::removeTrailingLeadingSpaces(QString string) {
    for (int i = 0; i < string.count(); i++) {
        if (string[i] == ' ') {
            string.remove(i,1);
        } else {
            break;
        }
    }
    for (int i = string.count() - 1; i >= 0; i--) {
        if (string[i] == ' ') {
            string.remove(i,1);
        } else {
            break;
        }
    }
    return string;
}

inline QString IniParser::removeParenthesis(QString string) {
    for (int i = 0; i < string.count(); i++) {
        if (string[i] == '"' || string[i] == '[') {
            string.remove(i,1);
        } else {
            break;
        }
    }
    for (int i = string.count() - 1; i >= 0; i--) {
        if (string[i] == '"' || string[i] == ']') {
            string.remove(i,1);
        } else {
            break;
        }
    }
    return string;
}
