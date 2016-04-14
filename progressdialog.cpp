#include "progressdialog.h"
#include "ui_progressdialog.h"

ProgressDialog::ProgressDialog(QWidget *parent, QString rom, QString out) :
    QDialog(parent),
    ui(new Ui::ProgressDialog)
{
    ui->setupUi(this);
    ui->progressBar->setMaximum(0);
    ui->progressBar->setMinimum(0);

    romPath = rom;
    outPath = out;

    //std::string argv[] {(QDir::toNativeSeparators(QDir::currentPath() + '\\' + "gba_mus_ripper.exe")).toStdString(), (ui->romPathEdit->text()).toStdString(), "-o ", ui->outputPathEdit->text().toStdString()};
    QString nativeArgs;
    //nativeArgs += QString(QDir::toNativeSeparators(QDir::currentPath() + '\\' + "gba_mus_ripper.exe"));
    //nativeArgs += " \"";
    nativeArgs += '"';
    nativeArgs += romPath;
    nativeArgs += '"';
    nativeArgs += " -o ";
    nativeArgs += '"';
    nativeArgs += outPath;
    nativeArgs += '"';
    //puts(QString(QDir::toNativeSeparators(QDir::currentPath() + '\\' + "gba_mus_ripper.exe ")).toStdString().c_str());
    //puts(nativeArgs.toStdString().c_str());

    QProcess * ripper = new QProcess(this);
    ripper->setProgram(QString(QDir::toNativeSeparators(QDir::currentPath() + '\\' + "gba_mus_ripper.exe")));
    ripper->setArguments(nativeArgs.split(" "));

    connect(ripper, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(Finish()));
    ripper->start();
}

void ProgressDialog::Finish()
{
    QMessageBox * resultMsg = new QMessageBox(this);

    ui->progressBar->setMaximum(100);
    if (ripper->exitCode() == 0)
    {
        ui->label->setText("Extraction completed!");
        ui->progressBar->setValue(100);
        resultMsg->setIcon(QMessageBox::Information);
        resultMsg->setText("Music was successfully extracted to " + outPath);
    }
    else
    {
        ui->label->setText("An error occurred while extracting!");
        ui->progressBar->setValue(0);
        resultMsg->setIcon(QMessageBox::Critical);

        switch (ripper->exitCode())
        {
        case -1:
            resultMsg->setText("Invalid arguments passed to gba_mus_ripper!");
            break;

        case -2:
            resultMsg->setText("Unable to open file " + romPath + " for reading!");
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
    }

    resultMsg->exec();
    close();
}

ProgressDialog::~ProgressDialog()
{
    delete ui;
}
