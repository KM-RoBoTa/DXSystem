#include "multireadwidget.h"
#include "ui_multireadwidget.h"

MultiReadWidget::MultiReadWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MultiReadWidget)
{
    ui->setupUi(this);
    ui->tab0->portnumSetNLock(0);
    ui->tab1->portnumSetNLock(1);
    ui->tab2->portnumSetNLock(2);
    ui->tab3->portnumSetNLock(3);
    ui->tab4->portnumSetNLock(4);
}

MultiReadWidget::~MultiReadWidget()
{
    ui->tab0->close();
    ui->tab1->close();
    ui->tab2->close();
    ui->tab3->close();
    ui->tab4->close();
    ui->tab->close();
    ui->tab0->~ReadFileWidget();
    ui->tab1->~ReadFileWidget();
    ui->tab2->~ReadFileWidget();
    ui->tab3->~ReadFileWidget();
    ui->tab4->~ReadFileWidget();
    ui->tab->~ReadFileWidget();
    delete ui;
}
