#include <iostream>
#include <string>
#include <list>

#include "BusPort.hpp"

#ifndef INTERNAL_BUS
#define INTERNAL_BUS

namespace simulationcomputer
{

const int CPU_BUS_PORTS = 4;

typedef struct
{
    BusRequestType request_type;
    int address;
    std::string data;
} BusRequest;

class Bus : public Observer
{
private:
    BusRequest _current_request;
    std::list<BusRequest> _request_queue[CPU_BUS_PORTS]; //this is the queue of requests for control
    CpuPort _cpu_ports[CPU_BUS_PORTS];
    RAMPort _ram_memory;
    int bus_address;
    std::string data_line;

public:
    //sends request to read data to all CPUs and ultimately Memory (RAM)
    void requestReadData();
    //sends a request to write data to Memory
    bool requestWriteData();

    //noifies CPUs to invalidate cache data
    void invalidateCaches();

    //notifies
    void notifyRequestDone();

  
    //adds a request to the queue starts to process requests if there were none queued
    void addRequest();

    //this methods loops through the queue of requests and resolves each of them
    void processRequests();

    void onNotify(CpuPortEvent event);
};

} // namespace simulationcomputer

#endif