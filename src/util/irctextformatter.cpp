#include "irctextformatter.h"

#include <QDebug>

IrcTextFormatter::IrcTextFormatter(QObject *parent) : QObject(parent)
{

}

IrcTextFormatter::~IrcTextFormatter()
{

}

//!
//! \brief IrcTextFormatter::sanitize """sanitizes""" a given string `s`.
//!     What it really does is just replace every '&' character with "&amp;" and every space with "&nbsp;".\
//! \param str A string
//! \return Another string
//!
QString IrcTextFormatter::sanitize(QString str)
{
    return str.replace('&', "&amp;").replace(' ', "&nbsp;");
}

//!
//! \brief IrcTextFormatter::parseColor processes `str` for the first color pair match.
//!     `str` must not include the color format character `0x03`, and it has to start with the color pair.
//! \param text
//! \example parseColor("3,4");  // returns QPair<int, int> with 3 and 4 as values
//! \return first: foreground color, second: background color.  If one of the values is -1, it is not set.
//!
QPair<int, int> IrcTextFormatter::parseColor(const QString &str, int *len) const
{
    int fg, bg;
    bool ok = false;
    fg = bg = -1;
    *len = 0;

    QRegExp re("^(\\d+)(?:,(\\d+))?");

    if (re.indexIn(str) == -1) {
        return QPair<int, int>(fg, bg);
    }
    *len = re.cap(0).length();

    fg = re.cap(1).toInt(&ok);
    if (!ok) {
        return QPair<int, int>(-1, -1);
    }

    if (re.captureCount() > 1) {
        bg = re.cap(2).toInt(&ok);
        if (!ok) {
            bg = -1;
        }
    }

    return QPair<int, int>(fg, bg);
}

//!
//! \brief IrcTextFormatter::parse processes `text` for special formatting characters (e.g. bold, colour, ...) and
//!     returns them as HTML with <span> elements.
//! \param text the text to process
//! \return HTMLified text.
//!
QString IrcTextFormatter::parse(const QString &text) const
{
    QString str = text;
    int position;
    char fg, bg, bold, reverse, italic, underline;
    str.replace('<', "&lt;");

    // initialize attributes
    fg = bg = -1;
    bold = reverse = italic = underline = 0;
    position = 0;

    while (position < str.size()) {
        QString s;
        bool formattingChanged = false;
        switch (str.at(position).unicode()) {
        case IRC_FORMAT_BOLD:
            formattingChanged = true;
            bold ^= 1;
            break;

        case IRC_FORMAT_COLOR: {
            formattingChanged = true;
            int skip = -1;
            auto colors = parseColor(str.mid(1 + position), &skip);
            fg = colors.first;
            if (fg != -1 && colors.second != -1) {
                bg = fg == -1 ? -1 : colors.second;
            }

            str.remove(position, skip);

            break;
        }

        case IRC_FORMAT_RESET:
            if (bold || reverse || italic || underline || (fg != -1)) {
                s = "</span>";
            } else {
                str.remove(position--, 1);
            }
            fg = bg = -1;
            bold = reverse = italic = underline = 0;
            break;

        case IRC_FORMAT_REVERSE:
            // TODO: reverse video
            break;

        case IRC_FORMAT_ITALIC:
            formattingChanged = true;
            italic ^= 1;
            break;

        case IRC_FORMAT_UNDERLINE:
            formattingChanged = true;
            underline ^= 1;
            break;
        }

        if (formattingChanged) {
            s = "</span><span style=\"";
            if (bold)      s.append("font-weight: bold;");
            if (italic)    s.append("font-style: italic;");
            if (underline) s.append("text-decoration: underline;");
            if (fg != -1) {
                s.append("color: ");
                s.append(ircColorToHex(fg));
                s.append(";");
                if (bg != -1) {
                    s.append("background-color: ");
                    s.append(ircColorToHex(bg));
                    s.append(";");
                }
            }
            s.append("\">");
        }

        if (!s.isEmpty()) {
            str.replace(position, 1, s);
            position += s.length();
        } else {
            position++;
        }
    }

    return "<span>" + str;
}

//!
//! \brief IrcTextFormatter::strip removes special formatting characters (e.g. bold, colour, ...) from `text`
//! \param text the text to process
//! \return text without formatting
//!
QString IrcTextFormatter::strip(const QString &text) const
{
    QString str = text;
    int position = 0;

    while (position < str.size()) {
        int adv = 0;
        switch (str.at(position).unicode()) {
        case IRC_FORMAT_COLOR:
            parseColor(str.mid(1 + position), &adv);
            // fallthrough intended
        case IRC_FORMAT_BOLD:
        case IRC_FORMAT_RESET:
        case IRC_FORMAT_REVERSE:
        case IRC_FORMAT_UNDERLINE:
            adv++;
            break;
        }

        if (adv) {
            str.replace(position, adv, "");
            position += adv;
        } else {
            position++;
        }
    }

    return str;
}

QString IrcTextFormatter::ircColorToHex(int code) const
{
    // http://www.mirc.com/colors.html
    switch (code) {
    case 0:  return "#ffffff";  // white
    case 1:  return "#000000";  // black
    case 2:  return "#00007f";  // blue
    case 3:  return "#009300";  // green
    case 4:  return "#ff0000";  // light red
    case 5:  return "#7f0000";  // Brown/dark red
    case 6:  return "#9c009c";  // purple
    case 7:  return "#fc7f00";  // orange
    case 8:  return "#ffff00";  // yellow
    case 9:  return "#00fc00";  // light green
    case 10: return "#009393";  // cyan
    case 11: return "#00ffff";  // light cyan
    case 12: return "#0000fc";  // blue
    case 13: return "#ff00ff";  // pink
    case 14: return "#7f7f7f";  // grey
    case 15: return "#d2d2d2";  // light grey
    default: return "inherit";  // system default
    }
}
