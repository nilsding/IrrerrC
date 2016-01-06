#include "njsengine.h"
#include <QDebug>


QJSEngine *NJSEngine::_engine = new QJSEngine();
bool NJSEngine::_initialized = false;
QList<NScript *> *NJSEngine::_scripts = new QList<NScript *>();
NScriptBindings *NJSEngine::_bindings = new NScriptBindings();

NJSEngine::NJSEngine(QObject *parent) : QObject(parent)
{

}

void NJSEngine::init()
{
    if (_initialized) {
        return;
    }
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
    QTimer::singleShot(0, this, [&]() {
        qDebug() << "Reloading scripts...";
        unloadScripts();
        _scripts->clear();
        _engine->collectGarbage();
        _engine->deleteLater();
        _initialized = false;
        _engine = new QJSEngine;
        NJSEngine::init();
        loadScripts();
    });
}

void NJSEngine::unloadScripts()
{
    for (NScript *script : *_scripts) {
        script->unload(_engine);
        script->deleteLater();
    }
}
