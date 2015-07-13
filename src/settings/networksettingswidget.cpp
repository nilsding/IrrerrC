#include "networksettingswidget.h"
#include "ui_networksettingswidget.h"

NetworkSettingsWidget::NetworkSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NetworkSettingsWidget)
{
    ui->setupUi(this);
}

NetworkSettingsWidget::~NetworkSettingsWidget()
{
    delete ui;
}
