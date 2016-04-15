#include "nscriptaction.h"

NScriptAction::NScriptAction(NJSEngine::ActionType type, const QString &text, QJSValue function, QAction *parent) :
    QAction(parent), _type(type), _function(function)
{
    this->setText(text);
    connect(this, SIGNAL(triggered(bool)), this, SLOT(trigger()));
}

void NScriptAction::trigger()
{
    if (!_function.isCallable()) {
        return;
    }

    switch (_type) {
    case NJSEngine::ActionType::ToolsMenu:
        _function.call();
        break;
    case NJSEngine::ActionType::UserMenu:
        break;
    case NJSEngine::ActionType::Unknown:
        // fallthrough intended
    default:
        break;
    }
}
