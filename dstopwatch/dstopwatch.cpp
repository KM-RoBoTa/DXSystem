#include <iostream>

#include "dtools/additionaltools.h"

#include "dstopwatch.h"

using namespace std;

/**
 * @brief DStopWatch::DStopWatch
 * Constructor for DStopWatch
 * (A "stopwatch" class for timing execution time of operations)
 */
DStopWatch::DStopWatch()
{
    initialized = false;
}

/**
 * @brief DStopWatch::~DStopWatch
 * Destructor for DStopWatch
 */
DStopWatch::~DStopWatch()
{

}

/**
 * @brief DStopWatch::start
 * Starts the stopwatch
 */
void DStopWatch::start()
{
    if (initialized)
    {
        cout << "Stopwatch is already initialized" << endl;
        cout << "Restart stopwatch?:";
        if(!getYorN())
            return;
    }
    initialized = true;
    startTime = chrono::high_resolution_clock::now();
}

/**
 * @brief DStopWatch::stop
 * Stops the stopwatch
 * @return Time elapsed
 */
long long DStopWatch::stop()
{
    long long timeElapsed = lap();
    initialized = false;
    return timeElapsed;
}

/**
 * @brief DStopWatch::lap
 * Laps the stopwatch (stopwatch continues running)
 * @return Time elapsed
 */
long long DStopWatch::lap()
{
    chrono::high_resolution_clock::time_point endTime = chrono::high_resolution_clock::now();
    if (!initialized)
    {
        cout << "Stopwatch is not initialized" << endl;
        return 0;
    }
    return chrono::duration_cast<chrono::nanoseconds>(endTime-startTime).count();;
}
