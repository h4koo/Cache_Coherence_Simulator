#include <iostream>
#include <string>
#include <ctime>

class Clock
{
    int _freq;

public:
    Clock(int frequency)
    {
        _freq = frequency;
    }

    /**
 * @brief Sleeps for the amount of time specified in the frequency
 * 
 */
    void tick()
    {
    }
};