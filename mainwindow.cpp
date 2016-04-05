#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gba_mus_ripper.h"
#include <QDir>
#include <QFileDialog>

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

void MainWindow::on_startButton_clicked()
{
    std::string argv[] {(QDir::toNativeSeparators(QDir::currentPath() + "\\" + "gba_mus_ripper.exe")).toStdString(), (ui->romPathEdit->text()).toStdString()};
    musRip(2, argv);
}

void MainWindow::on_chooseRomButton_clicked()
{
    if (!ui->romPathEdit->text().isEmpty() && QDir(QDir::toNativeSeparators(ui->romPathEdit->text())).exists())
        ui->romPathEdit->setText(QDir::toNativeSeparators(QFileDialog::getOpenFileName(this, tr("Choose output directory"), ui->romPathEdit->text(), "*.gba")));
    else
        ui->romPathEdit->setText(QDir::toNativeSeparators(QFileDialog::getOpenFileName(this, tr("Choose output directory"), QDir::currentPath(), "*.gba")));

    //if (ui->outputPathEdit->text().isEmpty())
        //ui->outputPathEdit->setText(ui->romPathEdit->text());
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
