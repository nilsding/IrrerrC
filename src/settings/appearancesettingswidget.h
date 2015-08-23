#ifndef APPEARANCESETTINGSWIDGET_H
#define APPEARANCESETTINGSWIDGET_H

#include <QWidget>
#include <QFileDialog>
#include "nsettings.h"

#define IMAGE_FILE_DIALOG(x) \
    QFileDialog x(this); \
    x.setFileMode(QFileDialog::ExistingFile); \
    x.setNameFilter(tr("Image files (*.png *.svg *.jpg *.bmp *.gif)"));

namespace Ui {
class AppearanceSettingsWidget;
}

class AppearanceSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AppearanceSettingsWidget(QWidget *parent = 0);
    ~AppearanceSettingsWidget();

public slots:
    void loadSettings();
    void storeSettings();

private slots:
    void on_qpbMainToolbarBackgroundFileChooser_clicked();

    void on_qpbWindowToolbarBackgroundFileChooser_clicked();

    void on_qleMainWindowBackgroundFileChooser_clicked();

private:
    Ui::AppearanceSettingsWidget *_ui;
};

#endif // APPEARANCESETTINGSWIDGET_H
