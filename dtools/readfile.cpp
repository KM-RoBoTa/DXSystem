#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "additionaltools.h"
#include "dclass/dmodeldetails.h"
#include "dclass/control_table_constants.h"
#include "src/userinputfn.h"

#include "readfile.h"

#define PI 3.14159

using namespace std;

/**
 * @brief getDXSystemInfo
 * Saves motors information/specifications to a vector of Limits structs, for subsequent reference
 * @param d Reference to the motor system
 * @return Vector of limits info from the motors (given in a Limits vector corresponding to the motor #)
 */
vector<Limits> getDXSystemInfo(DXMotorSystem& d)
{
   vector<Limits> allLimits;

    for (unsigned int ii = 0; ii < d.getNumMotors(); ii++)
    {
        Limits lim = {
            modelMaxPosition(d.getModel(ii)),
            modelAngleRange(d.getModel(ii)),
            modelMaxRPM(d.getModel(ii)),
        };
        allLimits.push_back(lim);
    }

    return allLimits;
}

/**
 * @brief writeDXSystemInfo
 * Writes motors information/specifications (position, angle and speed limits) to a csv file
 * @param d Reference to the motor system
 * @return String file name of the written csv file
 */
string writeDXSystemInfo(DXMotorSystem& d)
{
    vector<Limits> allLimits = getDXSystemInfo(d);
    ostringstream posLimits;
    ostringstream angleLimits;
    ostringstream spdLimits;

    posLimits << "Max Pos (Digital Value)";
    angleLimits << "Max Angle Range (Degree)";
    spdLimits << "Max Speed (RPM corresponding to 1023)";
    for (unsigned int ii = 0; ii < allLimits.size(); ii++) {
        posLimits << "," << allLimits[ii].maxPosition;
        angleLimits << "," << allLimits[ii].maxAngle;
        spdLimits << "," << allLimits[ii].maxRPM;
    }

    string writeFilename = "data/LimitsInfo" + getTimeStr() + ".csv";
    fstream limInfo (writeFilename, ios_base::in | ios_base::out | ios_base::trunc);

    limInfo << posLimits.str() << endl << angleLimits.str() << endl << spdLimits.str() << endl;
    limInfo.close();
    cout << "Files written" << endl;

    return writeFilename;
}

/**
 * @brief parseAngles2File
 * Parses a csv file with input joint angle values and outputs a csv file with position values
 * @param d Reference to the motor system
 * @param anglesFilename File name of the csv file containing input angle values (default: "data/angles.csv")
 * @param hasLeftHeader Flag to denote the presence of a left header (i.e. ignore first column)
 * @param hasTopHeader Flag to denote the presence of a top header (i.e. ignore first row)
 * @param isRadians Flag to denote if the angle data is in radians (true) or degrees (false)
 * @return String file name of the written csv file
 */
