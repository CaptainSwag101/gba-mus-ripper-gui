#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDesktopServices>
#include <QDialog>
#include <QDir>
#include <QMessageBox>
#include <QObject>
#include <QProcess>
#include <QUrl>

#include "mainwindow.h"

namespace Ui {
class ProgressDialog;
}

class ProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProgressDialog(MainWindow *parent);
    ~ProgressDialog();

public slots:
    void Finish();

private:
    Ui::ProgressDialog *ui;
    QProcess *ripper;
    QString romPath;
    QString outPath;
};

#endif // PROGRESSDIALOG_H
