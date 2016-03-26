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
#include "core/ircconnection.h"
#include "core/ircidentity.h"
#include "settings/nsettings.h"

#define _NSCRIPT_ENGINE_INSTANCE NJSEngine::instance()

class NScriptAction;
class NScriptBindings;

class NJSEngine : public QObject
{
    Q_OBJECT
public:
    //!
    //! \brief The ActionType enum is used internally for some methods.
    //!
    enum ActionType {
        Unknown = 0,
        UserMenu,
        ToolsMenu
    };

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
    static void registerAction(ActionType type, const QString &text, QJSValue function);

    static QList<NScriptAction *> *actions() { return _actions; }

    static IrcConnection *connection() { return _connection; }
    static void setConnection(IrcConnection *connection) { _connection = connection; }
    static IrcIdentity *identity() { return _id; }
    static void setIdentity(IrcIdentity *id) { _id = id; }

signals:
    void actionsChanged();

private:
    static NJSEngine *_njsengine;
    static QJSEngine *_engine;
    static bool _initialized;
    static QList<NScript *> *_scripts;
    static QList<QJSValue> *_deinitFunctions;
    static QList<NScriptAction *> *_actions;
    static NScriptBindings *_bindings;

    static IrcConnection *_connection;
    static IrcIdentity *_id;

    Q_DISABLE_COPY(NJSEngine)
};

#endif // NJSENGINE_H
