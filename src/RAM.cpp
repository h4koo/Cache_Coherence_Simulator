#include "../include/RAM.hpp"

namespace simulationcomputer
{

//default contructor
RAM::RAM() : clk(RAM_TIME)
{
    for (int i = 0; i < RAM_SIZE; ++i)
        _data[i] = "0";
}

/**
     * @brief Looks for the data from the internal memory, if address is present and valid in the cache a cache_hit(true) is returned, otherwise a cache miss(false) is returned
     * 
     * @param address The address we want to read the data from
     */
void RAM::readData(int address)
{
    if (address > 0 && address < RAM_SIZE)
    {
        _bus->setData(_data[address]);
        clk.tick();
    }
}

/**
     * @brief Writes data specified in _data to the address specified in _address
     * 
     * @return true 
     * @return false 
     */
void RAM::writeData(int address, std::string data)
{

    if (address > 0 && address < RAM_SIZE)
    {
        _data[address] = data;
    }
    clk.tick();
}

//method call when there is a request by the bus
void RAM::onNotify(MemPortEvent event)
{
    switch (event)
    {
    case WR_DATA:
        writeData(_bus->getAddress(), _bus->getData());
        break;

    case RD_DATA:
        readData(_bus->getAddress());
        break;
    }
}
} // namespace simulationcomputer