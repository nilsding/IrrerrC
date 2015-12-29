#ifndef SCRIPTLISTING_H
#define SCRIPTLISTING_H

#include <QDialog>

namespace Ui {
class ScriptListingDialog;
}

class ScriptListingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScriptListingDialog(QWidget *parent = 0);
    ~ScriptListingDialog();

private:
    Ui::ScriptListingDialog *_ui;
};

#endif // SCRIPTLISTING_H
