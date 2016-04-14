#include "mainwindow.h"
#include "progressdialog.h"
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
    ProgressDialog * progDialog = new ProgressDialog(this, ui->romPathEdit->text(), ui->outputPathEdit->text());
    progDialog->exec();
}

void MainWindow::on_chooseRomButton_clicked()
{
    QFileDialog * openRom;
    if (!ui->romPathEdit->text().isEmpty() && QDir(QDir::toNativeSeparators(ui->romPathEdit->text())).exists())
        ui->romPathEdit->setText(QDir::toNativeSeparators(openRom->getOpenFileName(this, tr("Choose a GBA ROM"), ui->romPathEdit->text(), "GBA ROMs (*.gba)")));
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
}
