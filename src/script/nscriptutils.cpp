#include "nscriptutils.h"

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
