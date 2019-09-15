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
    WAITING_WB,
    RDY
};
enum CPUrequestType
{
    Rd,
    Wr
};

typedef struct
{
    CPUrequestType req;
    int address;
    std::string data;
} CPUrequest;

class CacheController : public simulationcomputer::Observer
{
private:
    CacheControllerStatus _status; //current status action of the controller

    CPUrequest _curr_request;
    Cache _cache;
    int _cpu_address_line;
    int _cache_address_line;
    simulationcomputer::CpuPort *_bus_port;

    std::string _cpu_data_line;

    bool tryReadCacheData(int address);

    bool tryWriteCacheData();

    void memWriteback(int address, std::string data);

    void cpuReadMiss(int address);
    void cpuWriteMiss(int address);
    void cpuWriteInvalidate(int address);

public:
    CacheController();
    CacheController(simulationcomputer::CpuPort *bus_port);

    CacheControllerStatus getStatus();

    std::string getCpuDataLine()
    {
        _status = BSY;
        return _cpu_data_line;
    };

    void connectBusPort(simulationcomputer::CpuPort *bus_port) { _bus_port = bus_port; };

    void cpuWrite(int address, std::string data);

    void cpuRead(int address);

    //checks the operation in the bus, acts accordingly and acknowledges the bus
    void snoop();

    void onNotify(simulationcomputer::CpuPortEvent event);
};

} // namespace cpucore

#endif