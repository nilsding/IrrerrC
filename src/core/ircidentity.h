#ifndef IRCIDENTITY_H
#define IRCIDENTITY_H

#include <QObject>

// TODO: write docs
class IrcIdentity : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString nickname READ nickname WRITE setNickname NOTIFY nicknameChanged)
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)
    Q_PROPERTY(QString realname READ realname WRITE setRealname NOTIFY realnameChanged)
    Q_PROPERTY(bool invisible READ isInvisible WRITE setInvisible NOTIFY invisibleChanged)
    Q_PROPERTY(bool wallops READ hasWallops WRITE setWallops NOTIFY wallopsChanged)
    Q_PROPERTY(QString partMessage READ partMessage WRITE setPartMessage NOTIFY partMessageChanged)
    Q_PROPERTY(QString quitMessage READ quitMessage WRITE setQuitMessage NOTIFY quitMessageChanged)

public:
    explicit IrcIdentity(QObject *parent = 0);

    unsigned int usermode();

    //!
    //! \brief nickname returns the prefered nickname for this identity
    //! \return the prefered nickname for this identity
    //!
    QString nickname() { return _nickname; }

    //!
    //! \brief setNickname sets the prefered nickname for this identity
    //! \param nickname the prefered nickname for this identity
    //!
    void setNickname(QString nickname) { _nickname = nickname; }

    // tbh fuck docs for getters and setters
    QString username() { return _username; }
    void setUsername(QString username) { _username = username; }

    QString realname() { return _realname; }
    void setRealname(QString realname) { _realname = realname; }

    bool isInvisible() { return _invisible; }
    void setInvisible(bool invisible) { _invisible = invisible; }

    bool hasWallops() { return _wallops; }
    void setWallops(bool wallops) { _wallops = wallops; }

    QString partMessage() { return _partMessage; }
    void setPartMessage(QString partMessage) { _partMessage = partMessage; }

    QString quitMessage() { return _quitMessage; }
    void setQuitMessage(QString quitMessage) { _quitMessage = quitMessage; }

signals:
    void nicknameChanged(QString nickname);
    void usernameChanged(QString username);
    void realnameChanged(QString realname);
    void invisibleChanged(bool invisible);
    void wallopsChanged(bool wallops);
    void partMessageChanged(QString partMessage);
    void quitMessageChanged(QString quitMessage);

public slots:

private:
    QString _nickname;
    QString _username;
    QString _realname;

    // user modes
    bool _invisible;        // +i
    bool _wallops;          // +w

    QString _partMessage;
    QString _quitMessage;
};

#endif // IRCIDENTITY_H
