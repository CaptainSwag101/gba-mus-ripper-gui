#include "mainwindow.h"
#include "aboutdialog.h"
#include "ui_mainwindow.h"
#include <QDesktopServices>
#include <QDir>
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags((this->windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowMaximizeButtonHint & ~Qt::WindowContextHelpButtonHint);
    setFixedSize(width(), height());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    if (!QFileInfo(QDir::currentPath() + '/' + "gba_mus_ripper").exists() && !QFileInfo(QDir::currentPath() + '/' + "gba_mus_ripper.exe").exists())
    {
        QMessageBox errorMsg(QMessageBox::Critical,
                           "Extraction error",
                           "Unable to find the \"gba_mus_ripper\" executable in this program's directory.\nUnable to extract music.",
                           QMessageBox::Ok,
                           this);
        errorMsg.exec();
        return;
    }
    if (ui->romPathEdit->text().isEmpty())
    {
        QMessageBox errorMsg(QMessageBox::Critical,
                           "Extraction error",
                           "No GBA ROM specified.",
                           QMessageBox::Ok,
                           this);

        errorMsg.exec();
        return;
    }
    if (ui->outputPathEdit->text().isEmpty())
    {
        QMessageBox errorMsg(QMessageBox::Critical,
                           "Extraction error",
                           "No output path specified.",
                           QMessageBox::Ok,
                           this);
        errorMsg.exec();
        return;
    }

    QProgressDialog progressDlg("Extracting, please wait...", QString(), 0, 0, this);
    progressDlg.setWindowModality(Qt::WindowModal);
    progressDlg.setWindowFlags(progressDlg.windowFlags() & ~Qt::WindowCloseButtonHint & ~Qt::WindowContextHelpButtonHint);

    QStringList args;
    args.append(ui->romPathEdit->text());
    args.append("-o");
    args.append(ui->outputPathEdit->text());

    if (ui->flagGM->isChecked())
        args.append("-gm");
    if (ui->flagXG->isChecked())
        args.append("-xg");
    if (ui->flagRC->isChecked())
        args.append("-rc");
    if (ui->flagSB->isChecked())
        args.append("-sb");
    if (ui->flagRaw->isChecked())
        args.append("-raw");
    if (ui->flagAddr->isChecked() && !ui->manualAddress->text().isEmpty())
    {
        args.append("-adr");
        args.append(ui->manualAddress->text());
    }

    QProcess ripper(this);
#ifdef _WIN32
    ripper.setProgram(QString(QDir::currentPath() + '\\' + "gba_mus_ripper.exe"));
#else
    ripper.setProgram(QString(QDir::currentPath() + '/' + "gba_mus_ripper"));
#endif
    ripper.setArguments(args);

    QObject::connect(&ripper, SIGNAL(finished(int, QProcess::ExitStatus)), &progressDlg, SLOT(reset()));
    ripper.start();
    progressDlg.exec();
    ripper.waitForFinished(-1);


    QMessageBox resultMsg;
    qDebug() << QString(ripper.readAll());
    progressDlg.setMaximum(100);
    if (ripper.exitCode() == 0)
    {
        progressDlg.setLabelText("Extraction completed!");
        progressDlg.setValue(100);
        QString romName = ui->romPathEdit->text().split(QDir::separator()).last();
        romName.truncate((romName.length() - 1) - romName.split('.').last().length());
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this,
                                      "Extraction complete!",
                                      "Music was successfully extracted to \"" + ui->outputPathEdit->text() + QDir::separator() + romName + "\".\nDo you want to open the output directory now?",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            QUrl folderUrl = QUrl::fromLocalFile(ui->outputPathEdit->text());
            QDesktopServices::openUrl(folderUrl);
        }
    }
    else
    {
        progressDlg.setLabelText("An error occurred while extracting!");
        progressDlg.setValue(0);
        resultMsg.setIcon(QMessageBox::Critical);

        switch (ripper.exitCode())
        {
        case -1:
            resultMsg.setText("Invalid arguments passed to gba_mus_ripper!");
            break;

        case -2:
            resultMsg.setText("Unable to open file \"" + ui->romPathEdit->text() + "\" for reading!");
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
}

void MainWindow::on_chooseRomButton_clicked()
{
    QFileDialog openRom;
    QString initialPath;
    QString result;

    if (!ui->romPathEdit->text().isEmpty() && QDir(ui->romPathEdit->text()).exists())
        initialPath = ui->romPathEdit->text();
    else if (!ui->outputPathEdit->text().isEmpty() && QDir(ui->outputPathEdit->text() + "/").exists())
        initialPath = ui->outputPathEdit->text();
    else
        initialPath = QDir::currentPath();

    result = QDir::toNativeSeparators(openRom.getOpenFileName(this, tr("Choose a GBA ROM"), initialPath, "GBA ROMs (*.gba)"));

    if (!result.isEmpty())
    {
        ui->romPathEdit->setText(result);

        if (ui->outputPathEdit->text().isEmpty())
        {
            QString outPath = QFileInfo(result).absoluteFilePath();
            outPath.truncate(outPath.length() - (QFileInfo(result).suffix().length() + 1));
            ui->outputPathEdit->setText(QDir::toNativeSeparators(outPath));
            ui->startButton->setFocus();
        }
    }
}

void MainWindow::on_chooseOutputButton_clicked()
{
    QFileDialog openOutput;
    QString initialPath;
    QString result;

    if (!ui->outputPathEdit->text().isEmpty() && QDir(ui->outputPathEdit->text()).exists())
        initialPath = ui->outputPathEdit->text();
    else if (!ui->romPathEdit->text().isEmpty() && QDir(ui->romPathEdit->text()).exists())
        initialPath = ui->romPathEdit->text();
    else
        initialPath = QDir::currentPath();

    result = QDir::toNativeSeparators(openOutput.getExistingDirectory(this, tr("Choose output directory"), initialPath, QFileDialog::ShowDirsOnly));

    if (!result.isEmpty())
    {
        ui->outputPathEdit->setText(result);
    }
}

void MainWindow::on_manualAddressEnable_stateChanged()
{
    ui->manualAddress->setEnabled(ui->flagAddr->isChecked());
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog about;
    about.exec();
}
