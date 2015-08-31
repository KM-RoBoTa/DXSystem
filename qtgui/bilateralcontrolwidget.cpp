#include <iostream>
#include "dtools/bilateralcontrol.h"
#include "dtools/setup.h"

#include "bilateralcontrolwidget.h"
#include "ui_bilateralcontrolwidget.h"

/**
 * @brief BilateralControlWidget
 * Governs the GUI for bilateral control
 */

BilateralControlWidget::BilateralControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BilateralControlWidget)
{
    ui->setupUi(this);
    ui->numSlaveBox->setValue(1);
    on_numSlaveBox_valueChanged(1);
    runFlag = 0;
    torqueSetting = 0;
    filePrefix = ui->filePrefixEdit->text().toStdString();
    ui->filePrefixEdit->setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9_]*"),this));
}

BilateralControlWidget::~BilateralControlWidget()
{
    delete ui;
}

bool BilateralControlWidget::close()
{
    ui->startButton->setChecked(0);
    return QWidget::close();
}

void BilateralControlWidget::on_filePrefixEdit_textChanged(const QString &arg1)
{
    filePrefix = arg1.toStdString();
}

void BilateralControlWidget::on_startButton_toggled(bool checked)
{
    try {
        if (runFlag == checked)
        {
            ui->filePrefixEdit->setReadOnly(0);
            ui->numSlaveBox->setReadOnly(0);
            ui->portnumTable->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed|QAbstractItemView::AnyKeyPressed);
            return;
        }
        runFlag = checked;
        if (runFlag)
        {
            runFlag = 1;
            int numSlave = ui->numSlaveBox->value();
            ui->filePrefixEdit->setReadOnly(1);
            ui->numSlaveBox->setReadOnly(1);
            ui->portnumTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
            std::vector<int> allPortNum;
            for (int ii = 0; ii < numSlave+1; ii++)
            {
                allPortNum.push_back(ui->portnumTable->item(0,ii)->text().toInt());
            }

            allSys = multidxsystemSetup(allPortNum);
            bisystemThread = std::thread(bisystemControl, ref(allSys), ref(runFlag), ref(torqueSetting), ref(filePrefix));
        }
        else
        {
            runFlag = 0;
            bisystemThread.join();
            ui->filePrefixEdit->setReadOnly(0);
            ui->portnumTable->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed|QAbstractItemView::AnyKeyPressed);
            ui->numSlaveBox->setReadOnly(0);
        }
    } catch(runtime_error e) {
        std::cout << e.what() << endl;
        runFlag = 0;
        ui->startButton->setChecked(0);
    }
}

void BilateralControlWidget::on_numSlaveBox_valueChanged(int arg1)
{
    for (int ii = 1; ii <= ui->numSlaveBox->maximum(); ii++)
        ui->portnumTable->setColumnHidden(ii, ii > arg1);
}
