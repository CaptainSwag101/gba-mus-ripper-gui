#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>
#include  <QMessageBox>
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
    explicit ProgressDialog(QWidget *parent, QString rom = "", QString out = "");
    ~ProgressDialog();

public slots:
    void Finish();

private:
    Ui::ProgressDialog *ui;
    QProcess * ripper;
    QString romPath;
    QString outPath;
};

#endif // PROGRESSDIALOG_H
