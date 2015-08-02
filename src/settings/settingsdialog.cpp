#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include "identitysettingswidget.h"
#include "networksettingswidget.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::SettingsDialog)
{
    _ui->setupUi(this);

    _settingsWidgets.append(new IdentitySettingsWidget);
    _settingsWidgets.append(new NetworkSettingsWidget);

    for (auto w : _settingsWidgets) {
        connect(this, SIGNAL(accepted()), w, SLOT(storeSettings()));
    }

    _ui->qlvSettingsCategories->setModel(new CategoryListModel(_settingsWidgets));
    _ui->qlvSettingsCategories->setCurrentIndex(_ui->qlvSettingsCategories->model()->index(0, 0));

    for (int i = _settingsWidgets.length() - 1; i >= 0; i--) {
        on_qlvSettingsCategories_clicked(_ui->qlvSettingsCategories->model()->index(i, 0));
    }
}

SettingsDialog::~SettingsDialog()
{
    for (auto w : _settingsWidgets) {
        w->deleteLater();
    }
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

void SettingsDialog::on_qlvSettingsCategories_clicked(const QModelIndex &index)
{
    _ui->horizontalLayout->removeWidget(_ui->qwCurrentWidget);
    _ui->qwCurrentWidget->hide();
    _ui->qwCurrentWidget = _ui->qlvSettingsCategories->model()->data(index, Qt::UserRole).value<QWidget *>();
    _ui->qwCurrentWidget->setVisible(true);
    _ui->horizontalLayout->addWidget(_ui->qwCurrentWidget);
}
