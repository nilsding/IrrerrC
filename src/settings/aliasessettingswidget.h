#ifndef ALIASESSETTINGSWIDGET_H
#define ALIASESSETTINGSWIDGET_H

#include <QWidget>
#include "nsettings.h"
#include "../util/ircalias.h"

namespace Ui {
class AliasesSettingsWidget;
}

class AliasesSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AliasesSettingsWidget(QWidget *parent = 0);
    ~AliasesSettingsWidget();

public slots:
    void loadSettings();
    void storeSettings();

private:
    Ui::AliasesSettingsWidget *_ui;
    QList<IrcAlias *> _aliases;
};

#endif // ALIASESSETTINGSWIDGET_H
