#include <iostream>

#include "dtools/additionaltools.h"
#include "dtools/setup.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

/**
 * @brief MainWindow
 * Governs the GUI main window
 */

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    bwindow = new BilateralControlWidget();
    cwindow = new ClosedLoopWidget();
    rwindow = new ReadFileWidget();
    mrwindow = new MultiReadWidget();

}

MainWindow::~MainWindow()
{
    delete bwindow;
    delete cwindow;
    delete rwindow;
    delete mrwindow;
    delete ui;
}

void MainWindow::on_exitButton_clicked()
{
//    ui->readButton->setChecked(0);
//    ui->bisystemButton->setChecked(0);
//    ui->closedloopButton->setChecked(0);
    delete bwindow;
    delete cwindow;
    delete rwindow;
    delete mrwindow;
    cout << endl;
    cout << "Closing all windows..." << endl;
    cout << "-----" << endl << endl;
    bwindow = new BilateralControlWidget();
    cwindow = new ClosedLoopWidget();
    rwindow = new ReadFileWidget();
    mrwindow = new MultiReadWidget();
}

void MainWindow::on_bisystemButton_toggled(bool checked)
{
    if(checked)
    {
        bwindow->show();
    } else {
        bwindow->close();
    }
}
void MainWindow::on_readButton_toggled(bool checked)
{
    if(checked)
    {
        mrwindow->show();
    } else {
        mrwindow->close();
    }
}

void MainWindow::on_closedloopButton_toggled(bool checked)
{
    if(checked)
    {
        cwindow->show();
    } else {
        cwindow->close();
    }
}

void MainWindow::on_setupButton_toggled(bool checked)
{
    if(checked)
    {
        dxlSetupSequence();
        ui->exitButton->setChecked(1);
    }
}
