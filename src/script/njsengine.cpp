#include "njsengine.h"
#include <QDebug>

bool NJSEngine::_initialized = false;

void NJSEngine::init()
{
    if (_initialized) {
        return;
    }
    qDebug() << "Initialized NJSEngine";

    // TODO: add some globals to the engine

    _initialized = true;
}
