#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDir>
#include <QFileInfo>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QProcess>

#include <string>

using string = std::string;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


    QString romPath;
    QString outPath;

    bool gmFlag;
    bool xgFlag;
    bool rcFlag;
    bool sbFlag;
    bool rawFlag;
    bool addrFlag;
    int address;


private slots:
    void on_startButton_clicked();
    void on_chooseRomButton_clicked();
    void on_chooseOutputButton_clicked();
    void on_manualAddressEnable_stateChanged();
    void on_actionExit_triggered();
    void on_actionAbout_triggered();


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
