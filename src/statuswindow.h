#ifndef STATUSWINDOW_H
#define STATUSWINDOW_H

#include <QIcon>
#include <QDebug>
#include <QSplitter>
#include <QLineEdit>
#include <QTextEdit>
#include <QDateTime>
#include <QListView>
#include <QVBoxLayout>
#include <QStringListModel>

#include "core/ircmessage.h"
#include "settings/nsettings.h"

class StatusWindow : public QSplitter
{
    Q_OBJECT
    Q_PROPERTY(QString targetName READ targetName WRITE setTargetName)
    Q_PROPERTY(QString targetDescription READ targetDescription WRITE setTargetDescription)
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

signals:
    void textEntered(QString);

public slots:
    void onTextEntered();
    void onTextEdited(QString);
    void onUserActivated(QModelIndex);

    void onNamesReply(const QStringList &);
    void onEndOfNamesReply();

private:
    void createLayout();
    void createUserList();

    NWindowType _windowType;
    QLineEdit *_qleInput;
    QTextEdit *_qteBuffer;

    QString _targetName;
    QString _targetDescription;

    QList<QString> _userList;       //!< this is the full user list
    QList<QString> _tmpUserList;    //!< used when NAMES replies are received
    QListView *_qlvUsers;

    void loadSettings();
    void storeSettings();
};

#endif // STATUSWINDOW_H
