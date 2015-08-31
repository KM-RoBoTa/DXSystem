#ifndef READFILE_H
#define READFILE_H

#include <queue>

#include "dclass/dxmotorsystem.h"

struct Limits {
    int maxPosition;
    int maxAngle;
    int maxRPM;
};

vector<Limits> getDXSystemInfo(DXMotorSystem& d);
string writeDXSystemInfo(DXMotorSystem& d);
string parseAngles2File(DXMotorSystem& d, const string& anglesFilename = "data/angles.csv", bool hasLeftHeader = true, bool hasTopHeader = true, bool isRadians = false);
int getInstructions(queue<vector<int> > &all_instr, const string& filename, bool hasLeftHeader = true, bool hasTopHeader = true);
int getInstructions(queue<vector<double> >& all_instr, const string& filename, bool hasLeftHeader = true, bool hasTopHeader = true);

int readFile2Byte(DXMotorSystem& d, const string& filename, int addr, bool& runFlag, int delayTime = 4, bool hasLeftHeader = true, bool hasTopHeader = true);
int readFile2Word(DXMotorSystem& d, const string& filename, int addr, bool& runFlag, int delayTime = 4, bool hasLeftHeader = true, bool hasTopHeader = true);
int readFile2Pos(DXMotorSystem& d, const string& filename, bool& runFlag, int delayTime = 4, bool hasLeftHeader = true, bool hasTopHeader = true);
int readFile2PosSpd(DXMotorSystem& d, const string& filename, bool& runFlag, int delayTime = 4, bool hasLeftHeader = true, bool hasTopHeader = true);
int readFile2PosSpdPID(DXMotorSystem& d, const string& filename, bool& runFlag, int delayTime = 4, bool hasLeftHeader = true, bool hasTopHeader = true);

int readFile2Byte(DXMotorSystem& d, queue<vector<int> >& all_instr, int addr, bool& runFlag, int delayTime = 4);
int readFile2Word(DXMotorSystem& d, queue<vector<int> >& all_instr, int addr, bool& runFlag, int delayTime = 4);
int readFile2Pos(DXMotorSystem& d, queue<vector<int> >& all_instr, bool& runFlag, int delayTime = 4);
int readFile2PosSpd(DXMotorSystem& d, queue<vector<int> >& all_instr, bool& runFlag, int delayTime = 4);
int readFile2PosSpdPID(DXMotorSystem& d, queue<vector<int> >& all_instr, bool& runFlag, int delayTime = 4);

int readFile2Byte(DXMotorSystem& d, const string& filename, int addr, int delayTime = 4, bool hasLeftHeader = true, bool hasTopHeader = true);
int readFile2Word(DXMotorSystem& d, const string& filename, int addr, int delayTime = 4, bool hasLeftHeader = true, bool hasTopHeader = true);
int readFile2Pos(DXMotorSystem& d, const string& filename, int delayTime = 4, bool hasLeftHeader = true, bool hasTopHeader = true);
int readFile2PosSpd(DXMotorSystem& d, const string& filename, int delayTime = 4, bool hasLeftHeader = true, bool hasTopHeader = true);
int readFile2PosSpdPID(DXMotorSystem& d, const string& filename, int delayTime = 4, bool hasLeftHeader = true, bool hasTopHeader = true);

int readFile2Byte(DXMotorSystem& d, queue<vector<int> >& all_instr, int addr, int delayTime = 4);
int readFile2Word(DXMotorSystem& d, queue<vector<int> >& all_instr, int addr, int delayTime = 4);
int readFile2Pos(DXMotorSystem& d, queue<vector<int> >& all_instr, int delayTime = 4);
int readFile2PosSpd(DXMotorSystem& d, queue<vector<int> >& all_instr, int delayTime = 4);
int readFile2PosSpdPID(DXMotorSystem& d, queue<vector<int> >& all_instr, int delayTime = 4);

void loopReading(DXMotorSystem& d, queue<vector<int> >& allInstr, bool& runFlag, int delayTime = 4, bool loop = true);

void writeHomePositions(DXMotorSystem& d, const string& homeFile = "data/homepositions.csv");
int readHomePositions(DXMotorSystem& d, const string& homeFile = "data/homepositions.csv");

#endif // READFILE_H
