#include <QFileDialog>
#include <iostream>

#include "dtools/threadedreadfile.h"

#include "closedloopwidget.h"
#include "ui_closedloopwidget.h"

ClosedLoopWidget::ClosedLoopWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClosedLoopWidget)
{
    ui->setupUi(this);
    ui->filenameLine->setReadOnly(1);
    runFlag = 0;
    delayTime = 4;
    ui->delayBox->setValue(delayTime);
}

ClosedLoopWidget::~ClosedLoopWidget()
{
    delete ui;
}

bool ClosedLoopWidget::close()
{
    ui->startButton->setChecked(0);
    return QWidget::close();
}


void ClosedLoopWidget::on_browse_clicked()
{
    try{
        QString path = (QFileDialog::getOpenFileName(this,tr("Open File"),"",tr("CSV Files(*.csv)")));
        ui->filenameLine->setText(path);
        filename = path.toStdString();
    } catch (runtime_error e) {
        std::cout << e.what() << endl;
    }
}


void ClosedLoopWidget::on_startButton_toggled(bool checked)
{
    try {
        if (runFlag == checked)
        {
            ui->delayBox->setReadOnly(0);
            ui->numsysBox->setReadOnly(0);
            return;
        }
        runFlag = checked;
        if (runFlag)
        {
            ui->delayBox->setReadOnly(1);
            ui->numsysBox->setReadOnly(1);

            loopThread = std::thread(threadedReadFileSequenceSample,ui->numsysBox->value(),ref(filename),ref(runFlag),delayTime);
        }
        else
        {
            ui->delayBox->setReadOnly(0);
            ui->numsysBox->setReadOnly(0);
            loopThread.join();
        }
    } catch (runtime_error e) {
        std::cout << e.what() << endl;
        runFlag = 0;
        ui->startButton->setChecked(0);
    }
}
void ClosedLoopWidget::on_delayBox_valueChanged(int arg1)
{
    delayTime = arg1;
}
