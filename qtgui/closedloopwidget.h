#ifndef CLOSEDLOOPWIDGET_H
#define CLOSEDLOOPWIDGET_H

#include <QWidget>
#include <string>
#include <thread>

namespace Ui {
class ClosedLoopWidget;
}

class ClosedLoopWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ClosedLoopWidget(QWidget *parent = 0);
    ~ClosedLoopWidget();
    bool close();

private slots:
    void on_browse_clicked();

    void on_delayBox_valueChanged(int arg1);

    void on_startButton_toggled(bool checked);

private:
    Ui::ClosedLoopWidget *ui;
    int delayTime;
    std::string filename;
    bool runFlag;
    std::thread loopThread;
};

#endif // CLOSEDLOOPWIDGET_H
