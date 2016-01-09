#ifndef NJSENGINE_H
#define NJSENGINE_H

#include <QObject>
#include <QJSEngine>
#include <QDir>
#include <QFileInfo>
#include <QTimer>
#include <QJSValue>
#include "nscript.h"
#include "nscriptbindings.h"
#include "settings/nsettings.h"

#define _NSCRIPT_ENGINE_INSTANCE NJSEngine::instance()

class NScriptBindings;

class NJSEngine : public QObject
{
    Q_OBJECT
public:
    explicit NJSEngine(QObject *parent = 0);

    static void init();

    static NJSEngine* instance()
    {
        if (!_initialized) {
            return 0;
        }
        return _njsengine;
    }

    static void loadScripts();
    static void reloadScripts();
    static void unloadScripts();

    static void registerDeinitFunction(QJSValue fn);

private:
    static NJSEngine *_njsengine;
    static QJSEngine *_engine;
    static bool _initialized;
    static QList<NScript *> *_scripts;
    static QList<QJSValue> *_deinitFunctions;
    static NScriptBindings *_bindings;

    Q_DISABLE_COPY(NJSEngine)
};

#endif // NJSENGINE_H
