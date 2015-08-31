#include <stdexcept>

#include "dtools/additionaltools.h"

#include "dxsinglemotor.h"

const int MOTOR_NUMBER = 0;

DXSingleMotor::DXSingleMotor(int devicePort, int dxlBaudRate):DXMotorSystem(devicePort, dxlBaudRate)
{
    if (isInitialized())
    {
        if (numMotors > 1)
            handleError("Please only connect a single motor in this system", true);

    }
}

DXSingleMotor::~DXSingleMotor()
{

}

//----------//
bool DXSingleMotor::isInitialized()
{
    return DXMotorSystem::isInitialized();
}

bool DXSingleMotor::motorInitialized()
{
    return numMotors == 1;
}

void DXSingleMotor::lockEEPROM()
{
    DXMotorSystem::lockEEPROM();
}

//----------//

int DXSingleMotor::getLoad()
{
    return DXMotorSystem::getLoad(MOTOR_NUMBER);
}

int DXSingleMotor::getTemperature()
{
    return DXMotorSystem::getTemperature(MOTOR_NUMBER);
}

int DXSingleMotor::getVoltage()
{
    return DXMotorSystem::getVoltage(MOTOR_NUMBER);
}

int DXSingleMotor::getMaxTemperature()
{
    return DXMotorSystem::getMaxTemperature(MOTOR_NUMBER);
}

int DXSingleMotor::getMaxVoltage()
{
    return DXMotorSystem::getMaxVoltage(MOTOR_NUMBER);
}

int DXSingleMotor::getMinVoltage()
{
    return DXMotorSystem::getMinVoltage(MOTOR_NUMBER);
}

int DXSingleMotor::getDelay()
{
    return DXMotorSystem::getDelay(MOTOR_NUMBER);
}

int DXSingleMotor::getStatusReturnLevel()
{
    return systemSRL;
}
int DXSingleMotor::getBaudRate()
{
    return DXMotorSystem::getBaudRate(MOTOR_NUMBER);
}

int DXSingleMotor::getID()
{
    return allID[MOTOR_NUMBER];
}

int DXSingleMotor::getModel()
{
    return allModel[MOTOR_NUMBER];
}

//----------//
void DXSingleMotor::setPositionLimits(int lowerBound, int upperBound)
{
    DXMotorSystem::setPositionLimits(MOTOR_NUMBER, lowerBound, upperBound);
}

int DXSingleMotor::getPositionLowerLimit()
{
    return DXMotorSystem::getPositionLowerLimit(MOTOR_NUMBER);
}

int DXSingleMotor::getPositionUpperLimit()
{
    return DXMotorSystem::getPositionUpperLimit(MOTOR_NUMBER);
}

void DXSingleMotor::setTorqueLimit(int upperBound)
{
    DXMotorSystem::setTorqueLimit(MOTOR_NUMBER, upperBound);
}

int DXSingleMotor::getTorqueMax()
{
    return DXMotorSystem::getTorqueMax(MOTOR_NUMBER);
}

void DXSingleMotor::setTorqueEn(bool enableVal)
{
    DXMotorSystem::setTorqueEn(MOTOR_NUMBER, enableVal);
}

bool DXSingleMotor::getTorqueEn()
{
    return DXMotorSystem::getTorqueEn(MOTOR_NUMBER);
}

void DXSingleMotor::queuePosition(int goalPosition)
{
    DXMotorSystem::queuePosition(MOTOR_NUMBER, goalPosition);
}

void DXSingleMotor::setPosition(int goalPosition)
{
    return DXMotorSystem::setPosition(MOTOR_NUMBER, goalPosition);
}

void DXSingleMotor::movetoPosition(int goalPosition)
{
    return DXMotorSystem::movetoPosition(MOTOR_NUMBER, goalPosition);
}

int DXSingleMotor::getPosition()
{
    return DXMotorSystem::getPosition(MOTOR_NUMBER);
}

void DXSingleMotor::queueSpeed(int goalSpeed)
{
    DXMotorSystem::queueSpeed(MOTOR_NUMBER, goalSpeed);
}

void DXSingleMotor::setSpeed(int goalSpeed)
{
    return DXMotorSystem::setSpeed(MOTOR_NUMBER, goalSpeed);
}

int DXSingleMotor::getCurrentSpeed()
{
    return DXMotorSystem::getCurrentSpeed(MOTOR_NUMBER);
}

int DXSingleMotor::getSetSpeed()
{
    return DXMotorSystem::getSetSpeed(MOTOR_NUMBER);
}

bool DXSingleMotor::isMoving()
{
    return DXMotorSystem::isMoving(MOTOR_NUMBER);
}

void DXSingleMotor::resetMovingSettings()
{
    DXMotorSystem::resetMovingSettings(MOTOR_NUMBER);
}

// PWM
void DXSingleMotor::setPunch(int punch)
{
    DXMotorSystem::setPosition(MOTOR_NUMBER, punch);
}

int DXSingleMotor::getPunch()
{
    return DXMotorSystem::getPunch(MOTOR_NUMBER);
}

// PID
void DXSingleMotor::setGainP(int pVal)
{
    DXMotorSystem::setGainP(MOTOR_NUMBER, pVal);
}

int DXSingleMotor::getGainP()
{
    return DXMotorSystem::getGainP(MOTOR_NUMBER);
}

void DXSingleMotor::setGainI(int iVal)
{
    DXMotorSystem::setGainI(MOTOR_NUMBER, iVal);
}

int DXSingleMotor::getGainI()
{
    return DXMotorSystem::getGainI(MOTOR_NUMBER);
}

void DXSingleMotor::setGainD(int dVal)
{
    DXMotorSystem::setGainD(MOTOR_NUMBER, dVal);
}

int DXSingleMotor::getGainD()
{
    return DXMotorSystem::getGainD(MOTOR_NUMBER);
}

// Compliance Settings
void DXSingleMotor::setComplianceSlope(int cwSlope, int ccwSlope)
{
    DXMotorSystem::setComplianceSlope(MOTOR_NUMBER, cwSlope, ccwSlope);
}

void DXSingleMotor::setComplianceMargin(int cwMargin, int ccwMargin)
{
    DXMotorSystem::setComplianceMargin(MOTOR_NUMBER, cwMargin, ccwMargin);
}

int DXSingleMotor::getComplianceSlopeCW()
{
    return DXMotorSystem::getComplianceSlopeCW(MOTOR_NUMBER);
}

int DXSingleMotor::getComplianceSlopeCCW()
{
    return DXMotorSystem::getComplianceSlopeCCW(MOTOR_NUMBER);
}

int DXSingleMotor::getComplianceMarginCW()
{
    return DXMotorSystem::getComplianceMarginCW(MOTOR_NUMBER);
}

int DXSingleMotor::getComplianceMarginCCW()
{
    return DXMotorSystem::getComplianceMarginCCW(MOTOR_NUMBER);
}

