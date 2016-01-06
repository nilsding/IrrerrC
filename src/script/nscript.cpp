#include "nscript.h"
#include <QDebug>

#define HANDLE_ERRORS(value, mbox) if (handleErrors(&value, mbox)) { return; }

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
    QJSValue initFn = engine->evaluate(_scriptContents, _filePath);
    HANDLE_ERRORS(initFn, true);
    initFn = engine->evaluate("init");
    initFn.call();
    HANDLE_ERRORS(initFn, true);
}

void NScript::unload(QJSEngine *engine)
{
    qDebug() << "Unloading script" << _scriptName;
    QJSValue deinitFn = engine->evaluate(_scriptContents, _filePath);
    HANDLE_ERRORS(deinitFn, true);
    deinitFn = engine->evaluate("deinit");
    deinitFn.call();
    HANDLE_ERRORS(deinitFn, true);
}

bool NScript::handleErrors(QJSValue *value, bool showMessageBox)
{
    if (value->isError()) {
        if (showMessageBox) {
            QMessageBox::critical(0, tr("Scripting error"),
                                  tr("Uncaught exception in file %1 at line %2:<p>%3")
                                  .arg(value->property("fileName").toString())
                                  .arg(value->property("lineNumber").toInt())
                                  .arg(value->toString()));
        } else {
            qDebug() << "Uncaught exception in file" << value->property("fileName").toString() << "at line"
                     << value->property("lineNumber").toInt() << ":" << value->toString();
        }
    }
    return value->isError();
}
