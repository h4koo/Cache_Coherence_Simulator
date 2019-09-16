#include <iostream>
#include <string>
#include <list>
#include <omp.h>
#include <vector>
#include <future>
#include <thread>
#include <mutex>

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
    bool _is_on;
    std::mutex mut;

    BusRequest *_current_request;
    std::list<BusRequest> _request_queue; //this is the queue of requests for control

    std::vector<CpuPort> _cpu_ports;
    int _connected_ports;
    RAMPort _ram_memory;

    int bus_address;
    std::string data_line;

public:
    Bus();

    //sends request to read data to all CPUs and ultimately Memory (RAM)
    void requestReadData();

    //sends a request to write data to Memory
    void requestWriteData();

    //noifies CPUs to invalidate cache data
    void invalidateCaches();

    //notifies
    void notifyRequestDone();

    //adds a request to the queue starts to process requests if there were none queued
    void addRequest(BusRequest request);

    //this methods loops through the queue of requests and resolves each of them
    void processRequests();

    void onNotify(CpuPortEvent event);

    CpuPort *connectCPU(Observer *cpu)
    {

        //  CpuPort port(i, (Observer *)this);
        _cpu_ports[_connected_ports++].connectCPU(cpu);
        return &_cpu_ports[_connected_ports - 1];
    }

    RAMPort *connectRAM(RAMObserver *ram)
    {

        _ram_memory.connectRAM(ram);
        return &_ram_memory;
        //  CpuPort port(i, (Observer *)this);
        // _cpu_ports[_connected_ports++].connectCPU(cpu);
    }

    void start()
    {
        _is_on = true;
        //called as an asynchronous job
        std::thread bus_worker(&Bus::processRequests, this);
        bus_worker.detach();
        // std::async(std::launch::async, &Bus::processRequests, this);
        printf("after bus async started ----------------------------------\n");
    }

    void stop()
    {
        _is_on = false;
    }
};

} // namespace simulationcomputer

#endif