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
    for (; i < _networks.size() && !_networks.at(i)->isActive(); ++i);
    _ui->qcbNetwork->setCurrentIndex(i);
    _ui->qcbNetwork->repaint();
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

void NetworkSettingsWidget::on_qpbAddServer_clicked()
{
    bool ok = false;
    QString text = QInputDialog::getText(this, tr("Add server to network"), tr("Server address:"), QLineEdit::Normal,
                                         "irc.rrerr.net:6667", &ok);

    if (ok && !text.isEmpty()) {
        // TODO: check if network address is in a valid format
        auto i = _ui->qcbNetwork->currentIndex();
        _networks.at(i)->servers()->append(text);
        _ui->qlvServers->setModel(new QStringListModel(*_networks.at(i)->servers()));
    }
}

void NetworkSettingsWidget::on_qpbEditServer_clicked()
{
    if (!is_server_entry_selected()) {
        return;
    }

    bool ok = false;
    auto i = _ui->qcbNetwork->currentIndex();
    QString selected_server = _networks.at(i)->servers()->at(_ui->qlvServers->selectionModel()->selectedIndexes().first().row());
    QString text = QInputDialog::getText(this, tr("Edit server address"), tr("Server address:"), QLineEdit::Normal,
                                         selected_server, &ok);

    if (ok && !text.isEmpty()) {
        // TODO: check if network address is in a valid format
        _networks.at(i)->servers()->replace(_ui->qlvServers->selectionModel()->selectedIndexes().first().row(), text);
        _ui->qlvServers->setModel(new QStringListModel(*_networks.at(i)->servers()));
    }
}

void NetworkSettingsWidget::on_qpbDeleteServer_clicked()
{
    if (!is_server_entry_selected()) {
        return;
    }

    auto i = _ui->qcbNetwork->currentIndex();
    if (_networks.at(i)->servers()->length() == 1) {
        QMessageBox mbox;
        mbox.setText(tr("A network must have at least one server to connect to."));
        mbox.exec();
        return;
    }

    auto selected_index = _ui->qlvServers->selectionModel()->selectedIndexes().first();
    _networks.at(i)->servers()->removeAt(selected_index.row());
    _ui->qlvServers->setModel(new QStringListModel(*_networks.at(i)->servers()));

    // select the last selected index for convenience
    _ui->qlvServers->selectionModel()->select(selected_index, QItemSelectionModel::Select);
}

bool NetworkSettingsWidget::is_server_entry_selected()
{
    if (_ui->qlvServers->selectionModel()->selectedIndexes().empty()) {
        QMessageBox mbox;
        mbox.setText(tr("You must select a server entry first."));
        mbox.exec();
        return false;
    }
    return true;
}
