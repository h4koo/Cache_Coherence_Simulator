#include <iostream>
#include <string>
#include "Cache.hpp"
#include "BusPort.hpp"

#ifndef CACHE_CONTROLLER
#define CACHE_CONTROLLER

namespace cpucore
{
enum CacheControllerStatus
{
    BSY,
    RDY
};

class CacheController : public simulationcomputer::Observer
{
private:
    CacheControllerStatus _status; //current status action of the controller
    int _cpu_address_line;
    int _cache_address_line;
    simulationcomputer::CpuPort *_bus_port;

    std::string _cpu_data_line;

    bool tryReadCacheData(int address);

    bool tryWriteCacheData();

    void memWriteback();

    void onNotify(simulationcomputer::CpuPortEvent event);

public:
    CacheControllerStatus getStatus();

    void connectBusPort(simulationcomputer::CpuPort *bus_port) { _bus_port = bus_port; };

    void cpuWrite(int address, std::string data);

    void cpuRead(int address);

    //checks the operation in the bus, acts accordingly and acknowledges the bus
    void snoop();
};

} // namespace cpucore

#endif