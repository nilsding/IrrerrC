#ifndef IRCNETWORK_H
#define IRCNETWORK_H

#include <QObject>
#include <QStringList>

class IrcNetwork : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QStringList *servers READ servers WRITE setServers NOTIFY serversChanged)
    Q_PROPERTY(bool active READ isActive WRITE setActive)

public:
    explicit IrcNetwork(QObject *parent = 0);

    void addServer(QString host, int port = 6667, bool useTls = false);

    QString getServer(int index = -1);

    QString name() { return _name; }
    void setName(QString name) { _name = name; }

    QStringList *servers() { return _servers; }
    void setServers(QStringList *servers) { if (_servers != 0) { delete _servers; } _servers = servers; }
    void setServers(QStringList servers) {
        for (auto server : servers) {
            _servers->append(server);
        }
    }

    bool isActive() { return _active; }
    void setActive(bool active) { _active = active; }

signals:
    void nameChanged(QString name);
    void serversChanged(QString name);

public slots:

private:
    QString _name;
    QStringList *_servers;
    bool _active;
};

#endif // IRCNETWORK_H
