#include "njsengine.h"
#include <QDebug>

NJSEngine *NJSEngine::_njsengine = 0;
QJSEngine *NJSEngine::_engine = new QJSEngine();
bool NJSEngine::_initialized = false;
QList<NScript *> *NJSEngine::_scripts = new QList<NScript *>();
QList<QJSValue *> *NJSEngine::_deinitFunctions = new QList<QJSValue *>();
NScriptBindings *NJSEngine::_bindings = new NScriptBindings();

NJSEngine::NJSEngine(QObject *parent) : QObject(parent)
{

}

void NJSEngine::init()
{
    if (_initialized) {
        return;
    }
    _njsengine = new NJSEngine();
    qDebug() << "Initialized NJSEngine";

    QJSValue scriptObj = _engine->newQObject(_bindings);
    _engine->globalObject().setProperty("IrrerrC", scriptObj);

    _initialized = true;
}

void NJSEngine::loadScripts()
{
    QString scriptDir = QFileInfo(_SETTINGS.fileName()).absolutePath() + "/scripts";
    QDir d(scriptDir);
    if (!d.exists()) {
        d.mkpath(scriptDir);
    }
    QStringList filters;
    filters << "*.js";
    QStringList entries = d.entryList(filters, QDir::NoDotAndDotDot | QDir::Files);

    for (const QString& entry : entries) {
        NScript *script = new NScript(scriptDir + "/" + entry);
        _scripts->append(script);
        script->load(_engine);
    }
}

void NJSEngine::reloadScripts()
{
    QTimer::singleShot(0, _njsengine, [&]() {
        qDebug() << "Reloading scripts...";
        _njsengine->unloadScripts();

        _engine->collectGarbage();
        _engine->deleteLater();
        _initialized = false;
        _engine = new QJSEngine;
        NJSEngine::init();

        _njsengine->loadScripts();
    });
}

void NJSEngine::unloadScripts()
{
    for (QJSValue *deinitFn : *_deinitFunctions) {
        QJSValue retval = deinitFn->call();
        _NSCRIPT_IF_ERRORS_PRINT_MESSAGE(retval, true);
    }
    _deinitFunctions->clear();

    for (NScript *script : *_scripts) {
        script->deleteLater();
    }
    _scripts->clear();
}

void NJSEngine::registerDeinitFunction(QJSValue fn)
{
    qDebug() << "Registered deinit function:" << fn.toString();
    _deinitFunctions->append(&fn);
}