string parseAngles2File(DXMotorSystem& d, const string& anglesFilename, bool hasLeftHeader, bool hasTopHeader, bool isRadians)
{
    unsigned int numMotors = d.getNumMotors();
    queue<vector<double> > allAngleVal;
    if(getInstructions(allAngleVal, anglesFilename, hasLeftHeader, hasTopHeader))
    {
        throw(runtime_error("Unable to parse angle files"));
        return "";
    }

    vector<Limits> allLimits = getDXSystemInfo(d);
    vector<double> angle2PosUnits(numMotors, -1);
    for (unsigned int ii = 0; ii < numMotors; ii++)
    {
        angle2PosUnits[ii]= (double) allLimits[ii].maxPosition/allLimits[ii].maxAngle;
    }

    queue<vector<int> > allPosData;
    if(d.getNumMotors() != allAngleVal.front().size())
    {
        throw(runtime_error("Number of motors does not tally with number of columns of data. Please check the CSV file format and the checkboxes for top/left headers."));
        return "";
    }
    while(!allAngleVal.empty())
    {
        vector<double> angles = allAngleVal.front();
        allAngleVal.pop();
        vector<int> positions(numMotors,-1);
        double angleInDegree;
        for (unsigned int ii = 0; ii < angles.size(); ii++)
        {
            angleInDegree = isRadians ? angles[ii]*180/PI : angles[ii];
            positions[ii] = ((angleInDegree + allLimits[ii].maxAngle/2)*angle2PosUnits[ii]) + 0.5;
            if (positions[ii] > allLimits[ii].maxPosition)
                positions[ii] = allLimits[ii].maxPosition;
            else if (positions[ii] < 0)
                positions[ii] = 0;
        }
        allPosData.push(positions);
    }


    string writeFilename = "data/readFile" + getTimeStr() + ".csv";
    ofstream dataFile(writeFilename);
    int timestep = 0;
    ostringstream header;
    header << "timestep";
    for (unsigned int ii = 0; ii < numMotors; ii++)
    {
        header << ",#" << ii;
    }
    dataFile << header.str() << endl;
    while(!allPosData.empty())
    {
        ostringstream posStream;
        posStream << timestep++;
        vector<int> posData = allPosData.front();
        allPosData.pop();
        for (unsigned int ii = 0; ii < posData.size(); ii++)
        {
            posStream << "," << posData[ii];
        }
        dataFile << posStream.str() << endl;
    }
    dataFile.close();

    return writeFilename;
}


/**
 * @brief readFile2Byte
 * Reads a csv file/Takes in a queue of instructions, and parses the information (byte) to the motor system
 * @param d Reference to the motor system
 * @param filename File name of the csv file
 * @param addr Address on the Control Table of which the data should be written to
 * @param delayTime Delay time between setting each timestep (row) - optional, default to 4
 * @param hasLeftHeader Flag to denote the presence of a left header (i.e. ignore first column)
 * @param hasTopHeader Flag to denote the presence of a top header (i.e. ignore first row)
 * @return Status flag - 0 upon exit without issues
 */
int readFile2Byte(DXMotorSystem& d, const string& filename, int addr, int delayTime, bool hasLeftHeader, bool hasTopHeader)
{
    bool runFlag = true;
    return readFile2Byte(d,filename,addr,runFlag,delayTime, hasLeftHeader, hasTopHeader);
}

/**
 * @brief readFile2Byte
 * Reads a csv file/Takes in a queue of instructions, and parses the information (byte) to the motor system
 * @param d Reference to the motor system
 * @param all_instr Instructions parsed from the csv file
 * @param addr Address on the Control Table of which the data should be written to
 * @param delayTime Delay time between setting each timestep (row) - optional, default to 4
 * @return Status flag - 0 upon exit without issues
 */
int readFile2Byte(DXMotorSystem& d, queue<vector<int> >& all_instr, int addr, int delayTime)
{
    bool runFlag = true;
    return readFile2Byte(d,all_instr,addr,runFlag,delayTime);
}

/**
 * @brief readFile2Byte
 * Reads a csv file/Takes in a queue of instructions, and parses the information (byte) to the motor system
 * @param d Reference to the motor system
 * @param filename File name of the csv file
 * @param addr Address on the Control Table of which the data should be written to
 * @param runFlag Reference to a boolean, only continues reading the file when true - optional
 * @param delayTime Delay time between setting each timestep (row) - optional, default to 4
 * @param hasLeftHeader Flag to denote the presence of a left header (i.e. ignore first column)
 * @param hasTopHeader Flag to denote the presence of a top header (i.e. ignore first row)
 * @return Status flag - 0 upon exit without issues
 */
int readFile2Byte(DXMotorSystem& d, const string& filename, int addr, bool& runFlag, int delayTime, bool hasLeftHeader, bool hasTopHeader)
{
    queue<vector<int> > all_instr;
    if (getInstructions(all_instr, filename, hasLeftHeader, hasTopHeader))
        return 1;
    return readFile2Byte(d, all_instr, addr, runFlag, delayTime);
}

