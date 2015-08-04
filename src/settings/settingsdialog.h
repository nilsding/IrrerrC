#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QAbstractButton>
#include <QAbstractListModel>

class CategoryListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    CategoryListModel(QList<QWidget *> data) : _data(data) { }

    int rowCount(const QModelIndex &/*parent*/) const {
        return _data.size();
    }

    QVariant data(const QModelIndex &index, int role) const {
        if (index.row() < 0 || index.row() > _data.size()) {
            return QVariant(QVariant::Invalid);
        }

        auto retobj = _data.at(index.row());
        switch (role) {
            case Qt::DisplayRole: {
                return retobj->windowTitle();
            }
            case Qt::DecorationRole: {
                return retobj->windowIcon();
            }
            case Qt::UserRole: {
                return QVariant::fromValue(retobj);
            }
        }

        return QVariant(QVariant::Invalid);
    }

private:
    QList<QWidget *> _data;
};

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

private slots:
    void on_qdbbButtons_clicked(QAbstractButton *button);

    void on_qlvSettingsCategories_clicked(const QModelIndex &index);

private:
    Ui::SettingsDialog *_ui;

    QList<QWidget *> _settingsWidgets;
};

#endif // SETTINGSDIALOG_H
