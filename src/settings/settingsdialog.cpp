#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include "identitysettingswidget.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::SettingsDialog)
{
    _ui->setupUi(this);

    QList<QWidget *> settingsWidgets;
    settingsWidgets.append(new IdentitySettingsWidget);

    for (auto w : settingsWidgets) {
        connect(this, SIGNAL(accepted()), w, SLOT(storeSettings()));
    }

    _ui->qlvSettingsCategories->setModel(new CategoryListModel(settingsWidgets));
    _ui->qlvSettingsCategories->setCurrentIndex(_ui->qlvSettingsCategories->model()->index(0, 0));
    on_qlvSettingsCategories_activated(_ui->qlvSettingsCategories->model()->index(0, 0));
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

void SettingsDialog::on_qlvSettingsCategories_activated(const QModelIndex &index)
{
   _ui->horizontalLayout->removeWidget(_ui->qwCurrentWidget);
   _ui->qwCurrentWidget->hide();
   _ui->qwCurrentWidget = _ui->qlvSettingsCategories->model()->data(index, Qt::UserRole).value<QWidget *>();
   _ui->qwCurrentWidget->setVisible(true);
   _ui->horizontalLayout->addWidget(_ui->qwCurrentWidget);
}
