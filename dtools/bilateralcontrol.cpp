#include <ctime>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <thread>

#include <queue>

#include "additionaltools.h"
#include "setup.h"
#include "src/userinputfn.h"

#include "bilateralcontrol.h"

#define MAX_DATE 16

void torqueControlListener(int& torqueSetting, bool runFlag);
void setPositions(DXMotorSystem& dslave, vector<int>& allPos);
void write2file(ofstream& file, vector<int>& allPos, int timestep);

/**
 * @brief bisystemControl
 * Initiates the bilateral control between master and slave systems
 * @param allSys Vector of pointers to the instantiated DXMotorSystem object pointers
 * @param runFlag Boolean run flag, continues the bilateral control while true - passed by reference to allow changes at runtime
 * @param goalTorqueSetting Desired torque settings to be used (parameters defined as macro in header) - passed by reference to allow modifications at runtime
 * @param filename Prefix filename where recorded position data is saved to
 */
void bisystemControl(vector<DXMotorSystem*>& allSys, bool& runFlag, int& goalTorqueSetting, const string& filename)
{
    try {

        DXMotorSystem dmaster = *(allSys[0]);
        unsigned int numMotors = dmaster.getNumMotors();
        dmaster.setAllTorqueEn(0);
        vector<int> allMasterTorqueLimit(numMotors,MASTER_TORQUE);
        dmaster.setAllTorqueLimit(allMasterTorqueLimit);

        // Check that all systems have the same motor count
        for (unsigned int ii = 1; ii < allSys.size(); ii++)
        {
            if (allSys[ii]->getNumMotors() != numMotors)
                throw(runtime_error("Master and slave motor count do not tally."));
            vector<int> allSlaveTorqueLimit(numMotors, SLAVE_TORQUE);
            allSys[ii]->setAllTorqueLimit(allSlaveTorqueLimit);
        }

        vector<int> posWrite = dmaster.getAllHomePosition(); //dmaster.getAllPosition();
        string writeFilename  = "data/" + filename + getTimeStr() + ".csv";
        ofstream file (writeFilename);

        ostringstream header;
        header << "timestep";
        for (unsigned int ii = 0; ii < numMotors; ii++)
        {
            header << ",#" << ii;
        }
        file << header.str() << endl;

        int torqueSetting = 0;
        vector<int> lowTorques(numMotors,MASTER_LOW_TORQUE);
        vector<int> highTorques(numMotors,MASTER_HIGH_TORQUE);

        vector<queue<vector<int> > > allPosHistory;
        for (unsigned int ii = 0; ii < allSys.size(); ii++)
        {
            queue<vector<int> > posHistory;
            allPosHistory.push_back(posHistory);
        }


        int timestep = 0;
        while(runFlag)
        {
            thread write(write2file, ref(file), ref(posWrite), timestep);

            // Change torque settings of master
            if (torqueSetting != goalTorqueSetting)
            {
                switch(goalTorqueSetting)
                {
                case 1:
                    dmaster.setAllTorqueLimit(lowTorques);
                    dmaster.setAllTorqueEn(1);
                    break;
                case 2:
                    dmaster.setAllTorqueLimit(highTorques);
                    dmaster.setAllTorqueEn(1);
                    break;
                case 0:
                default:
                    dmaster.setAllTorqueLimit(allMasterTorqueLimit);
                    dmaster.setAllTorqueEn(0);
                    break;
                }
                torqueSetting = goalTorqueSetting;
            }

            vector<int> pos = dmaster.getAllPosition();

            thread thr[allSys.size()-1];
            for (unsigned int ii = 0; ii < allSys.size()-1; ii++)
            {
                thr[ii] = thread(setPositions,ref(*(allSys[ii+1])),ref(pos));

//                allSys[ii+1]->setAllPosition(pos);
//                allSys[ii+1]->getAllPosition();
            }
            posWrite = pos; // position data to write (in next time step)

            write.join();
            for (unsigned int ii = 0; ii < allSys.size()-1; ii++)
            {
                thr[ii].join();
            }
            timestep++;
        }

        file.close();
        // Delete pointers
        for (unsigned int nn = 0; nn < allSys.size(); nn++)
        {
            allSys[nn]->setAllTorqueLimit(vector<int>(numMotors,1023));
            allSys[nn]->setAllTorqueEn(0);
            delete allSys[nn];
        }

    } catch(runtime_error e) {
        cout << e.what() << endl;
    }
}

/**
 * @brief setPositions
 * Function helper to set (and get, to obtain status packets) all positions
 * @param d Reference to the motor system
 * @param allPos Desired positions of the motors (given in an int vector corresponding to the motor #)
 */
void setPositions(DXMotorSystem& d, vector<int>& allPos)
{
    d.setAllPosition(allPos);
    d.getAllPosition(); // To obtain status packets for motor system
}

/**
 * @brief write2file
 * Function helper to write vector of position date to file stream
 * @param file Reference to file stream (target CSV file for writing)
 * @param allPos Positions of the motors (given in an int vector corresponding to the motor #)
 * @param timestep Time step of sequence
 */
void write2file(ofstream& file, vector<int>& allPos, int timestep)
{
    ostringstream posString;
    posString << timestep;
    for (unsigned int ii = 0; ii < allPos.size(); ii++)
    {
        posString << "," << allPos[ii];
    }

    file << posString.str() << endl;
}

