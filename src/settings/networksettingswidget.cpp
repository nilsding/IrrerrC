#include "networksettingswidget.h"
#include "ui_networksettingswidget.h"

NetworkSettingsWidget::NetworkSettingsWidget(QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::NetworkSettingsWidget)
{
    _ui->setupUi(this);
    loadSettings();
    _ui->qcbNetwork->clear();
    for (auto network : _networks) {
        _ui->qcbNetwork->addItem(network->name());
    }

    int i = 0;
    for (; i < _networks.size() && _networks.at(i)->isActive(); i++);
    _ui->qcbNetwork->setCurrentIndex(i);
}

NetworkSettingsWidget::~NetworkSettingsWidget()
{
    delete _ui;
}

void NetworkSettingsWidget::loadSettings()
{
    int size = _SETTINGS.beginReadArray("Networks");
        for (int i = 0; i < size; i++) {
            _SETTINGS.setArrayIndex(i);
            IrcNetwork *network = new IrcNetwork;
            network->setName(_SETTINGS.value("name").toString());
            network->setServers(_SETTINGS.value("servers").toStringList());
            network->setActive(_SETTINGS.value("active").toBool());
            _networks.append(network);
        }
    _SETTINGS.endArray();
}

void NetworkSettingsWidget::storeSettings()
{
    _SETTINGS.beginWriteArray("Networks");
        for (int i = 0; i < _networks.size(); ++i) {
            _SETTINGS.setArrayIndex(i);
            _SETTINGS.setValue("name", _networks.at(i)->name());
            _SETTINGS.setValue("servers", *_networks.at(i)->servers());
            _SETTINGS.setValue("active", _networks.at(i)->isActive());
        }
    _SETTINGS.endArray();
}

void NetworkSettingsWidget::on_qcbNetwork_currentIndexChanged(int index)
{
    if (index == -1) return;
    _ui->qlvServers->setModel(new QStringListModel(*_networks.at(index)->servers()));
}
