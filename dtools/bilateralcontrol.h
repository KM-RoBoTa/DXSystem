#ifndef BILATERALCONTROL_H
#define BILATERALCONTROL_H

#include "dclass/dxmotorsystem.h"

#define MASTER_TORQUE 0
#define MASTER_LOW_TORQUE 256
#define MASTER_HIGH_TORQUE 1023
#define SLAVE_TORQUE 1023

void bisystemControl(vector<DXMotorSystem*>& allSys, bool& runFlag, int& goalTorqueSetting, const string& filename = "bisystemdata");

#endif // BILATERALCONTROL_H
