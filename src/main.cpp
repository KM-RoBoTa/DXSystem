#include <QApplication>
#include <iostream>
#include <stdio.h>
#include <stdexcept>

#include "dclass/dxmotorsystem.h"
#include "dclass/dmodeldetails.h"
#include "dclass/control_table_constants.h"
#include "dstopwatch/dstopwatch.h"
#include "dtools/additionaltools.h"
#include "dtools/bilateralcontrol.h"
#include "dtools/readfile.h"
#include "dtools/setup.h"
#include "dtools/threadedreadfile.h"

#include "characterizerw.h"
#include "manualinput.h"

#include "qtgui/mainwindow.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow *w = new MainWindow();
    w->show();
    a.exec();
//    characterizeFn();
    cout << "End" << endl;
    return 0;
}
