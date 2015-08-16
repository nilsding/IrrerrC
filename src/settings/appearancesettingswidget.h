#ifndef APPEARANCESETTINGSWIDGET_H
#define APPEARANCESETTINGSWIDGET_H

#include <QWidget>
#include "nsettings.h"

namespace Ui {
class AppearanceSettingsWidget;
}

class AppearanceSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AppearanceSettingsWidget(QWidget *parent = 0);
    ~AppearanceSettingsWidget();

public slots:
    void loadSettings();
    void storeSettings();

private:
    Ui::AppearanceSettingsWidget *_ui;
};

#endif // APPEARANCESETTINGSWIDGET_H
