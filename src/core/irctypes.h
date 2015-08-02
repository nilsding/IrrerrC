#ifndef IRCTYPES_H
#define IRCTYPES_H

#include <QObject>

namespace IrcTypes
{

class ListEntry : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString channelName READ channelName WRITE setChannelName)
    Q_PROPERTY(int userCount READ userCount WRITE setUserCount)
    Q_PROPERTY(QString topic READ topic WRITE setTopic)

public:

    ListEntry(const ListEntry& other) :
        _channelName(other._channelName), _userCount(other._userCount), _topic(other._topic) {}
    ListEntry(QString channelName, int userCount, QString topic) :
        _channelName(channelName), _userCount(userCount), _topic(topic) {}

    QString channelName() const { return _channelName; }
    void setChannelName(QString channelName) { _channelName = channelName; }
    int userCount() const { return _userCount; }
    void setUserCount(int userCount) { _userCount = userCount; }
    QString topic() const { return _topic; }
    void setTopic(QString topic) { _topic = topic; }

    ListEntry& operator=(const ListEntry& other) {
         _channelName = other._channelName;
         _userCount = other._userCount;
         _topic = other._topic;
         return *this;
     }

private:

    QString _channelName;
    int _userCount;
    QString _topic;
};

}

#endif // IRCTYPES_H

