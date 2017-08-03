#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <string>
#include <QDateTime>
#include <QDialog>
#include <QString>
#include <QTimeZone>

#define BUILD_DATE __DATE__
#define BUILD_TIME __TIME__

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = 0);
    ~AboutDialog();

private slots:
    void on_okButton_clicked();

private:
    Ui::AboutDialog *ui;

};

#endif // ABOUTDIALOG_H
