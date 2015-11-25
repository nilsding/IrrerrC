#ifndef NSETTINGS_H
#define NSETTINGS_H

#include <QObject>
#include <QSettings>
#include <QFile>

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

    static bool settingsFileExists()
    {
        return QFile::exists(instance().fileName());
    }

private:
    NSettings(NSettings const&) = delete;
    void operator=(NSettings const&) = delete;

};

#endif // NSETTINGS_H
