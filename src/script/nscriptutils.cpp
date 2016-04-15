#include "nscriptutils.h"

//!
//! \brief NScriptUtils::handleErrors checks whether a value is a JavaScript error object.
//! \param value a JavaScript value
//! \param showMessageBox `true`, if a message box should be shown, `false` if the exception should
//!     be printed to the console instead
//! \return `true` if the value is a JavaScript error object, `false` otherwise
//!
bool NScriptUtils::handleErrors(QJSValue *value, bool showMessageBox)
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
