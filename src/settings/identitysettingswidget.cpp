#include "identitysettingswidget.h"
#include "ui_identitysettingswidget.h"

IdentitySettingsWidget::IdentitySettingsWidget(QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::IdentitySettingsWidget)
{
    _ui->setupUi(this);
}

IdentitySettingsWidget::~IdentitySettingsWidget()
{
    delete _ui;
}
