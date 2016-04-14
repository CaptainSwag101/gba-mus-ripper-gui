#include "progressdialog.h"
#include "ui_progressdialog.h"

ProgressDialog::ProgressDialog(QWidget *parent, QString romPath, QString outPath) :
    QDialog(parent),
    ui(new Ui::ProgressDialog)
{
    ui->setupUi(this);
    ui->progressBar->setMaximum(0);
    ui->progressBar->setMinimum(0);

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
    ripper->setNativeArguments(nativeArgs);

    connect(ripper, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(Finish()));
    ripper->start();
}

void ProgressDialog::Finish()
{
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(100);
    ui->closeButton->setEnabled(true);
}

ProgressDialog::~ProgressDialog()
{
    delete ui;
}

void ProgressDialog::on_closeButton_clicked()
{
    close();
}
