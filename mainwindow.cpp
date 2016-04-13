#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::arePathsValid()
{
    return (!ui->outputPathEdit->text().isEmpty() && !ui->romPathEdit->text().isEmpty() && QDir(QDir::toNativeSeparators(ui->outputPathEdit->text())).exists() && QDir(QDir::toNativeSeparators(ui->romPathEdit->text())).exists());
}

void MainWindow::on_startButton_clicked()
{
    if (arePathsValid())
    {
        if (startRip(ui->romPathEdit->text(), ui->outputPathEdit->text(), false, false, false, false, false, -1) != 0)
        {

        }
    }
}

void MainWindow::on_chooseRomButton_clicked()
{
    if (!ui->romPathEdit->text().isEmpty() && QDir(QDir::toNativeSeparators(ui->romPathEdit->text())).exists())
        ui->romPathEdit->setText(QDir::toNativeSeparators(QFileDialog::getOpenFileName(this, tr("Choose output directory"), ui->romPathEdit->text(), "*.gba")));
    else
        ui->romPathEdit->setText(QDir::toNativeSeparators(QFileDialog::getOpenFileName(this, tr("Choose output directory"), QDir::currentPath(), "*.gba")));

    if (ui->outputPathEdit->text().isEmpty())
        ui->outputPathEdit->setText(QDir::toNativeSeparators(QFileInfo(ui->romPathEdit->text()).absolutePath()));
}

void MainWindow::on_chooseOutputButton_clicked()
{
    if (!ui->outputPathEdit->text().isEmpty() && QDir(QDir::toNativeSeparators(ui->outputPathEdit->text())).exists())
        ui->outputPathEdit->setText(QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, tr("Choose output directory"), ui->outputPathEdit->text(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks)));
    else if (!ui->romPathEdit->text().isEmpty() && QDir(QDir::toNativeSeparators(ui->romPathEdit->text())).exists())
        ui->outputPathEdit->setText(QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, tr("Choose output directory"), ui->romPathEdit->text(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks)));
    else
        ui->outputPathEdit->setText(QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, tr("Choose output directory"), QDir::currentPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks)));
}

void MainWindow::on_manualAddressEnable_stateChanged()
{
    ui->manualAddress->setEnabled(ui->manualAddressEnable->isChecked());
}
