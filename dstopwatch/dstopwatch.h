#ifndef DSTOPWATCH_H
#define DSTOPWATCH_H

#include <chrono>

class DStopWatch
{
public:
    DStopWatch();
    ~DStopWatch();
    void start();
    long long stop();
    long long lap();

private:
    bool initialized;
    std::chrono::high_resolution_clock::time_point startTime;

};

#endif // DSTOPWATCH_H
