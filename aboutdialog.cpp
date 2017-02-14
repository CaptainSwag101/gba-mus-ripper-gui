#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QString timestamp = QString::fromStdString((std::string)BUILD_DATE + " " + (std::string)BUILD_TIME);
    QDateTime buildDate = QDateTime::fromString(timestamp, "MMM d yyyy HH:mm:ss");
    ui->label->setText(ui->label->text() + buildDate.toString(Qt::SystemLocaleLongDate) + ")");
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_okButton_clicked()
{
    close();
}
