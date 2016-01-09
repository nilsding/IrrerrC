#ifndef NSCRIPT_H
#define NSCRIPT_H

#include <QObject>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QRegexp>
#include <QJSEngine>
#include <QJSValue>
#include <QMessageBox>
#include "nscriptutils.h"

class NScript : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString filePath READ filePath)
    Q_PROPERTY(QString scriptName READ scriptName WRITE setScriptName)
    Q_PROPERTY(QString author READ author WRITE setAuthor)
    Q_PROPERTY(QString description READ description WRITE setDescription)

public:
    explicit NScript(QString filePath, QObject *parent = 0);

    QString filePath() const { return _filePath; }
    QString scriptName() const { return _scriptName; }
    void setScriptName(QString scriptName) { _scriptName = scriptName; }
    QString author() const { return _author; }
    void setAuthor(QString author) { _author = author; }
    QString description() const { return _description; }
    void setDescription(QString description) { _description = description; }

signals:

public slots:
    void load(QJSEngine *engine);

private:
    QString _filePath;
    QString _scriptName;
    QString _author;
    QString _description;

    QString _scriptContents;

    void parseScriptInfo();
};

#endif // NSCRIPT_H
