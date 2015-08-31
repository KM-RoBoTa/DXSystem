#ifndef USERINPUTFN_H
#define USERINPUTFN_H

#include <vector>

#include "dtools/threadedreadfile.h"
#include "dclass/dxmotorsystem.h"
#include "dclass/dxsinglemotor.h"

void calcNewPosFromFile(DXMotorSystem& dptr, int timestep, const vector<int>& prevPos, const vector<int>& prevFilePos, const vector<int>& filePos, vector<int>& calcPos, WriteData& calcData);
void dxlSetupEEPROM(DXMotorSystem& d);
void dxlSetupEEPROM(DXSingleMotor& d);
void timestepDelay(DXMotorSystem& d, int delayTime = 4);
void timestepDelay(int delayTime = 4);

#endif // USERINPUTFN_H
