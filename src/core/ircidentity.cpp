#include "ircidentity.h"

IrcIdentity::IrcIdentity(QObject *parent) : QObject(parent)
{

}

//!
//! \brief IrcIdentity::usermode Returns the initial user mode.
//! This method should be called while the User message is being built.
//! \see RFC 2812 Section 3.1.3
//! \return the initial user mode
//!
unsigned int IrcIdentity::usermode()
{
    int i = 0;

    if (_wallops) {
        i |= (1 << 2);
    }
    if (_invisible) {
        i |= (1 << 3);
    }

    return i;
}
