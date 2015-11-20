#ifndef EDITALIASDIALOG_H
#define EDITALIASDIALOG_H

#include <QDialog>
#include "../util/ircalias.h"

namespace Ui {
class EditAliasDialog;
}

class EditAliasDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditAliasDialog(QWidget *parent = 0);
    ~EditAliasDialog();

    IrcAlias::IrcAliasType type() const;
    QString alias() const;
    QString action() const;

    void setType(IrcAlias::IrcAliasType type);
    void setAlias(QString alias);
    void setAction(QString action);

private:
    Ui::EditAliasDialog *_ui;
};

#endif // EDITALIASDIALOG_H
