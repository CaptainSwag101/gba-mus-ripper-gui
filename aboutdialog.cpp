#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_okButton_clicked()
{
    close();
}
