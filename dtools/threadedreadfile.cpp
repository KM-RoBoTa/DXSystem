#include <iostream>
#include <stdio.h>
#include <thread>

#include "additionaltools.h"
#include "readfile.h"
#include "setup.h"
#include "src/user_settings.h"
#include "src/userinputfn.h"

#include "threadedreadfile.h"

int multiDevFileThreading(vector<DXMotorSystem*>& allSys, const string& filename, bool& runFlag, int delayTime);
void multiDevFileCalc(vector<DXMotorSystem*>& allSys, int timestep, vector<vector<int> >& allPrevPos, vector<vector<int> >& allPrevFilePos, vector<vector<int> >& allFilePos, vector<vector<int> >& allCalcPos, vector<WriteData>& allCalcData);
void multiDevFileSet(DXMotorSystem& d, vector<int>& goalPos, WriteData& goalData, vector<int>& currentPos);

/**
 * @brief threadedReadFileSequenceSample
 * Sample sequence that reads position data files, transmits information via threads and provides for closed feedback loop
 * (Behavior of closed feedback loop governed by calcNewPosFromFile in "src/userinputfn.cpp")
 * @param numPorts Number of ports used/DXMotorSystems connected
 * @param filename File name of the csv file containing position data values
 * @param runFlag Reference to boolean flag, loops and runs the function while true
 * @param delayTime Delay at each cycle - optional, default to 4
 */
void threadedReadFileSequenceSample(int numPorts, const string& filename, bool& runFlag, int delayTime)
{
    vector<DXMotorSystem*> allSys = multidxsystemSetup(numPorts);
    multiDevFileThreading(allSys, filename, runFlag, delayTime);

    for (unsigned int ii = 0; ii < allSys.size(); ii++)
    {
        delete(allSys[ii]);
    }
}


// Main logic of the threading, file reading and calculation;
// adopts a one-ahead technique where parameters are calculated one time step ahead, before transmitted in the next time step
// calculations are based on information from the file and from information one time step before
int multiDevFileThreading(vector<DXMotorSystem*>& allSys, const string& filename, bool& runFlag, int delayTime)
{
    int numDXSystem = allSys.size();

    queue<vector<int> > allPosInstr;
    getInstructions(allPosInstr, filename);
    vector<vector<int> > allPrevPos, allCalcPos, allGoalPos, allCurrentPos, allPrevFilePos, allCurrentFilePos, allFilePos;
    vector<WriteData> allCalcData, allGoalData;

    int timestep = 0;
    try{
        unsigned int sumMotors = 0;
        vector<unsigned int> allNumMotor;
        for (unsigned int ii = 0; ii < allSys.size(); ii++)
        {
            int numMotors = (*(allSys[ii])).getNumMotors();
            allNumMotor.push_back(numMotors);
            sumMotors += numMotors;
            allCurrentPos.push_back((*(allSys[ii])).getAllPosition());
            WriteData wd;
            wd.setSpeed = false;
            wd.setPID = false;
            wd.setCompliance = false;
            allCalcData.push_back(wd);
        }
        if (allPosInstr.front().size() != sumMotors)
        {
            cout << "Number of columns in instructions does not tally with number of motors. Continue?";
            if(!getYorN())
                return 1;
        }
        allCalcPos = allCurrentPos;
        allFilePos = allCurrentPos;
        allCurrentFilePos = allFilePos;

        while(!allPosInstr.empty() && runFlag)
        {
            allPrevPos = allCurrentPos;
            allGoalPos = allCalcPos;
            allPrevFilePos = allCurrentFilePos;
            allCurrentFilePos = allFilePos;
            allGoalData = allCalcData;

            int idx = 0;
            vector<int> data = allPosInstr.front();
            allPosInstr.pop();
            allPosInstr.push(data);
            for (unsigned int ii = 0; ii < allSys.size(); ii++)
            {
                vector<int> filePos;
                for (unsigned int jj = 0; jj < allNumMotor[ii]; jj++)
                {
                    filePos.push_back(data[idx++]);
                }
                allFilePos[ii] = filePos;
            }

            thread calcThread(multiDevFileCalc, ref(allSys), timestep, ref(allPrevPos), ref(allPrevFilePos), ref(allFilePos), ref(allCalcPos), ref(allCalcData));
            thread fdt[numDXSystem];
            for (int ii = 0; ii < numDXSystem; ii++)
            {
                fdt[ii] = thread(multiDevFileSet,ref(*(allSys[ii])),ref(allGoalPos[ii]),ref(allGoalData[ii]),ref(allCurrentPos[ii]));
            }

            timestepDelay(delayTime);

            calcThread.join();
            for (int ii = 0; ii < numDXSystem; ii++)
            {
                fdt[ii].join();
            }
            timestep++;
        }
    } catch(runtime_error e)
    {
        cout << e.what() << endl;
        cout << "Exit?";
        if (getYorN())
            goto EXITTHREADFN;
    }

    EXITTHREADFN:
    return 0;
}

// Function helper to calculate parameters for next time step
void multiDevFileCalc(vector<DXMotorSystem*>& allSys, int timestep, vector<vector<int> >& allPrevPos, vector<vector<int> >& allPrevFilePos, vector<vector<int> >& allFilePos, vector<vector<int> >& allCalcPos, vector<WriteData>& allCalcData)
{
    for (unsigned int ii = 0; ii < allSys.size(); ii++)
    {
        calcNewPosFromFile(*(allSys[ii]), timestep, allPrevPos[ii], allPrevFilePos[ii], allFilePos[ii], allCalcPos[ii], allCalcData[ii]);
    }
}

// Function helper to set all parameters (position, speed, PID/compliance)
void multiDevFileSet(DXMotorSystem& d, vector<int>& goalPos, WriteData& goalData, vector<int>& currentPos)
{
    if (goalData.setSpeed)
        d.setAllSpeed(goalData.speed);
    if (goalData.setPID)
    {
        d.setAllGainP(goalData.gainP);
        d.setAllGainI(goalData.gainI);
        d.setAllGainD(goalData.gainD);
    }
    if (goalData.setCompliance)
    {
        d.setAllComplianceMargin(goalData.complianceMarginCW,goalData.complianceMarginCCW);
        d.setAllComplianceSlope(goalData.complianceSlopeCW, goalData.complianceSlopeCCW);
    }

    d.setAllPosition(goalPos);
    currentPos = d.getAllPosition();
}
