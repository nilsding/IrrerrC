#ifndef IDENTITYSETTINGSWIDGET_H
#define IDENTITYSETTINGSWIDGET_H

#include <QWidget>

namespace Ui {
class IdentitySettingsWidget;
}

class IdentitySettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IdentitySettingsWidget(QWidget *parent = 0);
    ~IdentitySettingsWidget();

private:
    Ui::IdentitySettingsWidget *_ui;
};

#endif // IDENTITYSETTINGSWIDGET_H
