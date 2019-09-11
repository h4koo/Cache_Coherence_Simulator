#include <iostream>
#include <string>
#include <list>
#include <omp.h>

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
    int port_id;
} BusRequest;

class Bus : public Observer
{
private:
    BusRequest _current_request;
    std::list<BusRequest> _request_queue; //this is the queue of requests for control
    CpuPort _cpu_ports[CPU_BUS_PORTS];
    RAMPort _ram_memory;
    int bus_address;
    std::string data_line;

public:
    //sends request to read data to all CPUs and ultimately Memory (RAM)
    void requestReadData(BusRequest request);

    //sends a request to write data to Memory
    void requestWriteData(BusRequest request);

    //noifies CPUs to invalidate cache data
    void invalidateCaches(BusRequest request);

    //notifies
    void notifyRequestDone(BusRequest request);

    //adds a request to the queue starts to process requests if there were none queued
    void addRequest(BusRequest request);

    //this methods loops through the queue of requests and resolves each of them
    void processRequests();

    void onNotify(CpuPortEvent event);
};

} // namespace simulationcomputer

#endif