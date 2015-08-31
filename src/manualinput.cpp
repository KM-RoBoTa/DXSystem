#include <iostream>
#include <stdexcept>

#include "dstopwatch/dstopwatch.h"

#include "dtools/additionaltools.h"
#include "dtools/setup.h"

#include "manualinput.h"

bool sampleProgram(DXMotorSystem& d);
int askMotorNo(DXMotorSystem& d);
void setGoalPosition(DXMotorSystem& d);
void setSpeed(DXMotorSystem& d);
void setPID(DXMotorSystem& d);
void getInfo(DXMotorSystem& d);

using namespace std;

/**
 * @brief askInputSequenceSample
 * Sample program sequence for manual control of motors
 */
void askInputSequenceSample()
{
    try{
    DXMotorSystem* d = dxsystemSetup(0);
    while(sampleProgram(*d));
    } catch (runtime_error e) {
        cout << e.what() << endl << endl;
        return;
    }
}


/**
 * @brief sampleProgram
 * Sample program for manual control of motors
 * @param d1 Reference to the motor system
 * @return Boolean flag corresponding to continue (true) or exit (false)
 */
bool sampleProgram(DXMotorSystem& d)
{

    try{
    cout << "Select Mode:" << endl;
    cout << "1. Set Goal Position" << endl;
    cout << "2. Set Speed" << endl;
    cout << "3. Set PID" << endl;
    cout << "4. Get Information" << endl;
    cout << "5. Reset" << endl;
    cout << "6. Identify/Detect Motors" << endl;
    cout << "0. Quit." << endl;
    cout << "--" << endl;
    int mode;
    cout << "Input Mode:";

    mode = getInt();
    switch(mode)
    {
    case 0:
        return false;
    case 1:
        setGoalPosition(d);
        break;
    case 2:
        setSpeed(d);
        break;
    case 3:
        setPID(d);
        break;
    case 4:
        getInfo(d);
        break;
    case 5:
        cout << "Resetting" << endl;
        d.setAllPosition(d.getAllHomePosition());
        break;
    case 6:
        d.identifyMotors();
        break;
    default:
        break;
    }
    cout << "--" << endl;
    } catch (runtime_error e) {
        cout << e.what() << endl << endl;
    }
    return true;
}

/**
 * @brief askMotorNo
 * Asks for motor number
 * @param d Reference to the motor system
 * @return Motor number
 */
int askMotorNo(DXMotorSystem& d)
{
    unsigned int motorNo;
    cout << "Input motor # to control (254 for all):";
    motorNo = getInt();
    if (motorNo < d.getNumMotors() || motorNo == 254)
        return motorNo;
    else
        cout << "Motor Number not valid" << endl;
    return -1;
}

// Mode 1: Set Goal Position
/**
 * @brief setGoalPosition
 * Sets goal position
 * @param d Reference to the motor system
 */
void setGoalPosition(DXMotorSystem& d)
{
    int motorNo = askMotorNo(d);
    if (motorNo < 0)
        return;
    int presentPos, goalPos;
    cout << "Input Desired Position:";
    goalPos = getInt();
    if (motorNo == 254) {
        vector<int> allPos(d.getNumMotors(), goalPos);
        d.setAllPosition(allPos);
        return;
    }
    d.setPosition(motorNo, goalPos);
    presentPos = d.getPosition(motorNo);
    cout << "Goal(Set):" << goalPos << "    Present:" << presentPos << endl;
}

// Mode 2: Set Goal Speed
/**
 * @brief setSpeed
 * Sets goal speed
 * @param d Reference to the motor system
 */
void setSpeed(DXMotorSystem& d)
{
    int motorNo = askMotorNo(d);
    if (motorNo < 0)
        return;
    int goalSpeed, presentSpeed;
    cout << "Input Desired Speed:" << endl;
    goalSpeed = getInt();
    if (motorNo == 254) {
        vector<int> allSpd(d.getNumMotors(),goalSpeed);
        d.setAllSpeed(allSpd);
        return;
    }
    d.setSpeed(motorNo, goalSpeed);
    presentSpeed = d.getSetSpeed(motorNo);
    cout << "Goal(Set):" << goalSpeed << "    Presently Set:" << presentSpeed << endl;
}

// Mode 3: Set PID
/**
 * @brief setPID
 * Sets goal PID
 * @param d Reference to the motor system
 */
void setPID(DXMotorSystem& d)
{
    int motorNo = askMotorNo(d);
    if (motorNo < 0)
        return;
    int gainP, gainI, gainD;
    while(true)
    {
        cout << "Input Desired P:" << endl;
        gainP = getInt();
        cout << "Input Desired I:" << endl;
        gainI = getInt();
        cout << "Input Desired D:" << endl;
        gainD = getInt();
        if (gainP >= 0 && gainP <= 254 && gainI >= 0 && gainI <= 254 && gainD >= 0 && gainD <= 254)
        {
            if (motorNo == 254) {
                vector<int> vP(d.getNumMotors(),gainP);
                vector<int> vI(d.getNumMotors(),gainI);
                vector<int> vD(d.getNumMotors(),gainD);
                d.setAllGainP(vP);
                d.setAllGainI(vI);
                d.setAllGainD(vD);
                return;
            }
            d.setGainP(motorNo, gainP);
            d.setGainI(motorNo, gainI);
            d.setGainD(motorNo, gainD);
            return;
        }
        cout << "PID values should be between 0 and 254" << endl;

    }
}

// Mode 4: Get Information
/**
 * @brief getInfo
 * Prints information of the motors
 * @param d Reference to the motor system
 */
void getInfo(DXMotorSystem& d)
{
    for (unsigned int ii = 0; ii < d.getNumMotors(); ii++)
    {
        cout << "-----" << endl;
        cout << "Device Motor #" << ii << endl;
        cout << "BaudRate:" << d.getBaudRate(ii) << endl;
        cout << "Delay Status:" << d.getDelay(ii) << endl;
        cout << "Status Return Level:" << d.getStatusReturnLevel(ii) << endl;
        cout << "Voltage:" << d.getVoltage(ii) << endl;
        cout << "Temperature:" << d.getTemperature(ii) << endl;
        cout << "Load:" << d.getLoad(ii) << endl;
        cout << "Position Limits:" << d.getPositionLowerLimit(ii) << "," << d.getPositionUpperLimit(ii) << endl;
        cout << "Current Position:" << d.getPosition(ii) << endl;
        cout << "Current Speed:" << d.getSetSpeed(ii) << endl;
        cout << "Current PID:" << d.getGainP(ii) << ", " << d.getGainI(ii) << ", " << d.getGainD(ii) << endl;
        cout << "-----" << endl;
    }
    cout << endl;
}
