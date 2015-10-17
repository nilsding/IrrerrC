#ifndef IRCTEXTFORMATTER_H
#define IRCTEXTFORMATTER_H

#include <QObject>

#define IRC_FORMAT_BOLD      0x02
#define IRC_FORMAT_COLOR     0x03
#define IRC_FORMAT_RESET     0x0f
#define IRC_FORMAT_REVERSE   0x16
#define IRC_FORMAT_ITALIC    0x1D
#define IRC_FORMAT_UNDERLINE 0x1F

class IrcTextFormatter : public QObject
{
    Q_OBJECT
public:
    explicit IrcTextFormatter(QObject *parent = 0);
    ~IrcTextFormatter();

    QString parse(const QString &text) const;

signals:

public slots:
};

#endif // IRCTEXTFORMATTER_H