/**
 * @brief readFile2Byte
 * Reads a csv file/Takes in a queue of instructions, and parses the information (byte) to the motor system
 * @param d Reference to the motor system
 * @param all_instr Instructions parsed from the csv file
 * @param addr Address on the Control Table of which the data should be written to
 * @param runFlag Reference to a boolean, only continues reading the file when true - optional
 * @param delayTime Delay time between setting each timestep (row) - optional, default to 4
 * @return Status flag - 0 upon exit without issues
 */
int readFile2Byte(DXMotorSystem& d, queue<vector<int> >& all_instr, int addr, bool& runFlag, int delayTime)
{
    if (all_instr.front().size() != d.getNumMotors())
    {
        cout << "Number of columns in instructions does not tally with number of motors. Continue?";
        if(!getYorN())
            return 1;
    }
    int numRow = all_instr.size();
    for (int ii = 0; ii < numRow && runFlag; ii++)
    {
        vector<int> vData = all_instr.front();
        all_instr.pop();
        all_instr.push(vData);
        d.setAllByte(addr, vData);
        timestepDelay(d, delayTime);
    }
    return 0;
}

/**
 * @brief readFile2Word
 * Reads a csv file and parses the information (word) to the motor system
 * @param d Reference to the motor system
 * @param filename File name of the csv file
 * @param addr Address on the Control Table of which the data should be written to
 * @param delayTime Delay time between setting each timestep (row) - optional, default to 4
 * @param hasLeftHeader Flag to denote the presence of a left header (i.e. ignore first column)
 * @param hasTopHeader Flag to denote the presence of a top header (i.e. ignore first row)
 * @return Status flag - 0 upon exit without issues
 */
int readFile2Word(DXMotorSystem& d, const string& filename, int addr, int delayTime, bool hasLeftHeader, bool hasTopHeader)
{
    bool runFlag = true;
    return readFile2Word(d,filename,addr,runFlag,delayTime, hasLeftHeader, hasTopHeader);
}

/**
 * @brief readFile2Word
 * Reads a csv file and parses the information (word) to the motor system
 * @param d Reference to the motor system
 * @param all_instr Instructions parsed from the csv file
 * @param addr Address on the Control Table of which the data should be written to
 * @param delayTime Delay time between setting each timestep (row) - optional, default to 4
 * @return Status flag - 0 upon exit without issues
 */
int readFile2Word(DXMotorSystem& d, queue<vector<int> >& all_instr, int addr, int delayTime)
{
    bool runFlag = true;
    return readFile2Word(d,all_instr,addr,runFlag,delayTime);
}

/**
 * @brief readFile2Word
 * Reads a csv file and parses the information (word) to the motor system
 * @param d Reference to the motor system
 * @param filename File name of the csv file
 * @param addr Address on the Control Table of which the data should be written to
 * @param runFlag Reference to a boolean, only continues reading the file when true - optional
 * @param delayTime Delay time between setting each timestep (row) - optional, default to 4
 * @param hasLeftHeader Flag to denote the presence of a left header (i.e. ignore first column)
 * @param hasTopHeader Flag to denote the presence of a top header (i.e. ignore first row)
 * @return Status flag - 0 upon exit without issues
 */
int readFile2Word(DXMotorSystem& d, const string& filename, int addr, bool& runFlag, int delayTime, bool hasLeftHeader, bool hasTopHeader)
{
    queue<vector<int> > all_instr;
    if (getInstructions(all_instr, filename, hasLeftHeader, hasTopHeader))
        return 1;
    return readFile2Word(d, all_instr, addr, runFlag, delayTime);
}

/**
 * @brief readFile2Word
 * Reads a csv file and parses the information (word) to the motor system
 * @param d Reference to the motor system
 * @param all_instr Instructions parsed from the csv file
 * @param addr Address on the Control Table of which the data should be written to
 * @param runFlag Reference to a boolean, only continues reading the file when true - optional
 * @param delayTime Delay time between setting each timestep (row) - optional, default to 4
 * @return Status flag - 0 upon exit without issues
 */
