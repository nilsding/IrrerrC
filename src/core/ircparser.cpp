#include "ircparser.h"

IrcParser::IrcParser(QObject *parent) : QObject(parent)
{

}

IrcMessage *IrcParser::parseLine(QString s)
{
    // From RFC 2812 section 2.3.1:
    // message    =  [ ":" prefix SPACE ] command [ params ] crlf
    // prefix     =  servername / ( nickname [ [ "!" user ] "@" host ] )
    // command    =  1*letter / 3digit
    // params     =  *14( SPACE middle ) [ SPACE ":" trailing ]
    //            =/ 14( SPACE middle ) [ SPACE [ ":" ] trailing ]
    //
    // nospcrlfcl =  %x01-09 / %x0B-0C / %x0E-1F / %x21-39 / %x3B-FF
    //                 ; any octet except NUL, CR, LF, " " and ":"
    // middle     =  nospcrlfcl *( ":" / nospcrlfcl )
    // trailing   =  *( ":" / " " / nospcrlfcl )
    //
    // SPACE      =  %x20        ; space character
    // crlf       =  %x0D %x0A   ; "carriage return" "linefeed"

    IrcMessage *msg = new IrcMessage();
    int res = 0;

    //:dragon.nilsding.org 396 qnilsding rrerr-8oj.c39.4bg1jd.IP :is now your displayed host
    //:qnilsding!nIRC@rrerr-8oj.c39.4bg1jd.IP MODE qnilsding +x

    // prefix
    if (s[0] == ':') {
        s.remove(0, 1);
        res = s.indexOf(' ');
        if (res == -1) {
            goto fail;
        }
        msg->setPrefix(s.mid(0, res));
        s.remove(0, res + 1);
    }

    // command
    res = s.indexOf(' ');
    if (res == -1) {
        goto fail;
    }
    // TODO: check if command matches regexp /([a-z]+|\d{3})/i
    msg->setCommand(s.mid(0, res));
    s.remove(0, res + 1);

    // params
    if (s[0] == ':') {
        s.remove(0, 1);
    }

    msg->setParams(s);

    qDebug() << msg->prefix() << msg->command() << msg->params()->join(" ");

    if (this->parent() != 0) {
        if (!msg->command().compare("PING", Qt::CaseInsensitive)) {
            ((IrcConnection *) this->parent())->raw(new IrcMessage("", "PONG", msg->params()->first()), true);
        }
    }

    return msg;

fail:
    qDebug() << "ignoring malformed line:" << s;
    delete msg;
    return 0;
}
