#ifndef ALIASESSETTINGSWIDGET_H
#define ALIASESSETTINGSWIDGET_H

#include <QWidget>
#include <QIcon>
#include <QMessageBox>
#include <QAbstractListModel>
#include "nsettings.h"
#include "editaliasdialog.h"
#include "../util/ircalias.h"

class AliasesListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    AliasesListModel(QList<IrcAlias *> *data) : _data(data) { }


    int rowCount(const QModelIndex &/*parent*/) const {
        return _data->size();
    }

    int columnCount(const QModelIndex &/*parent*/) const {
        return 2;
    }

    QVariant data(const QModelIndex &index, int role) const {
        if (index.row() < 0 || index.row() > _data->size() ||
            index.column() < 0 || index.column() > columnCount(index)) {
            return QVariant(QVariant::Invalid);
        }

        auto retobj = _data->at(index.row());
        switch (role) {
        case Qt::DisplayRole:
            switch (index.column()) {
            case 0:
                return retobj->alias();
            case 1:
                return retobj->action();
            }
        case Qt::DecorationRole:
            if (index.column() != 0) {
                return QVariant(QVariant::Invalid);
            }
            switch (retobj->type()) {
            // TODO: change these icons
            case IrcAlias::AliasSimple:
                return QIcon(":/icons/channel");

            case IrcAlias::AliasScript:
                return QIcon(":/icons/connect");

            }
        }

        return QVariant(QVariant::Invalid);
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const {
        if (role != Qt::DisplayRole || orientation == Qt::Vertical) {
            return QVariant(QVariant::Invalid);
        }

        switch (section) {
            case 0: {
                return tr("Alias");
            }
            case 1: {
                return tr("Action");
            }
        }

        return QVariant(QVariant::Invalid);
    }

private:
    QList<IrcAlias *> *_data;
};

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

private slots:
    void on_qpbAdd_clicked();

    void on_qpbEdit_clicked();

    void on_qpbDelete_clicked();

private:
    Ui::AliasesSettingsWidget *_ui;
    QList<IrcAlias *> _aliases;

    bool is_alias_entry_selected();
};

#endif // ALIASESSETTINGSWIDGET_H
