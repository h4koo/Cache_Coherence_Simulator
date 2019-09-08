#include <iostream>
#include <string>

#ifndef BUS_PORT
#define BUS_PORT

namespace simulationcomputer
{
enum CpuPortEvent
{
    ADD_REQUEST,
    ACKNOWLEDGE,
    DATA_CACHED,
    BUS_REQUEST,
    REQUEST_READY
};
class Observer
{
public:
    virtual ~Observer() {}
    virtual void onNotify(CpuPortEvent event) = 0;
};

enum BusRequestType
{
    FLUSH,
    RdMiss,
    WrMiss
};

enum MemPortEvent
{
    WR_DATA,
    RD_DATA
};

class BusPort
{
    Observer *_bus;
    BusRequestType _current_bus_request;
    int _address_line;
    std::string _data_line;

public:
    BusPort(Observer *bus) { _bus = bus; };
    void addBus(Observer *bus) { _bus = bus; };

    void setAddress(int address);
    int getAddress();

    void setData(std::string data);
    std::string getData();

    void setCurrentBusRequest(BusRequestType request);
    BusRequestType getCurrentBusRequest();

    void notifyBus(CpuPortEvent response);
};

class CpuPort : BusPort
{

    Observer *_cpu;
    CpuPortEvent _bus_response_line;
    CpuPortEvent _cpu_response_line;

    BusRequestType _cpu_request_type;

public:
    CpuPort();

    void setCPUResponse(CpuPortEvent response);
    void getCPUResponse();

    void setBusResponse(CpuPortEvent response);
    void getBusResponse();

    void setCPURequest(BusRequestType request);
    void getCPURequest();

    //this method notifies the cache controller to snoop on the bus because there is an action
    void notifyCPU(CpuPortEvent response);

    //used by Cache controller to add a request to the bus
    void sendRequest();

    //returns the current request being serviced in the bus
    BusRequestType readBusRequest();
};

class RAMPort : BusPort
{

    void readMem();

    void writeMem();
};

} // namespace simulationcomputer

#endif