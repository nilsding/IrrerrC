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
    int position;
    char fg, bg, bold, reverse, italic, underline;
    str.replace('<', "&lt;");

    // initialize attributes
    fg = bg = bold = reverse = italic = underline = 0;
    position = 0;

    while (position < str.size()) {
        QString s;
        bool formattingChanged = false;
        switch (str.at(position).unicode()) {
        case IRC_FORMAT_BOLD:
            formattingChanged = true;
            bold ^= 1;
            break;

        case IRC_FORMAT_COLOR:
            // TODO: parse colours
            break;

        case IRC_FORMAT_RESET:
            if (bold || reverse || italic || underline) {
                s = "</span>";
            } else {
                str.remove(position--, 1);
            }
            fg = bg = bold = reverse = italic = underline = 0;
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
