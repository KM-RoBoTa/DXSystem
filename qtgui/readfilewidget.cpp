#include <QFileDialog>
#include <iostream>

#include "dtools/readfile.h"
#include "dtools/setup.h"

#include "readfilewidget.h"
#include "ui_readfilewidget.h"

/**
 * @brief ReadFileWidget
 * Governs the GUI for file reading
 */

ReadFileWidget::ReadFileWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReadFileWidget)
{
    ui->setupUi(this);
    ui->filenameLine->setReadOnly(1);
    runFlag = 0;
    delayTime = 4;
    ui->delayBox->setValue(delayTime);
    ui->positionButton->setChecked(1);
    filename = "";
}

ReadFileWidget::~ReadFileWidget()
{
    delete ui;
}

bool ReadFileWidget::close()
{
    ui->startButton->setChecked(0);
    return QWidget::close();
}

void ReadFileWidget::on_browse_clicked()
{
    try{
        QString path = (QFileDialog::getOpenFileName(this,tr("Open File"),"",tr("CSV Files(*.csv)")));
        ui->filenameLine->setText(path);
        filename = path.toStdString();
        refreshTable();

    } catch (runtime_error e) {
        std::cout << e.what() << endl;
    }
}

void ReadFileWidget::on_leftHeaderBox_clicked()
{
    refreshTable();
}

void ReadFileWidget::on_topHeaderBox_clicked()
{
    refreshTable();
}


void ReadFileWidget::refreshTable()
{
    if (filename == "")
        return;
    queue<vector<double> > allInstr;
    if(getInstructions(allInstr,filename, ui->leftHeaderBox->isChecked(), ui->topHeaderBox->isChecked()))
        throw(runtime_error("Unable to open file"));
    if (allInstr.empty())
    {
        std::cout << "CSV File is in invalid format." << endl;
        return;
    }
    int numRows = allInstr.size();
    ui->dataTable->setRowCount(numRows);
    ui->dataTable->setColumnCount(allInstr.front().size());
    QStringList topHeaders;
    for (unsigned int nn = 0; nn < allInstr.front().size(); nn++)
    {
        QString hdr = "Motor #" + QString::number(nn);
        topHeaders << hdr;
    }
    ui->dataTable->setHorizontalHeaderLabels(topHeaders);
    for (int ii = 0; ii < numRows; ii++)
    {
        vector<double> data = allInstr.front();
        allInstr.pop();
        allInstr.push(data);
        for (unsigned int jj = 0; jj < data.size(); jj++)
        {
            ui->dataTable->setItem(ii,jj,new QTableWidgetItem(QString::number(data[jj])));
        }
    }
}

void ReadFileWidget::on_delayBox_valueChanged(int arg1)
{
    delayTime = arg1;
}

void ReadFileWidget::on_startButton_toggled(bool checked)
{
    try {
        if (runFlag == checked)
        {
            ui->delayBox->setReadOnly(0);
            ui->portnumBox->setReadOnly(0);
            return;
        }
        runFlag = checked;
        if (runFlag)
        {
            ui->delayBox->setReadOnly(1);
            dptr = dxsystemSetup(ui->portnumBox->value());
            ui->portnumBox->setReadOnly(1);

            string readFilename;
            bool hasLeftHeader = ui->leftHeaderBox->isChecked();
            bool hasTopHeader = ui->topHeaderBox->isChecked();

            if (ui->angleButton->isChecked() || ui->radButton->isChecked())
            {
                readFilename = parseAngles2File(*dptr,filename, hasLeftHeader, hasTopHeader, ui->radButton->isChecked());
                hasLeftHeader = true;
                hasTopHeader = true;
            }

            if (ui->positionButton->isChecked())
                readFilename = filename;


            if (getInstructions(allDataInstr,readFilename, hasLeftHeader, hasTopHeader))
                throw(runtime_error("Cannot read file"));

            if (allDataInstr.empty())
                throw(runtime_error("No data provided"));

            if (allDataInstr.front().size() != dptr->getNumMotors())
                throw(runtime_error("Columns in data file do not match motor count"));

            readThread = std::thread(loopReading, ref(*dptr), ref(allDataInstr), ref(runFlag), delayTime, ui->loopBox->isChecked());
        }
        else
        {
            ui->delayBox->setReadOnly(0);
            ui->portnumBox->setReadOnly(0);
            readThread.join();
            while(!allDataInstr.empty())
                allDataInstr.pop();
            delete dptr;
        }
    } catch (runtime_error e) {
        std::cout << e.what() << endl;
        runFlag = 0;
        ui->startButton->setChecked(0);
    }
}

void ReadFileWidget::portnumSetNLock(int portnum)
{
    ui->portnumBox->setValue(portnum);
    ui->portnumBox->setReadOnly(1);
}
