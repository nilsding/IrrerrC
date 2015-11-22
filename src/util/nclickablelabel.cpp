#include "nclickablelabel.h"

NClickableLabel::NClickableLabel(QWidget* parent, const QString& text)
    : QLabel(parent)
{
    setText(text);
}

NClickableLabel::~NClickableLabel()
{
}

void NClickableLabel::mousePressEvent(QMouseEvent* /*event*/)
{
    emit clicked();
}
