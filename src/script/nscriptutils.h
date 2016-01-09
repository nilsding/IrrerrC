#ifndef NSCRIPTUTILS_H
#define NSCRIPTUTILS_H

#include <QObject>
#include <QJSValue>
#include <QDebug>
#include <QMessageBox>

#define _NSCRIPT_IF_ERRORS(value, mbox) if (NScriptUtils::handleErrors(&value, mbox))
#define _NSCRIPT_IF_ERRORS_PRINT_MESSAGE(value, mbox) NScriptUtils::handleErrors(&value, mbox)
#define _NSCRIPT_IF_ERRORS_RETURN(value, mbox) if (NScriptUtils::handleErrors(&value, mbox)) { return; }
#define _NSCRIPT_IF_ERRORS_RETURN_VAL(value, mbox, val) if (NScriptUtils::handleErrors(&value, mbox)) { return val; }

class NScriptUtils : public QObject
{
    Q_OBJECT

public:
    static bool handleErrors(QJSValue *, bool showMessageBox);

signals:

public slots:

private:
};

#endif // NSCRIPTUTILS_H
