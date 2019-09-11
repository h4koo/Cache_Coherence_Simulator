#include <iostream>
#include <string>
#include "Clock.hpp"
#include "BusPort.hpp"

#ifndef RAM_MEM
#define RAM_MEM

namespace simulationcomputer
{

const int RAM_SIZE = 16;
const int RAM_TIME = 1;

class RAM : public RAMObserver
{
private:
    Clock clk;
    std::string _data[RAM_SIZE]; //this is the memory of the RAM
    RAMPort *_bus;
    // int _address_line;
    // std::string _data_line;

    bool dumpToFile();

public:
    RAM();
    /**
     * @brief Looks for the data from the internal memory, if address is present and valid in the cache a cache_hit(true) is returned, otherwise a cache miss(false) is returned
     * 
     * @param address The address we want to read the data from
     */
    void readData(int address);

    /**
     * @brief Writes data specified in _data to the address specified in _address
     * 
     * 
     */
    void writeData(int address, std::string data);

    // void setAddressLine(int address){_address_line=address;};

    // void setDataLine(std::string data){_data_line=data;};

    // int getAddressLine(){return _address_line;};
    // std::string getDataLine(){return _data_line;};

    void onNotify(MemPortEvent event);
};

} // namespace simulationcomputer

#endif