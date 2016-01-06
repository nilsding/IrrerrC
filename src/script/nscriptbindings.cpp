#include "nscriptbindings.h"

NScriptBindings::NScriptBindings(QObject *parent) : QObject(parent)
{

}

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

void NScriptBindings::showAboutDialog()
{
    AboutDialog().exec();
}
