#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::SettingsDialog)
{
    _ui->setupUi(this);
}

SettingsDialog::~SettingsDialog()
{
    delete _ui;
}

void SettingsDialog::on_qdbbButtons_clicked(QAbstractButton *button)
{
    auto buttons = _ui->qdbbButtons->buttons();
    QAbstractButton *b = 0;
    for (auto btn : buttons) {
        if (btn == button) {
            b = btn;
            break;
        }
    }

    // this is needed because the "Apply" button does not emit accepted() by default
    if (_ui->qdbbButtons->buttonRole(b) == QDialogButtonBox::ApplyRole) {
        emit accepted();
    }
}
