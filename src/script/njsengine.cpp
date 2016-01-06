#include "njsengine.h"

bool NJSEngine::_initialized = false;

void NJSEngine::init()
{
    if (_initialized) {
        return;
    }

    // TODO: add some globals to the engine

    _initialized = true;
}
