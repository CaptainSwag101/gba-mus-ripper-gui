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

    QStringList args;
    args.append(romPath);
    args.append("-o");
    args.append(outPath);

    if (parent->gmFlag)
        args.append("-gm");
    if (parent->xgFlag)
        args.append("-xg");
    if (parent->rcFlag)
        args.append("-rc");
    if (parent->sbFlag)
        args.append("-sb");
    if (parent->rawFlag)
        args.append("-raw");
    if (parent->adrFlag && !parent->address.isEmpty())
    {
        args.append("-adr");
        args.append(QString(parent->address));
    }

    ripper = new QProcess(this);
#ifdef _WIN32
    ripper->setProgram(QString(QDir::currentPath() + '\\' + "gba_mus_ripper.exe"));
#else
    ripper->setProgram(QString(QDir::currentPath() + '/' + "gba_mus_ripper"));
#endif
    ripper->setArguments(args);

    connect(ripper, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(Finish()));
    ripper->start();

}

void ProgressDialog::Finish()
{
    QMessageBox resultMsg;
    qDebug() << QString(ripper->readAll());
    ui->progressBar->setMaximum(100);
    if (ripper->exitCode() == 0)
    {
        ui->label->setText("Extraction completed!");
        ui->progressBar->setValue(100);
        QString romName = romPath.split(QDir::separator()).last();
        romName.truncate((romName.length() - 1) - romName.split('.').last().length());
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Extraction complete!", "Music was successfully extracted to \"" + outPath + QDir::separator() + romName + "\".\nDo you want to open the output directory now?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            QDesktopServices::openUrl(QUrl::fromLocalFile(outPath + QDir::separator() + romName));
        }
    }
    else
    {
        ui->label->setText("An error occurred while extracting!");
        ui->progressBar->setValue(0);
        resultMsg.setIcon(QMessageBox::Critical);

        switch (ripper->exitCode())
        {
        case -1:
            resultMsg.setText("Invalid arguments passed to gba_mus_ripper!");
            break;

        case -2:
            resultMsg.setText("Unable to open file \"" + romPath + "\" for reading!");
            break;

        case -3:
            resultMsg.setText("Invalid song table address specified.");
            break;

        case -4:
            resultMsg.setText("No sound engine was found. This ROM may not use the 'Sappy' sound engine.");
            break;

        case -5:
            resultMsg.setText("Invalid offset within GBA ROM.");
            break;

        case -6:
            resultMsg.setText("Song table address is outside the bounds of the ROM.");
            break;

        case -7:
            resultMsg.setText("Unable to parse song table.");
            break;
        }
        resultMsg.exec();
    }

    close();
}

ProgressDialog::~ProgressDialog()
{
    delete ui;
}
