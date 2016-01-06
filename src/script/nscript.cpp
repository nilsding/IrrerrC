#include "nscript.h"
#include <QDebug>

NScript::NScript(QString filePath, QObject *parent) : QObject(parent), _filePath(filePath)
{
    readScriptInfo();
    qDebug() << "Read script data:" << _filePath << "-- Script name: " << _scriptName << ", Author:" << _author << ", Description:" << _description;
}

void NScript::readScriptInfo()
{
    QFile file(_filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QString *currentTag = 0;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.startsWith("//! ")) {
            QRegExp re("[\\\\@](\\S+)");
            int offset = 4;
            if (re.indexIn(line, offset) != -1) {
                QString match = re.cap(1).toLower();
                if (match == "scriptname") {
                    currentTag = &_scriptName;
                    _scriptName = "";
                } else if (match == "author") {
                    currentTag = &_author;
                    _author = "";
                } else if (match == "description") {
                    currentTag = &_description;
                    _description = "";
                }
                offset += re.matchedLength() + 1;
            }
            if (currentTag) {
                currentTag->append(line.mid(offset).trimmed().append(" "));
            }
        } else {
            currentTag = 0;
        }
    }

    if (_scriptName.trimmed().isEmpty()) {
        QFileInfo fi(_filePath);
        _scriptName = fi.completeBaseName() + '.' + fi.completeSuffix();
    }
    if (_author.trimmed().isEmpty()) {
        _author = tr("Unknown");
    }
    if (_description.trimmed().isEmpty()) {
        _description = tr("This script does not have a description.");
    }
}
