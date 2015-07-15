#ifndef NETWORKSETTINGSWIDGET_H
#define NETWORKSETTINGSWIDGET_H

#include <QWidget>
#include <QStringListModel>
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

private:
    Ui::NetworkSettingsWidget *_ui;
    QList<IrcNetwork *> _networks;
};

#endif // NETWORKSETTINGSWIDGET_H
