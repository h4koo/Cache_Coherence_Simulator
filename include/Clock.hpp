#include <iostream>
#include <string>

#ifndef CLK
#define CLK

namespace simulationcomputer
{
class Clock
{
private:
    int _freq;

public:
    void tick();
};

} // namespace simulationcomputer

#endif