#include "ripper_core.h"
#include <QDir>
#include <QFileInfo>
#include <QFileDialog>

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    bool arePathsValid();

    void on_startButton_clicked();

    void on_chooseRomButton_clicked();

    void on_chooseOutputButton_clicked();

    void on_manualAddressEnable_stateChanged();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
