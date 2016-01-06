#include "scriptlistingdialog.h"
#include "ui_scriptlistingdialog.h"

ScriptListingDialog::ScriptListingDialog(QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::ScriptListingDialog),
    _scriptDirModel(new QFileSystemModel), _currentScript(0)
{
    _scriptDir = QFileInfo(_SETTINGS.fileName()).absolutePath() + "/scripts";
    QDir d(_scriptDir);
    if (!d.exists()) {
        d.mkpath(_scriptDir);
    }

    connect(_scriptDirModel, &QFileSystemModel::directoryLoaded, this, [=](const QString &path) {
        _ui->qlvScriptList->setRootIndex(_scriptDirModel->index(path));
    });

    _ui->setupUi(this);
    _ui->qdbbButtons->addButton(tr("&Edit"), QDialogButtonBox::ActionRole);

    _scriptDirModel->setRootPath(_scriptDir);
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
        QUrl url;
        url.setPath(_scriptDirModel->filePath(_ui->qlvScriptList->selectionModel()->selectedIndexes().first()));
        url.setScheme(QLatin1String("file"));
        QDesktopServices::openUrl(url);
    }
}

void ScriptListingDialog::on_qpbOpenScriptDirectory_clicked()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(_scriptDir));
}

void ScriptListingDialog::on_qpbGetMoreScripts_clicked()
{
    QDesktopServices::openUrl(QUrl("http://irc.rrerr.net/client/scripts"));
}

void ScriptListingDialog::on_qlvScriptList_clicked(const QModelIndex &index)
{
    if (_currentScript) {
        _currentScript->deleteLater();
    }
    _currentScript = new NScript(_scriptDirModel->filePath(index), this);
    _ui->qlScriptName->setText(_currentScript->scriptName().toHtmlEscaped());
    _ui->qlAuthor->setText(tr("Author: <b>%1</b>").arg(_currentScript->author().toHtmlEscaped()));
    _ui->qlDescription->setText(_currentScript->description());
}
