#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::AboutDialog)
{
    _ui->setupUi(this);
    _ui->qlLine01AppNameVersion->setText(QString("%1 %2").arg(APP_NAME).arg(APP_VERSION));
    _ui->qlLine04Copyright->setText(_ui->qlLine04Copyright->text().arg("2015").arg("FreeBSD"));
    _ui->qlLine06BuildDate->setText(_ui->qlLine06BuildDate->text().arg(QString("%1 %2").arg(__DATE__).arg(__TIME__)));
    _ui->qlLine07Commit->setText(_ui->qlLine07Commit->text().arg(COMMIT));
}

AboutDialog::~AboutDialog()
{
    delete _ui;
}
