#include <iostream>
#include <string>
#include "Clock.hpp"
#include "BusPort.hpp"

#ifndef RAM_MEM
#define RAM_MEM

namespace simulationcomputer
{

const int RAM_SIZE = 16;

class RAM : public Observer
{
private:
    Clock clk;
    std::string _data[RAM_SIZE]; //this is the memory of the RAM
    RAMPort *_bus;
    int _address_line;
    std::string _data_line;

    bool dumpToFile();

public:
    /**
     * @brief Looks for the data from the internal memory, if address is present and valid in the cache a cache_hit(true) is returned, otherwise a cache miss(false) is returned
     * 
     * @param address The address we want to read the data from
     */
    void readData(int address);

    /**
     * @brief Writes data specified in _data to the address specified in _address
     * 
     * @return true 
     * @return false 
     */
    bool writeData();

    void onNotify();
};

} // namespace simulationcomputer

#endif