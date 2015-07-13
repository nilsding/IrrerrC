#ifndef NETWORKSETTINGSWIDGET_H
#define NETWORKSETTINGSWIDGET_H

#include <QWidget>

namespace Ui {
class NetworkSettingsWidget;
}

class NetworkSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NetworkSettingsWidget(QWidget *parent = 0);
    ~NetworkSettingsWidget();

private:
    Ui::NetworkSettingsWidget *ui;
};

#endif // NETWORKSETTINGSWIDGET_H
