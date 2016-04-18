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
    if (!QFileInfo(QDir::currentPath() + '\\' + "gba_mus_ripper.exe").exists())
    {
        QMessageBox *errorMsg = new QMessageBox(this);
        errorMsg->setIcon(QMessageBox::Critical);
        errorMsg->setText("Unable to find the \"gba_mus_ripper.exe\" executable in this program's directory.\n"
                          "Unable to extract music.");
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

        ProgressDialog * progDialog = new ProgressDialog(this);
        progDialog->exec();
    }
}

void MainWindow::on_chooseRomButton_clicked()
{
    QFileDialog * openRom;
    if (!ui->romPathEdit->text().isEmpty() && QDir(QDir::toNativeSeparators(ui->romPathEdit->text())).exists())
        ui->romPathEdit->setText(QDir::toNativeSeparators(openRom->getOpenFileName(this, tr("Choose a GBA ROM"), ui->romPathEdit->text(), "GBA ROMs (*.gba)")));
    else if (!ui->outputPathEdit->text().isEmpty() && QDir(QDir::toNativeSeparators(ui->outputPathEdit->text())).exists())
        ui->romPathEdit->setText(QDir::toNativeSeparators(openRom->getOpenFileName(this, tr("Choose a GBA ROM"), ui->outputPathEdit->text(), "GBA ROMs (*.gba)")));
    else
        ui->romPathEdit->setText(QDir::toNativeSeparators(openRom->getOpenFileName(this, tr("Choose a GBA ROM"), QDir::currentPath(), "GBA ROMs (*.gba)")));

    if (ui->outputPathEdit->text().isEmpty())
        ui->outputPathEdit->setText(QDir::toNativeSeparators(QFileInfo(ui->romPathEdit->text()).absolutePath()));
}

void MainWindow::on_chooseOutputButton_clicked()
{
    QFileDialog * openOutput;
    if (!ui->outputPathEdit->text().isEmpty() && QDir(QDir::toNativeSeparators(ui->outputPathEdit->text())).exists())
        ui->outputPathEdit->setText(QDir::toNativeSeparators(openOutput->getExistingDirectory(this, tr("Choose output directory"), ui->outputPathEdit->text(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks)));
    else if (!ui->romPathEdit->text().isEmpty() && QDir(QDir::toNativeSeparators(ui->romPathEdit->text())).exists())
        ui->outputPathEdit->setText(QDir::toNativeSeparators(openOutput->getExistingDirectory(this, tr("Choose output directory"), ui->romPathEdit->text(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks)));
    else
        ui->outputPathEdit->setText(QDir::toNativeSeparators(openOutput->getExistingDirectory(this, tr("Choose output directory"), QDir::currentPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks)));
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
    // open 'About' window
    AboutDialog *about = new AboutDialog(this);
    about->exec();
}
