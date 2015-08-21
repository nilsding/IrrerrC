#ifndef STATUSWINDOW_H
#define STATUSWINDOW_H

#include <QIcon>
#include <QDebug>
#include <QSplitter>
#include <QLineEdit>
#include <QTextEdit>
#include <QDateTime>
#include <QListView>
#include <QTreeView>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QCloseEvent>
#include <QMessageBox>
#include <QStringListModel>

#include "core/irctypes.h"
#include "core/ircparser.h"
#include "core/ircmessage.h"
#include "settings/nsettings.h"


class ChannelListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    ChannelListModel(QList<IrcTypes::ListEntry> *data) : _data(data) { }

    int rowCount(const QModelIndex &/*parent*/) const {
        return _data->size();
    }

    int columnCount(const QModelIndex &/*parent*/) const {
        return 3;
    }

    QVariant data(const QModelIndex &index, int role) const {
        if (index.row() < 0 || index.row() > _data->size() ||
            index.column() < 0 || index.column() > columnCount(index)) {
            return QVariant(QVariant::Invalid);
        }

        auto retobj = &_data->at(index.row());
        if (role == Qt::DisplayRole) {
            switch (index.column()) {
                case 0: {
                    return retobj->channelName();
                }
                case 1: {
                    return retobj->userCount();
                }
                case 2: {
                    return retobj->topic();
                }
            }
        }

        return QVariant(QVariant::Invalid);
    }

    QVariant headerData(int section, Qt::Orientation orientation, int /*role*/) const {
        if (orientation == Qt::Vertical) {
            return QVariant(QVariant::Invalid);
        }

        switch (section) {
            case 0: {
                return tr("Channel");
            }
            case 1: {
                return tr("Users");
            }
            case 2: {
                return tr("Topic");
            }
        }

        return QVariant(QVariant::Invalid);
    }

private:
    QList<IrcTypes::ListEntry> *_data;
};

class StatusWindow : public QSplitter
{
    Q_OBJECT
    Q_PROPERTY(QString targetName READ targetName WRITE setTargetName)
    Q_PROPERTY(QString targetDescription READ targetDescription WRITE setTargetDescription)
    Q_PROPERTY(NWindowType type READ type)
    Q_ENUMS(NWindowType)

public:
    enum NWindowType {
        NWindowStatus,
        NWindowChannel,
        NWindowList,
        NWindowQuery
    };

    explicit StatusWindow(NWindowType windowType = NWindowStatus, QWidget *parent = 0);
    ~StatusWindow();

    void receiveMessage(IrcMessage *);
    void closeEvent(QCloseEvent *);

    QString targetName() { return _targetName; }
    void setTargetName(QString targetName) { _targetName = targetName;
                                             setWindowTitle(_targetDescription.isEmpty()
                                                            ? _targetName
                                                            : QString("%1 - %2").arg(_targetName)
                                                              .arg(_targetDescription)); }
    QString targetDescription() { return _targetDescription; }
    void setTargetDescription(QString targetDescription) { _targetDescription = targetDescription;
                                                           setWindowTitle(_targetDescription.isEmpty()
                                                                          ? _targetName
                                                                          : QString("%1 - %2").arg(_targetName)
                                                                            .arg(_targetDescription)); }

    NWindowType type() { return _windowType; }

signals:
    void textEntered(QString);
    void statusWindowClosing();

public slots:
    void onTextEntered();
    void onTextEdited(QString);
    void onUserActivated(QModelIndex);

    void onNamesReply(const QStringList &);
    void onEndOfNamesReply();
    void onListReply(QString, int, QString);
    void onEndOfListReply();

private:
    void createLayout();
    void createUserList();
    void createChannelList();

    NWindowType _windowType;
    QLineEdit *_qleInput;
    QTextEdit *_qteBuffer;

    QString _targetName;
    QString _targetDescription;

    QList<QString> _userList;       //!< this is the full user list
    QList<QString> _tmpUserList;    //!< used when NAMES replies are received
    QListView *_qlvUsers;

    // same as above:
    QList<IrcTypes::ListEntry> _channelList;
    QList<IrcTypes::ListEntry> _tmpChannelList;
    QTreeView *_qtvChannels;

    void loadSettings();
    void storeSettings();
};

#endif // STATUSWINDOW_H
