#ifndef NCLICKABLELABEL_H
#define NCLICKABLELABEL_H

#include <QLabel>

class NClickableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit NClickableLabel(QWidget* parent = 0, const QString& text = "");
    ~NClickableLabel();

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* event);
};

#endif // NCLICKABLELABEL_H
