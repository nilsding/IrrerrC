#include "scriptlistingdialog.h"
#include "ui_scriptlistingdialog.h"

ScriptListingDialog::ScriptListingDialog(QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::ScriptListingDialog),
    _scriptDirModel(new QFileSystemModel)
{
    connect(_scriptDirModel, &QFileSystemModel::directoryLoaded, this, [=](const QString &path) {
        _ui->qlvScriptList->setRootIndex(_scriptDirModel->index(path));
    });

    _ui->setupUi(this);
    _ui->qdbbButtons->addButton(tr("&Edit"), QDialogButtonBox::ActionRole);

    _scriptDirModel->setRootPath(QFileInfo(_SETTINGS.fileName()).absolutePath() + "/scripts");
    _scriptDirModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    QStringList filters;
    filters << "*.js";
    _scriptDirModel->setNameFilters(filters);
    _scriptDirModel->setNameFilterDisables(false);
    _ui->qlvScriptList->setModel(_scriptDirModel);
}

ScriptListingDialog::~ScriptListingDialog()
{
    delete _ui;
}

void ScriptListingDialog::on_qdbbButtons_clicked(QAbstractButton *button)
{
    auto buttons = _ui->qdbbButtons->buttons();
    QAbstractButton *b = 0;
    for (auto btn : buttons) {
        if (btn == button) {
            b = btn;
            break;
        }
    }

    if (_ui->qdbbButtons->buttonRole(b) == QDialogButtonBox::ActionRole) {
        // TODO: open default editor for the current script
    }
}

void ScriptListingDialog::on_qpbOpenScriptDirectory_clicked()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(_SETTINGS.fileName()).absolutePath() + "/scripts"));
}

void ScriptListingDialog::on_qpbGetMoreScripts_clicked()
{
    QDesktopServices::openUrl(QUrl("http://irc.rrerr.net/client/scripts"));
}