int readFile2Word(DXMotorSystem& d, queue<vector<int> >& all_instr, int addr, bool& runFlag, int delayTime)
{
    if (all_instr.front().size() != d.getNumMotors())
    {
        cout << "Number of columns in instructions does not tally with number of motors. Continue?";
        if(!getYorN())
            return 1;
    }
    int numRow = all_instr.size();
    for (int ii = 0; ii < numRow && runFlag; ii++)
    {
        vector<int> vData = all_instr.front();
        all_instr.pop();
        all_instr.push(vData);
        d.setAllWord(addr, vData);
        timestepDelay(d, delayTime);
    }

    return 0;
}

/**
 * @brief readFile2Pos
 * Reads a csv file containing position data and parses the information to the motor system
 * @param d Reference to the motor system
 * @param filename File name of the csv file
 * @param delayTime Delay time between setting each timestep (row) - optional, default to 4
 * @param hasLeftHeader Flag to denote the presence of a left header (i.e. ignore first column)
 * @param hasTopHeader Flag to denote the presence of a top header (i.e. ignore first row)
 * @return Status flag - 0 upon exit without issues
 */
int readFile2Pos(DXMotorSystem& d, const string& filename, int delayTime, bool hasLeftHeader, bool hasTopHeader)
{
    bool runFlag = true;
    return readFile2Pos(d,filename,runFlag,delayTime, hasLeftHeader, hasTopHeader);
}

/**
 * @brief readFile2Pos
 * Reads a csv file containing position data and parses the information to the motor system
 * @param d Reference to the motor system
 * @param filename File name of the csv file
 * @param all_instr Instructions parsed from the csv file
 * @param delayTime Delay time between setting each timestep (row) - optional, default to 4
 * @return Status flag - 0 upon exit without issues
 */
int readFile2Pos(DXMotorSystem& d, queue<vector<int> >& all_instr, int delayTime)
{
    bool runFlag = true;
    return readFile2Pos(d,all_instr,runFlag,delayTime);
}

/**
 * @brief readFile2Pos
 * Reads a csv file containing position data and parses the information to the motor system
 * @param d Reference to the motor system
 * @param filename File name of the csv file
 * @param runFlag Reference to a boolean, only continues reading the file when true - optional
 * @param delayTime Delay time between setting each timestep (row) - optional, default to 4
 * @param hasLeftHeader Flag to denote the presence of a left header (i.e. ignore first column)
 * @param hasTopHeader Flag to denote the presence of a top header (i.e. ignore first row)
 * @return Status flag - 0 upon exit without issues
 */
int readFile2Pos(DXMotorSystem& d, const string& filename, bool& runFlag, int delayTime, bool hasLeftHeader, bool hasTopHeader)
{
    return readFile2Word(d,filename,P_GOAL_POSITION_L,runFlag,delayTime, hasLeftHeader, hasTopHeader);
}

/**
 * @brief readFile2Pos
 * Reads a csv file containing position data and parses the information to the motor system
 * @param d Reference to the motor system
 * @param all_instr Instructions parsed from the csv file
 * @param runFlag Reference to a boolean, only continues reading the file when true - optional
 * @param delayTime Delay time between setting each timestep (row) - optional, default to 4
 * @return Status flag - 0 upon exit without issues
 */
int readFile2Pos(DXMotorSystem& d, queue<vector<int> >& all_instr, bool& runFlag, int delayTime)
{
    return readFile2Word(d,all_instr,P_GOAL_POSITION_L,runFlag,delayTime);
}

/**
 * @brief readFile2Pos
 * Reads a csv file containing position and speed data and parses the information to the motor system
 * @param d Reference to the motor system
 * @param filename File name of the csv file
 * @param delayTime Delay time between setting each timestep (row) - optional, default to 4
 * @param hasLeftHeader Flag to denote the presence of a left header (i.e. ignore first column)
 * @param hasTopHeader Flag to denote the presence of a top header (i.e. ignore first row)
 * @return Status flag - 0 upon exit without issues
 */
