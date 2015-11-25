#ifndef IRCTEXTFORMATTER_H
#define IRCTEXTFORMATTER_H

#include <QObject>
#include <QPair>
#include <QRegExp>
#include <utility>

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

    static QString sanitize(QString str);

    QPair<int, int> parseColor(const QString &text, int *len) const;
    QString parse(const QString &text) const;
    QString strip(const QString &text) const;

signals:

public slots:

private:
    QString ircColorToHex(int code) const;
};

#endif // IRCTEXTFORMATTER_H
