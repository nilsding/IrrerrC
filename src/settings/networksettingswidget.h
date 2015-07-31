#ifndef NETWORKSETTINGSWIDGET_H
#define NETWORKSETTINGSWIDGET_H

#include <QWidget>
#include <QStringListModel>
#include <QMessageBox>
#include <QInputDialog>
#include "nsettings.h"
#include "../core/ircnetwork.h"

namespace Ui {
class NetworkSettingsWidget;
}

class NetworkSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NetworkSettingsWidget(QWidget *parent = 0);
    ~NetworkSettingsWidget();

public slots:
    void loadSettings();
    void storeSettings();

private slots:
    void on_qcbNetwork_currentIndexChanged(int index);

    void on_qpbAddServer_clicked();

    void on_qpbEditServer_clicked();

    void on_qpbDeleteServer_clicked();

    void on_qpbNewNetwork_clicked();

    void on_qpbRenameNetwork_clicked();

    void on_qpbDeleteNetwork_clicked();

private:
    Ui::NetworkSettingsWidget *_ui;
    QList<IrcNetwork *> _networks;

    bool is_server_entry_selected();
};

#endif // NETWORKSETTINGSWIDGET_H
