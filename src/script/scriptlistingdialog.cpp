#include "scriptlistingdialog.h"
#include "ui_scriptlistingdialog.h"

ScriptListingDialog::ScriptListingDialog(QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::ScriptListingDialog)
{
    _ui->setupUi(this);
    _ui->qdbbButtons->addButton(tr("&Edit"), QDialogButtonBox::ActionRole);
}

ScriptListingDialog::~ScriptListingDialog()
{
    delete _ui;
}
