#include <QApplication>
#include <iostream>
#include <stdio.h>
#include <stdexcept>

#include "qtgui/mainwindow.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow *w = new MainWindow();
    w->show();
    a.exec();
    cout << "End" << endl;
    return 0;
}
