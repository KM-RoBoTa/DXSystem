#include <iostream>

#include "dclass/control_table_constants.h"
#include "dclass/dxmotorsystem.h"
#include "dstopwatch/dstopwatch.h"
#include "dtools/additionaltools.h"
#include "dtools/setup.h"

#include "characterizerw.h"

using namespace std;
void printAll(const vector<int>& allData, const string& param);

void characterizeFn()
{
    string outputFile = "log" + getTimeStr() + ".txt";
    freopen(outputFile.c_str(), "a", stdout);

    DXMotorSystem d = *dxsystemSetup(0);


    cout << "Models:" << endl;
    for (unsigned int ii = 0; ii < d.getNumMotors(); ii++)
    {
        cout << d.getModel(ii) << "     ";
    }
    cout << endl;

    vector<long long> readbyteTime, readwordTime, readword2Time, writebyteTime, writewordTime, rwbyteTime, rwwordTime, rwmultiTime;

    DStopWatch watch;
    long long sumreadbyteTime, sumreadwordTime, sumreadword2Time, sumwritebyteTime, sumwritewordTime, sumrwbyteTime, sumrwwordTime, sumrwmultiTime;


    vector<vector<int>> posHistory;
    sumreadwordTime = 0;
    for (int ii = 0; ii < 1000; ii++)
    {
        watch.start();
        vector<int> allPos = d.getAllPosition();
        long long timeE = watch.stop();
        printAll(allPos,"Position");
        posHistory.push_back(allPos);
        sumreadwordTime += timeE;
        readwordTime.push_back(timeE);
    }

    sumreadword2Time = 0;
    for (int ii = 0; ii < 1000; ii++)
    {
        watch.start();
        vector<int> allPos;
        for (unsigned int jj = 0; jj < d.getNumMotors(); jj++)
        {
            allPos.push_back(d.getPosition(jj));
        }
        long long timeE = watch.stop();
        printAll(allPos,"Position (Manual)");
        sumreadword2Time += timeE;
        readword2Time.push_back(timeE);
    }

    sumreadbyteTime = 0;
    for (int ii = 0; ii < 1000; ii++)
    {
        watch.start();
        vector<int> allTemp = d.readAllByte(P_PRESENT_TEMPERATURE);
        long long timeE = watch.stop();
        printAll(allTemp,"Temperature");
        sumreadbyteTime += timeE;
        readbyteTime.push_back(timeE);
    }

    sumwritewordTime = 0;
    for (int ii = 0; ii < 1000; ii++)
    {
        watch.start();
        d.setAllPosition(posHistory[ii]);
        long long timeE = watch.stop();
        sumwritewordTime += timeE;
        writewordTime.push_back(timeE);
    }

    sumwritebyteTime = 0;
    for (int ii = 0; ii < 1000; ii++)
    {
        bool setting = ii%6 == 0 || ii%6 == 1 || ii%6 == 4;
        watch.start();
        d.setAllTorqueEn(setting);
        long long timeE = watch.stop();
        sumwritebyteTime += timeE;
        writebyteTime.push_back(timeE);
    }

    sumrwbyteTime = 0;
    for (int ii = 0; ii < 1000; ii++)
    {
        bool setting = ii%6 == 0 || ii%6 == 1 || ii%6 == 4;
        watch.start();
        d.setAllTorqueEn(setting);
        vector<int> allTorqueEn = d.readAllByte(P_TORQUE_ENABLE);
        long long timeE = watch.stop();
        printAll(allTorqueEn,"Torque Enable");
        sumrwbyteTime += timeE;
        rwbyteTime.push_back(timeE);
    }

    sumrwwordTime = 0;
    for (int ii = 0; ii < 100; ii++)
    {
        watch.start();
        d.setAllPosition(posHistory[ii]);
        vector<int> allPos = d.getAllPosition();
        long long timeE = watch.stop();
        printAll(allPos,"Position (loop)");
        sumrwwordTime += timeE;
        rwwordTime.push_back(timeE);
    }

    sumrwmultiTime = 0;
    for (int ii = 0; ii < 100; ii++)
    {
        watch.start();
        d.setAllPosition(posHistory[ii]);
        vector<int> allPos = d.getAllPosition();
        vector<int> allTemp = d.readAllByte(P_PRESENT_TEMPERATURE);
        vector<int> allLoad = d.getAllLoad();
        vector<int> allSpeed = d.getAllCurrentSpeed();
        vector<int> allGoalPos = d.readAllWord(P_GOAL_POSITION_L);
        long long timeE = watch.stop();
        printAll(allTemp,"Temperature (multi)");
        printAll(allSpeed,"Speed (multi)");
        printAll(allLoad,"Load (multi)");
        printAll(allPos,"Position (multi)");
        printAll(allGoalPos,"Goal Position (multi)");
        sumrwmultiTime += timeE;
        rwmultiTime.push_back(timeE);
    }

    cout << "Time to read position: " << (double)sumreadwordTime/readwordTime.size() << endl;
    cout << "Time to read position (loop): " << (double)sumreadword2Time/readword2Time.size() << endl;
    cout << "Time to read temperature: " << (double)sumreadbyteTime/readbyteTime.size() << endl;
    cout << "Time to write position: " << (double)sumwritewordTime/writewordTime.size() << endl;
    cout << "Time to write torque enable: " << (double)sumwritebyteTime/writebyteTime.size() << endl;
    cout << "Time to read/write torque enable: " << (double)sumrwbyteTime/rwbyteTime.size() << endl;
    cout << "Time to read/write position: " << (double)sumrwwordTime/rwwordTime.size() << endl;
    cout << "Time to multiple instr: " << (double)sumrwmultiTime/rwmultiTime.size() << endl;
    cout << "End of Characterization" << endl;
}

void printAll(const vector<int>& allData, const string& param)
{
    cout << endl;
    cout << param << endl;
    for (unsigned int ii = 0; ii < allData.size(); ii++)
    {
        cout << allData[ii] << "     ";
    }
    cout << endl;
}
