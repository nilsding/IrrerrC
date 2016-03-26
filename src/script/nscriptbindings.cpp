#include "nscriptbindings.h"

NScriptBindings::NScriptBindings(QObject *parent) : QObject(parent)
{

}

//!
//! \brief NScriptBindings::init calls the init function for this script.  It is called
//!     upon a clean (re)load of this script.
//! \param function a JavaScript function
//! \return true, if `function` is a callable function and the call was successful; false otherwise
//!
bool NScriptBindings::init(QJSValue function)
{
    if (!function.isCallable()) {
        return false;
    }

    QJSValue retval = function.call();
    _NSCRIPT_IF_ERRORS_RETURN_VAL(retval, true, false);
    return true;
}

//!
//! \brief NScriptBindings::deinit registers the deinit function which will be called
//!     upon a clean reload/unload of this script
//! \param function a JavaScript function
//! \return true, if `function` is a callable function; false otherwise
//!
bool NScriptBindings::deinit(QJSValue function)
{
    if (!function.isCallable()) {
        return false;
    }

    _NSCRIPT_ENGINE_INSTANCE->registerDeinitFunction(function);

    return true;
}

//!
//! \brief NScriptBindings::alert shows a message box.
//! \param message
//! \param type One of: "error", "information", "question".
//! \return for "error" and "information" types: -1; for "question" type: return value of QMessageBox::Question
//!
int NScriptBindings::alert(const QString &message, const QString &type)
{
    if (type == "error") {
        QMessageBox::critical(0, "Scripting", message);
    } else if (type == "information") {
        QMessageBox::information(0, "Scripting", message);
    } else if (type == "question") {
        return QMessageBox::question(0, "Scripting", message);
    }
    return -1;
}

//!
//! \brief NScriptBindings::showAboutDialog shows the about dialog.
//!
void NScriptBindings::showAboutDialog()
{
    AboutDialog().exec();
}

//!
//! \brief NScriptBindings::addUserMenuAction adds a new action to the user menu (i.e. the context menu in the userlist)
//! \param text The text to display
//! \param function The function to execute
//!
void NScriptBindings::addUserMenuAction(const QString &text, QJSValue function)
{
    _NSCRIPT_ENGINE_INSTANCE->registerAction(NJSEngine::ActionType::UserMenu, text, function);
}

//!
//! \brief NScriptBindings::addUserMenuAction adds a new action to the Tools menu (in the menu bar)
//! \param text The text to display
//! \param function The function to execute
//!
void NScriptBindings::addToolsMenuAction(const QString &text, QJSValue function)
{
    _NSCRIPT_ENGINE_INSTANCE->registerAction(NJSEngine::ActionType::ToolsMenu, text, function);
}

//!
//! \brief NScriptBindings::privmsg sends a `PRIVMSG` with the text `text` to `target`.
//! \param target
//! \param text
//!
void NScriptBindings::privmsg(const QString &target, const QString &text)
{
    auto conn = _NSCRIPT_ENGINE_INSTANCE->connection();
    if (!conn->isConnected()) {
        return;
    }

    auto identity = _NSCRIPT_ENGINE_INSTANCE->identity();

    QString input = QString("PRIVMSG %1 :%2").arg(target).arg(text);

    // TODO: echo back our own message
    /*IrcMessage *m = 0;
    if ((m = (new IrcParser)->parseLine(input))) {
        if (identity) {
            m->setPrefix(identity->nickname());
        }
        receiveMessage(m);
    }*/

    conn->raw(input);
}

//!
//! \brief NScriptBindings::action sends a `/me` action with the text `text` to `target`.
//! \param target
//! \param text
//!
void NScriptBindings::action(const QString &target, const QString &text)
{
    privmsg(target, QString("\1ACTION %1\1").arg(text));
}
