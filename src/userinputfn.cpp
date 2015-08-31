#include <iostream>
#include <stdexcept>

#include "dclass/dmodeldetails.h"
#include "dclass/control_table_constants.h"
#include "dtools/additionaltools.h"
#include "dtools/setup.h"
#include "dtools/readfile.h"
#include "dstopwatch/dstopwatch.h"

#include "userinputfn.h"

using namespace std;

/**
 * @brief calcNewPosFromFile
 * User defined function used to calculate the positions for the next time step, used in threaded file-reading/closed loop sequence
 * @param d Reference to the motor system
 * @param timestep Time step at point of calculation
 * @param prevPos Position data vector read from the system from the previous time step
 * @param prevFilePos Position data vector from the csv file instruction from the previous time step
 * @param filePos Position data vector from the csv file instructions for the next time step
 * @param calcPos Reference to calculated position data vector, which will be the position data values to be set in the next time step
 * @param calcData Reference to calculated data vector, which holds the speed/PID/compliance information to be set in the next time step
 */
void calcNewPosFromFile(DXMotorSystem& d, int timestep, const vector<int>& prevPos, const vector<int>& prevFilePos, const vector<int>& filePos, vector<int>& calcPos, WriteData& calcData)
{
    calcData = WriteData{};
    calcData.setCompliance = false;
    calcData.setPID = false;
    calcData.setSpeed = false;

    calcPos = filePos;
}

/**
 * @brief dxlSetupEEPROM
 * Function that applies the relevant settings and constraints before locking up the EEPROM
 * @param d Reference to the motor system
 */
void dxlSetupEEPROM(DXMotorSystem& d)
{
    int numMotors = d.getNumMotors();
    d.setAllPositionLimits(vector<int>(numMotors,0),vector<int>(numMotors,1023));
}
void dxlSetupEEPROM(DXSingleMotor& d)
{

}

/**
 * @brief timestepDelay
 * Manipulates the behavior of delay in each timestep
 * @param d Reference to the motor system - optional
 * @param delayTime Delay at each cycle - optional, default to 4
 */
void timestepDelay(DXMotorSystem& d, int delayTime) // Used in readfile functions
{
    delay(delayTime);
}

void timestepDelay(int delayTime) // Used in threadreadfile functions
{
    delay(delayTime);
}
