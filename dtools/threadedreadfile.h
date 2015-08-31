#ifndef THREADEDREADFILE_H
#define THREADEDREADFILE_H

#include <string>
#include <vector>

using namespace std;

typedef struct writedata {
    bool setSpeed = false;
    vector<int> speed;

    bool setPID = false;
    vector<int> gainP;
    vector<int> gainI;
    vector<int> gainD;

    bool setCompliance = false;
    vector<int> complianceSlopeCW;
    vector<int> complianceMarginCW;
    vector<int> complianceSlopeCCW;
    vector<int> complianceMarginCCW;
} WriteData;

void threadedReadFileSequenceSample(int numPorts, const string& filename, bool& runFlag, int delayTime = 4);

#endif // THREADEDREADFILE_H
