#include "identitysettingswidget.h"
#include "ui_identitysettingswidget.h"

IdentitySettingsWidget::IdentitySettingsWidget(QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::IdentitySettingsWidget)
{
    _ui->setupUi(this);
    loadSettings();
}

IdentitySettingsWidget::~IdentitySettingsWidget()
{
    delete _ui;
}

void IdentitySettingsWidget::loadSettings()
{
    _SETTINGS.beginGroup("Identity");
        _ui->qleNickname->setText(_SETTINGS.value("nickname", "Lamer").toString());
        _ui->qleRealname->setText(_SETTINGS.value("realname", "Unknown lamer").toString());
        _ui->qcbWallops->setChecked(_SETTINGS.value("wallops", false).toBool());
        _ui->qcbInvisible->setChecked(_SETTINGS.value("invisible", true).toBool());
        _ui->qleUsername->setText(_SETTINGS.value("username", "Lamer").toString());
        _ui->qleQuitMessage->setText(_SETTINGS.value("quitmessage", "That was the wrong button!").toString());
    _SETTINGS.endGroup();
}

void IdentitySettingsWidget::storeSettings()
{
    _SETTINGS.beginGroup("Identity");
        _SETTINGS.setValue("nickname", _ui->qleNickname->text());
        _SETTINGS.setValue("realname",  _ui->qleRealname->text());
        _SETTINGS.setValue("wallops", _ui->qcbWallops->isChecked());
        _SETTINGS.setValue("invisible", _ui->qcbInvisible->isChecked());
        _SETTINGS.setValue("username", _ui->qleUsername->text());
        _SETTINGS.setValue("quitmessage", _ui->qleQuitMessage->text());
    _SETTINGS.endGroup();
}
