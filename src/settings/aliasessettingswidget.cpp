#include "aliasessettingswidget.h"
#include "ui_aliasessettingswidget.h"

AliasesSettingsWidget::AliasesSettingsWidget(QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::AliasesSettingsWidget)
{
    _ui->setupUi(this);
    loadSettings();
    _ui->qtvAliases->header()->setStretchLastSection(true);
    _ui->qtvAliases->header()->setSectionsMovable(false);
    _ui->qtvAliases->setModel(new AliasesListModel(&_aliases));
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
            alias->setType(static_cast<IrcAlias::IrcAliasType>(_SETTINGS.value("type").toUInt()));
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

void AliasesSettingsWidget::on_qpbAdd_clicked()
{
    EditAliasDialog dlg;

    if (dlg.exec() == QDialog::Accepted) {
        IrcAlias *alias = new IrcAlias;
        alias->setType(dlg.type());
        alias->setAlias(dlg.alias());
        alias->setAction(dlg.action());
        _aliases.append(alias);

        auto oldModel = _ui->qtvAliases->model();
        _ui->qtvAliases->setModel(new AliasesListModel(&_aliases));
        oldModel->deleteLater();
    }
}

void AliasesSettingsWidget::on_qpbEdit_clicked()
{
    if (!is_alias_entry_selected()) {
        return;
    }

    auto selectedIndex = _ui->qtvAliases->selectionModel()->selectedIndexes().first();
    auto selectedAlias = _aliases.at(selectedIndex.row());
    EditAliasDialog dlg;
    dlg.setType(selectedAlias->type());
    dlg.setAlias(selectedAlias->alias());
    dlg.setAction(selectedAlias->action());

    if (dlg.exec() == QDialog::Accepted) {
        selectedAlias->setType(dlg.type());
        selectedAlias->setAlias(dlg.alias());
        selectedAlias->setAction(dlg.action());

        auto oldModel = _ui->qtvAliases->model();
        _ui->qtvAliases->setModel(new AliasesListModel(&_aliases));
        oldModel->deleteLater();
    }
}

void AliasesSettingsWidget::on_qpbDelete_clicked()
{
    if (!is_alias_entry_selected()) {
        return;
    }

    auto selectedIndex = _ui->qtvAliases->selectionModel()->selectedIndexes().first();
    _aliases.removeAt(selectedIndex.row());

    auto oldModel = _ui->qtvAliases->model();
    _ui->qtvAliases->setModel(new AliasesListModel(&_aliases));
    oldModel->deleteLater();
}

bool AliasesSettingsWidget::is_alias_entry_selected()
{
    if (_ui->qtvAliases->selectionModel()->selectedIndexes().empty()) {
        QMessageBox mbox;
        mbox.setText(tr("You must select an alias first."));
        mbox.exec();
        return false;
    }
    return true;
}
