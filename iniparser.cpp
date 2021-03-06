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
            if (result.count() == 0) {
                IniSection section;
                section.sectionName = "";
                result.append(section);
            }
            QString args;
            if (keyValue[0] == "args") {
                keyValue[1].remove(" ");
                args = keyValue[1];
                result.back().itemList.last()->args.append(args);
            } else {
                result.back().itemList.append(new IniItem(keyValue[0],(removeParenthesis(keyValue[1])),args));
            }
        }
    }
    return result;
}

QString IniParser::removeTrailingLeadingSpaces(QString string) {
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

QString IniParser::removeParenthesis(QString string) {
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

QString IniParser::valueByKey(QList<IniSection> sections, QString key) {
    for (auto section:sections) {
        for (int i = 0; i < section.itemList.count(); i++) {
            if (section.itemList[i]->buttonName == key) {
                return section.itemList[i]->buttonAction;
            }
        }
    }
    return "";
}

QString IniParser::createIniString(QList<IniSection> sections) {
    QString result;
    for (auto section : sections) {
        result.append('[' + section.sectionName + ']' + '\n');
        for (IniItem *item : section.itemList) {
            result.append(item->buttonName + " = " + item->buttonAction + '\n');
            if (!item->args.isNull())
                result.append("args = " + item->args + '\n');
        }
    }
    return result;
}
