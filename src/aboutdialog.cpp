#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::AboutDialog)
{
    _ui->setupUi(this);
    this->setWindowTitle(tr("About %1").arg(APP_NAME));
    _ui->qlLine01AppNameVersion->setText(QString("%1 %2").arg(APP_NAME).arg(APP_VERSION));
    _ui->qlLine04Copyright->setText(_ui->qlLine04Copyright->text().arg("2015").arg("FreeBSD"));
    _ui->qlLine06BuildDate->setText(_ui->qlLine06BuildDate->text().arg(QString("%1 %2").arg(__DATE__).arg(__TIME__)));
    _ui->qlLine07Commit->setText(_ui->qlLine07Commit->text().arg(COMMIT));
}

AboutDialog::~AboutDialog()
{
    delete _ui;
}

void AboutDialog::on_nclAppIcon_clicked()
{
    static int count = 0;
    static bool isUrl = true;
    QString url = "https://github.com/nilsding/IrrerrC";
    switch (++count) {
    case 1:
        url = "http://reddit.com/r/kreiswichs";
        break;
    case 2:
        isUrl = false;
        url = "@Revengeday ist der beste!";
        break;
    case 3:
        isUrl = true;
        url = "http://rrerr.net";
        break;
    case 4:
        url = "http://reddit.com/r/jonajokes";
        break;
    case 5:
        url = "http://irc.rrerr.net";
        break;
    case 6:
        url = "http://bad-dragon.com";
        break;
    default:
        count = 0;
        isUrl = true;
    }
    _ui->qlLine05Url->setText(isUrl ? ("<a href=\"" + url + "\">" + url + "</a>") : url);
}
