#include "mainwindow.h"
#include "progressdialog.h"
#include "aboutdialog.h"
#include "ui_mainwindow.h"
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
    setWindowFlags((windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowMaximizeButtonHint);
    setFixedSize(width(), height());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    if (!QFileInfo(QDir::currentPath() + '/' + "gba_mus_ripper.exe").exists())
    {
        QMessageBox *errorMsg = new QMessageBox(this);
        errorMsg->setIcon(QMessageBox::Critical);
        errorMsg->setText("Unable to find the \"gba_mus_ripper.exe\" executable in this program's directory.\n"
                          "Unable to extract music.");
        errorMsg->exec();
    }
    else if (ui->romPathEdit->text().isEmpty())
    {
        QMessageBox *errorMsg = new QMessageBox(this);
        errorMsg->setIcon(QMessageBox::Critical);
        if (ui->outputPathEdit->text().isEmpty())
            errorMsg->setText("No GBA ROM or output path specified.");
        else
            errorMsg->setText("No GBA ROM specified.");

        errorMsg->exec();
    }
    else if (ui->outputPathEdit->text().isEmpty())
    {
        QMessageBox *errorMsg = new QMessageBox(this);
        errorMsg->setIcon(QMessageBox::Critical);
        errorMsg->setText("No output path specified.");
        errorMsg->exec();
    }
    else
    {
        romPath = ui->romPathEdit->text();
        outputPath = ui->outputPathEdit->text();
        gmFlag = ui->giveGMNames->isChecked();
        xgFlag = ui->makeXGCompliant->isChecked();
        rcFlag = ui->avoidCh10->isChecked();
        sbFlag = ui->splitSoundBanks->isChecked();
        rawFlag = ui->outputRaw->isChecked();
        adrFlag = ui->manualAddressEnable->isChecked();
        address = ui->manualAddress->text();

        ProgressDialog *progDialog = new ProgressDialog(this);
        progDialog->exec();
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

    result = openRom.getOpenFileName(this, tr("Choose a GBA ROM"), initialPath, "GBA ROMs (*.gba)");

    if (!result.isEmpty())
    {
        ui->romPathEdit->setText(result);

        if (ui->outputPathEdit->text().isEmpty())
            ui->outputPathEdit->setText(QFileInfo(result).absolutePath());
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

    result = openOutput.getExistingDirectory(this, tr("Choose output directory"), initialPath, QFileDialog::ShowDirsOnly);

    if (!result.isEmpty())
    {
        ui->outputPathEdit->setText(result);
    }
}

void MainWindow::on_manualAddressEnable_stateChanged()
{
    ui->manualAddress->setEnabled(ui->manualAddressEnable->isChecked());
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog *about = new AboutDialog(this);
    about->exec();
}
