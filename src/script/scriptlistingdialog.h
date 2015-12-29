#ifndef SCRIPTLISTING_H
#define SCRIPTLISTING_H

#include <QDialog>
#include <QAbstractButton>
#include <QDesktopServices>
#include <QFileInfo>
#include <QDir>
#include <QUrl>
#include <QFileSystemModel>
#include "settings/nsettings.h"

namespace Ui {
class ScriptListingDialog;
}

class ScriptListingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScriptListingDialog(QWidget *parent = 0);
    ~ScriptListingDialog();

private slots:
    void on_qdbbButtons_clicked(QAbstractButton *button);

    void on_qpbOpenScriptDirectory_clicked();

    void on_qpbGetMoreScripts_clicked();

private:
    Ui::ScriptListingDialog *_ui;

    QFileSystemModel *_scriptDirModel;
};

#endif // SCRIPTLISTING_H
