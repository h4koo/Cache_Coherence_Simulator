#include <iostream>
#include <string>

#ifndef BUS_PORT
#define BUS_PORT

namespace simulationcomputer
{

enum CpuPortEventType
{
    ADD_REQUEST,
    ACKNOWLEDGE,
    DATA_CACHED,
    BUS_REQUEST,
    REQUEST_READY
};
enum BusRequestType
{
    FLUSH,
    RdMiss,
    WrMiss,
    WrInvalidate
};

enum MemPortEvent
{
    WR_DATA,
    RD_DATA
};
typedef struct
{
    CpuPortEventType event_type;
    int port_id;
} CpuPortEvent;

class Observer
{
public:
    virtual ~Observer() {}
    virtual void onNotify(CpuPortEvent event) = 0;
};

class RAMObserver
{
public:
    virtual ~RAMObserver() {}
    virtual void onNotify(MemPortEvent event) = 0;
};

class BusPort
{
protected:
    Observer *_bus;
    int _id;
    BusRequestType _current_bus_request;
    int _address_line;
    std::string _data_line;

    void notifyBus(CpuPortEvent response);

public:
    BusPort(int id, Observer *bus)
    {
        _id = id;
        _bus = bus;
    };

    void addBus(Observer *bus) { _bus = bus; };

    void setAddress(int address) { _address_line = address; };
    int getAddress() { return _address_line; };

    void setData(std::string data) { _data_line = data; };
    std::string getData() { return _data_line; };

    void setCurrentBusRequest(BusRequestType request) { _current_bus_request = request; };
    BusRequestType getCurrentBusRequest() { return _current_bus_request; };
};

class CpuPort : public BusPort
{

    Observer *_cpu;

    BusRequestType _cpu_request_type;

    CpuPortEvent _cpu_snoop_response;
    bool _shared;

public:
    CpuPort();

    void setCpuRequest(BusRequestType request) { _cpu_request_type = request; };
    BusRequestType getCpuRequest() { return _cpu_request_type; }

    void setCpuSnoopResponse(CpuPortEventType event) { _cpu_snoop_response.event_type = event; };
    CpuPortEventType getCpuSnoopResponse() { return _cpu_snoop_response.event_type; };

    bool getShared() { return _shared; };
    void setShared(bool shared) { _shared = shared; };

    void connectCPU(Observer *cpu);

    //this method notifies the cache controller to snoop on the bus because there is an action
    void notifyCPU(CpuPortEvent response);

    //used by Cache controller to add a request to the bus
    void sendRequest();
};

class RAMPort : public BusPort
{
    RAMObserver *_ram;

public:
    void connectRAM(RAMObserver *ram);
    void readMem();

    void writeMem();
};

} // namespace simulationcomputer

#endif