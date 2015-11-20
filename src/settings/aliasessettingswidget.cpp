#include "aliasessettingswidget.h"
#include "ui_aliasessettingswidget.h"

AliasesSettingsWidget::AliasesSettingsWidget(QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::AliasesSettingsWidget)
{
    _ui->setupUi(this);
}

AliasesSettingsWidget::~AliasesSettingsWidget()
{
    delete _ui;
}

void AliasesSettingsWidget::loadSettings()
{
    int size = _SETTINGS.beginReadArray("Aliases");
        for (int i = 0; i < size; i++) {
            _SETTINGS.setArrayIndex(i);
            IrcAlias *alias = new IrcAlias;
            alias->setType((IrcAlias::IrcAliasType) _SETTINGS.value("name").toInt());
            alias->setAlias(_SETTINGS.value("alias").toString());
            alias->setAction(_SETTINGS.value("action").toString());
            _aliases.append(alias);
        }
    _SETTINGS.endArray();
}

void AliasesSettingsWidget::storeSettings()
{
    _SETTINGS.beginWriteArray("Aliases");
        for (int i = 0; i < _aliases.size(); ++i) {
            _SETTINGS.setArrayIndex(i);
            _SETTINGS.setValue("type", _aliases.at(i)->type());
            _SETTINGS.setValue("alias", _aliases.at(i)->alias());
            _SETTINGS.setValue("action", _aliases.at(i)->action());
        }
    _SETTINGS.endArray();
}
