#ifndef DSINGLEMOTOR_H
#define DSINGLEMOTOR_H

#include "dxmotorsystem.h"

class DXSingleMotor: protected DXMotorSystem
{
public:
    DXSingleMotor(int devicePort, int dxlBaudRate);
    ~DXSingleMotor();

    bool isInitialized();
    bool motorInitialized();
    void lockEEPROM();

    int getLoad();
    int getTemperature();
    int getVoltage();
    int getMaxTemperature();
    int getMaxVoltage();
    int getMinVoltage();
    int getDelay();
    int getStatusReturnLevel();
    int getBaudRate();
    int getID();
    int getModel();

    void setPositionLimits(int lowerBound, int upperBound);
    int getPositionLowerLimit();
    int getPositionUpperLimit();

    void setTorqueLimit(int upperBound);
    int getTorqueMax();

    void setTorqueEn(bool enableVal);
    bool getTorqueEn();

    void queuePosition(int goalPosition);
    void setPosition(int goalPosition);
    void movetoPosition(int goalPosition);
    int getPosition();

    void queueSpeed(int goalSpeed);
    void setSpeed(int goalSpeed);
    int getCurrentSpeed();
    int getSetSpeed();

    bool isMoving();
    void resetMovingSettings();

    // PWM
    void setPunch(int punch);
    int getPunch();
    // PID
    void setGainP(int pVal);
    int getGainP();

    void setGainI(int iVal);
    int getGainI();

    void setGainD(int dVal);
    int getGainD();

    // To be implemented
    // Only for AX12 and RX28
    void setComplianceSlope(int cwSlope, int ccwSlope);
    void setComplianceMargin(int cwMargin, int ccwMargin);
    int getComplianceSlopeCW();
    int getComplianceSlopeCCW();
    int getComplianceMarginCW();
    int getComplianceMarginCCW();
};

#endif // DSINGLEMOTOR_H
