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
        auto ff = _SETTINGS.value("fontFamily", DEFAULT_FONT).toString();
        _ui->qfcbStatusWindowFont->setCurrentFont(QFont(ff));
        _ui->qleTimestampFormat->setText(_SETTINGS.value("timestampFormat", "[HH:mm:ss]").toString());
    _SETTINGS.endGroup();
    _SETTINGS.beginGroup("MainWindow");
        _ui->qleMainWindowBackground->setText(_SETTINGS.value("backgroundImage", "").toString());
        _ui->qleMainToolbarBackground->setText(_SETTINGS.value("MainToolbar/backgroundImage", "").toString());
        _ui->qleWindowToolbarBackground->setText(_SETTINGS.value("WindowToolbar/backgroundImage", "").toString());
    _SETTINGS.endGroup();
}

void AppearanceSettingsWidget::storeSettings()
{
    _SETTINGS.beginGroup("StatusWindow");
        _SETTINGS.setValue("fontFamily", _ui->qfcbStatusWindowFont->currentFont().family());
        _SETTINGS.setValue("timestampFormat", _ui->qleTimestampFormat->text());
    _SETTINGS.endGroup();
    _SETTINGS.beginGroup("MainWindow");
        _SETTINGS.setValue("backgroundImage", _ui->qleMainWindowBackground->text());
        _SETTINGS.setValue("MainToolbar/backgroundImage", _ui->qleMainToolbarBackground->text());
        _SETTINGS.setValue("WindowToolbar/backgroundImage", _ui->qleWindowToolbarBackground->text());
    _SETTINGS.endGroup();
}

void AppearanceSettingsWidget::on_qpbMainToolbarBackgroundFileChooser_clicked()
{
    IMAGE_FILE_DIALOG(dlg);

    if (dlg.exec()) {
        _ui->qleMainToolbarBackground->setText(dlg.selectedFiles().first());
    }
}

void AppearanceSettingsWidget::on_qpbWindowToolbarBackgroundFileChooser_clicked()
{
    IMAGE_FILE_DIALOG(dlg);

    if (dlg.exec()) {
        _ui->qleWindowToolbarBackground->setText(dlg.selectedFiles().first());
    }
}

void AppearanceSettingsWidget::on_qleMainWindowBackgroundFileChooser_clicked()
{
    IMAGE_FILE_DIALOG(dlg);

    if (dlg.exec()) {
        _ui->qleMainWindowBackground->setText(dlg.selectedFiles().first());
    }
}
