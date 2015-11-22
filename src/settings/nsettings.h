#ifndef NSETTINGS_H
#define NSETTINGS_H

#include <QObject>
#include <QSettings>

#define _SETTINGS NSettings::instance()

class NSettings : public QObject
{
    Q_OBJECT
public:
    static QSettings& instance()
    {
        static QSettings s(QSettings::IniFormat, QSettings::UserScope, "nilsding", "IrrerrC");
        return s;
    }

private:
    NSettings(NSettings const&) = delete;
    void operator=(NSettings const&) = delete;

};

#endif // NSETTINGS_H
