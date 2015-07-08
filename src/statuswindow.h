#ifndef STATUSWINDOW_H
#define STATUSWINDOW_H

#include <QIcon>
#include <QSplitter>
#include <QLineEdit>
#include <QTextEdit>
#include <QDateTime>
#include <QVBoxLayout>

#include "core/ircmessage.h"

class StatusWindow : public QSplitter
{
    Q_OBJECT
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

signals:
    void textEntered(QString);

public slots:
    void onTextEntered();
    void onTextEdited(QString);

private:
    void createLayout();

    NWindowType _windowType;
    QLineEdit *_qleInput;
    QTextEdit *_qteBuffer;
};

#endif // STATUSWINDOW_H
