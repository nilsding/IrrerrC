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
