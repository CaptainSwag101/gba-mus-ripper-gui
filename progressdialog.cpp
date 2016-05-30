#include "progressdialog.h"
#include "ui_progressdialog.h"

ProgressDialog::ProgressDialog(MainWindow *parent) : QDialog(parent), ui(new Ui::ProgressDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint & ~Qt::WindowCloseButtonHint);

    ui->progressBar->setMaximum(0);
    ui->progressBar->setMinimum(0);

    romPath = parent->romPath;
    outPath = parent->outputPath;

    nativeArgs += romPath;
    nativeArgs += "\n-o\n";
    nativeArgs += outPath;

    if (parent->gmFlag)
        nativeArgs += "\n-gm";
    if (parent->xgFlag)
        nativeArgs += "\n-xg";
    if (parent->rcFlag)
        nativeArgs += "\n-rc";
    if (parent->sbFlag)
        nativeArgs += "\n-sb";
    if (parent->rawFlag)
        nativeArgs += "\n-raw";
    if (parent->adrFlag && !parent->address.isEmpty())
        nativeArgs += "\n-adr\n" + parent->address;
}

void ProgressDialog::StartRip()
{
    QStringList argList = nativeArgs.split("\n");

    int i = 0;
    int size = argList.size();
    string c[size];
    foreach(QString s, argList)
    {
        c[i] = s.toStdString();
        i++;
    }

    /*
    ripper = new QProcess(this);
    ripper->setProgram(QString(QDir::toNativeSeparators(QDir::currentPath() + '/' + "gba_mus_ripper.exe")));
    ripper->setNativeArguments(nativeArgs);

    connect(ripper, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(Finish()));
    ripper->start();
    */

    Finish(mus_ripper(size, c));
    close();
}

void ProgressDialog::Finish(int exitCode)
{
    QMessageBox *resultMsg = new QMessageBox(this);

    ui->progressBar->setMaximum(100);
    if (exitCode == 0)
    {
        ui->label->setText("Extraction completed!");
        ui->progressBar->setValue(100);
        QString romName = romPath.split(QDir::separator()).last();
        romName.truncate((romName.length() - 1) - romName.split('.').last().length());
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Extraction complete!", "Music was successfully extracted to \"" + romName + "\".\nDo you want to open the output directory now?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            QDesktopServices::openUrl(QUrl::fromLocalFile(QDir::toNativeSeparators(romName)));
        }
    }
    else
    {
        ui->label->setText("An error occurred while extracting!");
        ui->progressBar->setValue(0);
        resultMsg->setIcon(QMessageBox::Critical);

        switch (exitCode)
        {
        case -1:
            resultMsg->setText("Invalid arguments passed to gba_mus_ripper!");
            break;

        case -2:
            resultMsg->setText("Unable to open file \"" + romPath + "\" for reading!");
            break;

        case -3:
            resultMsg->setText("Invalid song table address specified.");
            break;

        case -4:
            resultMsg->setText("No sound engine was found. This ROM may not use the 'Sappy' sound engine.");
            break;

        case -5:
            resultMsg->setText("Invalid offset within GBA ROM.");
            break;

        case -6:
            resultMsg->setText("Song table address is outside the bounds of the ROM.");
            break;

        case -7:
            resultMsg->setText("Unable to parse song table.");
            break;
        }
        resultMsg->exec();
    }

    close();
}

ProgressDialog::~ProgressDialog()
{
    delete ui;
}
