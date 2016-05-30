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
#include "gba_mus_ripper/gba_mus_ripper.h"

namespace Ui {
class ProgressDialog;
}

class ProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProgressDialog(MainWindow *parent);
    ~ProgressDialog();
    void StartRip();

public slots:
    void Finish(int exitCode);

private:
    Ui::ProgressDialog *ui;
    QProcess *ripper;
    QString romPath;
    QString outPath;
    QString nativeArgs;
};

#endif // PROGRESSDIALOG_H
