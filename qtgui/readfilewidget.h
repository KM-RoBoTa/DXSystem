#ifndef READFILEWIDGET_H
#define READFILEWIDGET_H

#include <QWidget>
#include <queue>
#include <string>
#include <thread>

#include "dclass/dxmotorsystem.h"

using namespace std;

namespace Ui {
class ReadFileWidget;
}

class ReadFileWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ReadFileWidget(QWidget *parent = 0);
    ~ReadFileWidget();
    bool close();
    void portnumSetNLock(int portnum);

private slots:
    void on_browse_clicked();

    void on_delayBox_valueChanged(int arg1);

    void on_startButton_toggled(bool checked);

    void on_leftHeaderBox_clicked();

    void on_topHeaderBox_clicked();

private:
    Ui::ReadFileWidget *ui;
    int delayTime;
    string filename;
    bool runFlag;
    DXMotorSystem* dptr;
    queue<vector<int> > allDataInstr;
    std::thread readThread;
    void refreshTable();
};

#endif // READFILEWIDGET_H
