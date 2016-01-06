#ifndef NJSENGINE_H
#define NJSENGINE_H

#include <QObject>
#include <QJSEngine>
#include <QDir>
#include <QFileInfo>
#include <QTimer>
#include "nscript.h"
#include "nscriptbindings.h"
#include "settings/nsettings.h"

#define _JSENGINE NJSEngine::instance()

class NJSEngine : public QObject
{
    Q_OBJECT
public:
    explicit NJSEngine(QObject *parent);

    static void init();

    static QJSEngine& instance()
    {
        return *_engine;
    }

signals:

public slots:
    void loadScripts();
    void reloadScripts();
    void unloadScripts();

private:
    static QJSEngine *_engine;
    static bool _initialized;
    static QList<NScript *> *_scripts;
    static NScriptBindings *_bindings;

    Q_DISABLE_COPY(NJSEngine)
};

#endif // NJSENGINE_H
