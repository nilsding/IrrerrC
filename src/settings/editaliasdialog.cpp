#include "editaliasdialog.h"
#include "ui_editaliasdialog.h"

EditAliasDialog::EditAliasDialog(QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::EditAliasDialog)
{
    _ui->setupUi(this);
    _ui->qrbSimple->setChecked(true);
#ifdef SCRIPTING_ENABLED
    _ui->qrbScript->setEnabled(true);
#else
    _ui->qrbScript->setEnabled(false);
#endif
}

EditAliasDialog::~EditAliasDialog()
{
    delete _ui;
}

IrcAlias::IrcAliasType EditAliasDialog::type() const
{
    if (_ui->qrbSimple->isChecked()) {
        return IrcAlias::AliasSimple;
    }
    if (_ui->qrbScript->isChecked()) {
        return IrcAlias::AliasScript;
    }
    return IrcAlias::AliasSimple;
}

QString EditAliasDialog::alias() const
{
    return _ui->qleAlias->text();
}

QString EditAliasDialog::action() const
{
    return _ui->qleAction->text();
}

void EditAliasDialog::setType(IrcAlias::IrcAliasType type)
{
    switch (type) {
    case IrcAlias::AliasScript:
        _ui->qrbScript->setChecked(true);
        break;
    default:
        _ui->qrbSimple->setChecked(true);
    }
}

void EditAliasDialog::setAlias(QString alias)
{
    _ui->qleAlias->setText(alias);
}

void EditAliasDialog::setAction(QString action)
{
    _ui->qleAction->setText(action);
}
