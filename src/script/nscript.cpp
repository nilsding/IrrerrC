#include "nscript.h"
#include <QDebug>

NScript::NScript(QString filePath, QObject *parent) : QObject(parent), _filePath(filePath)
{
    QFile file(_filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }
    QTextStream in(&file);
    _scriptContents = in.readAll();
    file.close();

    parseScriptInfo();
    qDebug() << "Read script data:" << _filePath << "-- Script name: " << _scriptName << ", Author:" << _author << ", Description:" << _description;
}

void NScript::parseScriptInfo()
{
    QTextStream in(&_scriptContents);
    QString *currentTag = 0;
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

void NScript::load(QJSEngine *engine)
{
    qDebug() << "Loading script" << _scriptName;
    QJSValue retval = engine->evaluate(_scriptContents, _filePath);
    _NSCRIPT_IF_ERRORS_RETURN(retval, true);
}
