#include "irctextformatter.h"

IrcTextFormatter::IrcTextFormatter(QObject *parent) : QObject(parent)
{

}

IrcTextFormatter::~IrcTextFormatter()
{

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
    int position, depth;
    char fg, bg, bold, reverse, italic, underline;
    str.replace('<', "&lt;");

    // initialize attributes
    fg = bg = bold = reverse = italic = underline = 0;
    depth = position = 0;

    // TODO: fix this, it fails on strings like
    //   \x02fett\x02 nicht fett \x02fett \x1ffett und unterstrichen \x02nicht mehr fett \x02wieder fett \x0f NIX
    while (position < str.size()) {
        QString s;
        switch (str.at(position).unicode()) {
        case IRC_FORMAT_BOLD:
            if (bold) {
                depth--;
                s = "</span>";
            } else {
                depth++;
                s = "<span style=\"font-weight: bold;\">";
            }
            bold ^= 1;
            break;

        case IRC_FORMAT_COLOR:
            // TODO: parse colours
            break;

        case IRC_FORMAT_RESET:
            if (depth > 0) {
                s = QString("</span>").repeated(depth);
            } else {
                str.remove(position--, 1);
            }
            fg = bg = bold = reverse = italic = underline = 0;
            break;

        case IRC_FORMAT_REVERSE:
            // TODO: reverse video
            break;

        case IRC_FORMAT_ITALIC:
            if (italic) {
                depth--;
                s = "</span>";
            } else {
                depth++;
                s = "<span style=\"font-style: italic;\">";
            }
            italic ^= 1;
            break;

        case IRC_FORMAT_UNDERLINE:
            if (underline) {
                depth--;
                s = "</span>";
            } else {
                depth++;
                s = "<span style=\"text-decoration: underline;\">";
            }
            underline ^= 1;
            break;
        }

        if (!s.isEmpty()) {
            str.replace(position, 1, s);
            position += s.length();
        } else {
            position++;
        }
    }

    return str;
}
