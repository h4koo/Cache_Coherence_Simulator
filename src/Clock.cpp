#include "../include/Clock.hpp"

namespace simulationcomputer
{
Clock::Clock(int frequency)
{
    _freq = frequency * 1000000;
}

/**
 * @brief Sleeps for the amount of time specified in the frequency
 * 
 */
void Clock::tick()
{
    usleep(_freq);
}
} // namespace simulationcomputer