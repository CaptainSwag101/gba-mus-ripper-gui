#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>
#include <QProcess>
#include <QObject>
#include <QDir>

namespace Ui {
class ProgressDialog;
}

class ProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProgressDialog(QWidget *parent, QString romPath = "", QString outPath = "");
    ~ProgressDialog();

public slots:
    void Finish();

private slots:
    void on_closeButton_clicked();

private:
    Ui::ProgressDialog *ui;
    QProcess * ripper;
};

#endif // PROGRESSDIALOG_H
