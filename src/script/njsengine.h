#ifndef NJSENGINE_H
#define NJSENGINE_H

#include <QObject>
#include <QJSEngine>

#define _JSENGINE NJSEngine::instance()

class NJSEngine : public QObject
{
    Q_OBJECT
public:
    static void init();

    static QJSEngine& instance()
    {
        return _engine;
    }

signals:

public slots:

private:
    static QJSEngine _engine;
    static bool _initialized;
    Q_DISABLE_COPY(NJSEngine)
};

#endif // NJSENGINE_H