int readFile2PosSpd(DXMotorSystem& d, const string& filename, int delayTime, bool hasLeftHeader, bool hasTopHeader)
{
    bool runFlag = true;
    return readFile2PosSpd(d,filename,runFlag,delayTime, hasLeftHeader, hasTopHeader);
}

/**
 * @brief readFile2Pos
 * Reads a csv file containing position and speed data and parses the information to the motor system
 * @param d Reference to the motor system
 * @param all_instr Instructions parsed from the csv file
 * @param delayTime Delay time between setting each timestep (row) - optional, default to 4
 * @return Status flag - 0 upon exit without issues
 */
int readFile2PosSpd(DXMotorSystem& d, queue<vector<int> >& all_instr, int delayTime)
{
    bool runFlag = true;
    return readFile2PosSpd(d,all_instr,runFlag,delayTime);
}

/**
 * @brief readFile2Pos
 * Reads a csv file containing position and speed data and parses the information to the motor system
 * @param d Reference to the motor system
 * @param filename File name of the csv file
 * @param runFlag Reference to a boolean, only continues reading the file when true - optional
 * @param delayTime Delay time between setting each timestep (row) - optional, default to 4
 * @param hasLeftHeader Flag to denote the presence of a left header (i.e. ignore first column)
 * @param hasTopHeader Flag to denote the presence of a top header (i.e. ignore first row)
 * @return Status flag - 0 upon exit without issues
 */
int readFile2PosSpd(DXMotorSystem& d, const string& filename, bool& runFlag, int delayTime, bool hasLeftHeader, bool hasTopHeader)
{
    queue<vector<int> > all_instr;
    if (getInstructions(all_instr, filename, hasLeftHeader, hasTopHeader))
        return 1;
    return readFile2PosSpd(d, all_instr, runFlag, delayTime);
}

/**
 * @brief readFile2Pos
 * Reads a csv file containing position and speed data and parses the information to the motor system
 * @param d Reference to the motor system
 * @param all_instr Instructions parsed from the csv file
 * @param runFlag Reference to a boolean, only continues reading the file when true - optional
 * @param delayTime Delay time between setting each timestep (row) - optional, default to 4
 * @return Status flag - 0 upon exit without issues
 */
int readFile2PosSpd(DXMotorSystem& d, queue<vector<int> >& all_instr, bool& runFlag, int delayTime)
{
    if (all_instr.front().size() != 2*d.getNumMotors())
    {
        cout << "Number of columns in instructions does not tally with number of motors. Continue?";
        if(!getYorN())
            return 1;
    }
    int numMotors = d.getNumMotors();
    int numRow = all_instr.size();
    for (int ii = 0; ii < numRow && runFlag; ii++)
    {
        vector<int> data = all_instr.front();
        all_instr.pop();
        all_instr.push(data);
        vector<int> vPos, vSpd;
        for (int jj = 0; jj < numMotors; jj++)
        {
            vPos.push_back(data[2*jj]);
            vSpd.push_back(data[2*jj+1]);
        }
        d.setAllSpeed(vSpd);
        d.setAllPosition(vPos);
        timestepDelay(d, delayTime);
    }
    return 0;
}


/**
 * @brief readFilePosSpdPID
 * Reads a csv file containing position, speed and PID and parses the information to the motor system
 * @param d Reference to the motor system
 * @param filename File name of the csv file
 * @param delayTime Delay time between setting each timestep (row) - optional, default to 4
 * @param hasLeftHeader Flag to denote the presence of a left header (i.e. ignore first column)
 * @param hasTopHeader Flag to denote the presence of a top header (i.e. ignore first row)
 * @return Status flag - 0 upon exit without issues
 */
int readFile2PosSpdPID(DXMotorSystem& d, const string& filename, int delayTime, bool hasLeftHeader, bool hasTopHeader)
{
    bool runFlag = true;
    return readFile2PosSpdPID(d,filename,runFlag,delayTime, hasLeftHeader, hasTopHeader);
}

