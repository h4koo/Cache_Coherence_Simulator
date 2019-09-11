#include <iostream>
#include <time.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#ifndef CLK
#define CLK

namespace simulationcomputer
{
class Clock
{
private:
    int _freq;

public:
    Clock(int freq);
    void tick();
};

} // namespace simulationcomputer

#endif