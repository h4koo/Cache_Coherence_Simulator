#include "../include/BusPort.hpp"

namespace simulationcomputer
{

void BusPort::notifyBus(CpuPortEvent response) {}

// void addBus(Observer *bus) { _bus = bus; };

//----------------------------------------------------------------------------------------------
// CPU Port
//----------------------------------------------------------------------------------------------

void CpuPort::connectCPU(Observer *cpu)
{
    _cpu = cpu;
}

//this method notifies the cache controller to snoop on the bus because there is an action
void CpuPort::notifyCPU(CpuPortEvent response)
{
    _cpu->onNotify(response);
}

//used by Cache controller to add a request to the bus
void CpuPort::sendRequest()
{
    CpuPortEvent event;
    event.port_id = _id;
    event.event_type = ADD_REQUEST;
    _bus->onNotify(event);
}

//----------------------------------------------------------------------------------------------
//RAM Port
//----------------------------------------------------------------------------------------------

void RAMPort::connectRAM(RAMObserver *ram)
{
    _ram = ram;
}

//notifies the RAM of a read operation
void RAMPort::readMem()
{
    _ram->onNotify(RD_DATA);
}

//notifies the RAM about a write operation
void RAMPort ::writeMem()
{
    _ram->onNotify(WR_DATA);
}

} // namespace simulationcomputer