/**
 * @brief readFilePosSpdPID
 * Reads a csv file containing position, speed and PID and parses the information to the motor system
 * @param d Reference to the motor system
 * @param all_instr Instructions parsed from the csv file
 * @param delayTime Delay time between setting each timestep (row) - optional, default to 4
 * @return Status flag - 0 upon exit without issues
 */
int readFile2PosSpdPID(DXMotorSystem& d, queue<vector<int> >& all_instr, int delayTime)
{
    bool runFlag = true;
    return readFile2PosSpdPID(d,all_instr,runFlag,delayTime);
}

/**
 * @brief readFilePosSpdPID
 * Reads a csv file containing position, speed and PID and parses the information to the motor system
 * @param d Reference to the motor system
 * @param filename File name of the csv file
 * @param runFlag Reference to a boolean, only continues reading the file when true - optional
 * @param delayTime Delay time between setting each timestep (row) - optional, default to 4
 * @param hasLeftHeader Flag to denote the presence of a left header (i.e. ignore first column)
 * @param hasTopHeader Flag to denote the presence of a top header (i.e. ignore first row)
 * @return Status flag - 0 upon exit without issues
 */
int readFile2PosSpdPID(DXMotorSystem& d, const string& filename, bool& runFlag, int delayTime, bool hasLeftHeader, bool hasTopHeader)
{
    queue<vector<int> > all_instr;
    if (getInstructions(all_instr, filename, hasLeftHeader, hasTopHeader))
        return 1;
    return readFile2PosSpdPID(d,all_instr, runFlag, delayTime);
}

/**
 * @brief readFilePosSpdPID
 * Reads a csv file containing position, speed and PID and parses the information to the motor system
 * @param d Reference to the motor system
 * @param all_instr Instructions parsed from the csv file
 * @param runFlag Reference to a boolean, only continues reading the file when true - optional
 * @param delayTime Delay time between setting each timestep (row) - optional, default to 4
 * @return Status flag - 0 upon exit without issues
 */
int readFile2PosSpdPID(DXMotorSystem& d, queue<vector<int> >& all_instr, bool& runFlag, int delayTime)
{
    if (all_instr.front().size() != 5*d.getNumMotors())
    {
        cout << "Number of columns in instructions does not tally with number of motors. Continue?";
        if(!getYorN())
            return 1;
    }
    int numRow = all_instr.size();
    int numMotors = d.getNumMotors();

    for (int ii = 0; ii < numRow && runFlag; ii++)
    {
        vector<int> instr = all_instr.front();
        all_instr.pop();
        all_instr.push(instr);
        vector<int> vPos, vSpd;

        vector<int> vP, vI, vD;

        for (int jj = 0; jj < numMotors; jj++)
        {
            vSpd.push_back(instr[jj*5]);
            vPos.push_back(instr[jj*5+1]);
            vP.push_back(instr[jj*5+2]);
            vI.push_back(instr[jj*5+3]);
            vD.push_back(instr[jj*5+4]);
        }
        d.setAllSpeed(vSpd);
        d.setAllGainP(vP);
        d.setAllGainI(vI);
        d.setAllGainD(vD);
        d.setAllPosition(vPos);
        timestepDelay(d, delayTime);
    }

    return 0;
}

/**
 * @brief getInstructions
 * Parses a csv file into a 2D vector of instructions (in strings)
 * @param all_instr Output, reference to a queue of a vector<int>/vector<double containing instructions (data values) - the output type (int/double) depends on the type of the first argument (queue<vector<int/double> >)
 * @param filename Input filename of the CSV file
 * @param hasLeftHeader Flag to denote the presence of a left header (i.e. ignore first column)
 * @param hasTopHeader Flag to denote the presence of a top header (i.e. ignore first row)
 * @return Status flag - 0 upon exit without issues
 */
