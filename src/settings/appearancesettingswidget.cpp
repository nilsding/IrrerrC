#include "appearancesettingswidget.h"
#include "ui_appearancesettingswidget.h"

AppearanceSettingsWidget::AppearanceSettingsWidget(QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::AppearanceSettingsWidget)
{
    _ui->setupUi(this);
    loadSettings();
}

AppearanceSettingsWidget::~AppearanceSettingsWidget()
{
    delete _ui;
}

void AppearanceSettingsWidget::loadSettings()
{
    _SETTINGS.beginGroup("StatusWindow");
        auto ff = _SETTINGS.value("fontFamily", "monospace").toString();
        _ui->qfcbStatusWindowFont->setCurrentFont(QFont(ff));
        _ui->qleTimestampFormat->setText(_SETTINGS.value("timestampFormat", "[HH:mm:ss]").toString());
    _SETTINGS.endGroup();
}

void AppearanceSettingsWidget::storeSettings()
{
    _SETTINGS.beginGroup("StatusWindow");
        _SETTINGS.setValue("fontFamily", _ui->qfcbStatusWindowFont->currentFont().family());
        _SETTINGS.setValue("timestampFormat", _ui->qleTimestampFormat->text());
    _SETTINGS.endGroup();
}
