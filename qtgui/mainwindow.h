#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#include "qtgui/bilateralcontrolwidget.h"
#include "qtgui/closedloopwidget.h"
#include "qtgui/readfilewidget.h"
#include "qtgui/multireadwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_exitButton_clicked();
    void on_bisystemButton_toggled(bool checked);
    void on_readButton_toggled(bool checked);
    void on_closedloopButton_toggled(bool checked);
    void on_setupButton_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    ReadFileWidget *rwindow;
    BilateralControlWidget *bwindow;
    ClosedLoopWidget *cwindow;
    MultiReadWidget *mrwindow;
};

#endif // MAINWINDOW_H