int getInstructions(queue<vector<int> >& all_instr, const string& filename, bool hasLeftHeader, bool hasTopHeader)
{
    try{
    ifstream csvFile(filename.c_str());
    if (!csvFile.is_open()) {
        throw runtime_error("File could not be opened");
        return 1;
    }
    string s;
    if (hasTopHeader)
        getline(csvFile,s); // ignore first line
    while (getline(csvFile,s)) {
        istringstream strStream(s);
        vector<int> info;
        string s2;
        int val;
        if (hasLeftHeader)
            getline(strStream, s2, ','); // ignore first column of cells
        while(getline(strStream, s2, ',')) {
            val = str2int(s2);
            info.push_back(val);
        }
        all_instr.push(info);
    }
    csvFile.close();
    return 0;
    } catch(const runtime_error& e) {
        cout << e.what() << endl;
        return 1;
    }

    return 1;
}

int getInstructions(queue<vector<double> >& all_instr, const string& filename, bool hasLeftHeader, bool hasTopHeader)
{
    try{
    ifstream csvFile(filename.c_str());
    if (!csvFile.is_open()) {
        throw runtime_error("File could not be opened");
        return 1;
    }
    string s;
    if (hasTopHeader)
        getline(csvFile,s); // ignore first line
    while (getline(csvFile,s)) {
        istringstream strStream(s);
        vector<double> info;
        string s2;
        double val;
        if (hasLeftHeader)
            getline(strStream, s2, ','); // ignore first column of cells
        while(getline(strStream, s2, ',')) {
            istringstream strStream2(s2);
            strStream2 >> val;
            info.push_back(val);
        }
        all_instr.push(info);
    }
    csvFile.close();
    return 0;
    } catch(const runtime_error& e) {
        cout << e.what() << endl;
        return 1;
    }

    return 1;
}

/**
 * @brief loopReading
 * Loops the file-to-position function, function helper for File-Reading GUI
 * @param d Reference to the motor system
 * @param filename File name of the csv file
 * @param all_instr Instructions parsed from the csv file
 * @param runFlag Reference to a boolean, only loops while true - optional
 * @param delayTime Delay time between setting each timestep (row)
 * @param loop Boolean governing if the function should loop over file or only run through once
 */
void loopReading(DXMotorSystem& d, queue<vector<int> >& allInstr, bool& runFlag, int delayTime, bool loop)
{
    do
    {
        readFile2Pos(d,allInstr,runFlag,delayTime);
    } while(runFlag && loop);
}

/**
 * @brief writeHomePositions
 * Writes the home positions of the system to a csv file
 * @param d Reference to the motor system
 * @param homeFile File name of the csv file to be written - optional, default "data/homepositions.csv"
 */
void writeHomePositions(DXMotorSystem& d, const string& homeFile)
{
    d.setAllHomePosition(d.getAllPosition());
    vector<int> homePositions = d.getAllHomePosition();


    ostringstream homePosStream;

    homePosStream << "Home Positions";
    for (unsigned int ii = 0; ii < d.getNumMotors(); ii++) {
        homePosStream << "," << homePositions[ii];
    }

    ofstream homeFileStream (homeFile);

    homeFileStream << homePosStream.str() << endl;
    homeFileStream.close();
    cout << "Home position file written" << endl;

}

/**
 * @brief readHomePositions
 * Reads and sets the home positions of the system from a csv file
 * @param d Reference to the motor system
 * @param homeFile File name of the csv file to be written - optional, default "data/homepositions.csv"
 */
int readHomePositions(DXMotorSystem& d, const string& homeFile)
{
    try{
        ifstream csvFile(homeFile.c_str());
        if (!csvFile.is_open()) {
            throw runtime_error("File could not be opened");
            return 1;
        }
        string s;
        getline(csvFile,s);

        istringstream strStream(s);
        vector<int> homePos;
        string s2;
        int val;
        getline(strStream, s2, ','); // ignore first column of cells
        while(getline(strStream, s2, ',')) {
            val = str2int(s2);
            homePos.push_back(val);
        }

        d.setAllHomePosition(homePos);
        return 0;
        csvFile.close();
    } catch(const runtime_error& e) {
        cout << e.what() << endl;
        return 1;
    }
    return 1;
}

