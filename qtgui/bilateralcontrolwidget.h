#ifndef BILATERALCONTROLWIDGET_H
#define BILATERALCONTROLWIDGET_H

#include <QWidget>
#include <string>
#include <thread>
#include <vector>

#include "dclass/dxmotorsystem.h"

namespace Ui {
class BilateralControlWidget;
}

class BilateralControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BilateralControlWidget(QWidget *parent = 0);
    ~BilateralControlWidget();
    bool close();

private slots:
    void on_filePrefixEdit_textChanged(const QString &arg1);

    void on_startButton_toggled(bool checked);

    void on_numSlaveBox_valueChanged(int arg1);

private:
    Ui::BilateralControlWidget *ui;
    std::string filePrefix;
    bool runFlag;
    int torqueSetting;
    std::vector<DXMotorSystem*> allSys;
    std::thread bisystemThread;
};

#endif // BILATERALCONTROLWIDGET_H
