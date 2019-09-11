#include "../include/Bus.hpp"

namespace simulationcomputer
{

//sends request to read data to all CPUs and ultimately Memory (RAM)
void Bus::requestReadData(BusRequest request)
{
    CpuPort *port;
    CpuPortEvent event;

    event.port_id = _current_request.port_id;
    event.event_type = BUS_REQUEST;

    CpuPort *request_port = &_cpu_ports[_current_request.port_id];
    for (int i = 0; i < CPU_BUS_PORTS; ++i)
    {
        //don't send a read request to the CPU requesting it
        if (i == _current_request.port_id)
            continue;

        port = &_cpu_ports[i];
        port->setCurrentBusRequest(_current_request.request_type);
        port->setAddress(_current_request.address);
        port->notifyCPU(event);

        if (port->getCpuSnoopResponse() == DATA_CACHED)
        {
            //if the CPU had the data cached

            request_port->setData(port->getData());
            request_port->setAddress(_current_request.address);
            request_port->setCpuRequest(_current_request.request_type);
            request_port->setShared(true);
            notifyRequestDone(request);
            return;
        }
    }

    //if none of the other CPUs had the data, fetch it from memory
    _ram_memory.setAddress(_current_request.address);
    _ram_memory.readMem();

    request_port->setAddress(_current_request.address);
    request_port->setData(_ram_memory.getData());
    request_port->setShared(false);

    notifyRequestDone(request);
}

//sends a request to write data to Memory
void Bus::requestWriteData(BusRequest request)
{
    _ram_memory.setAddress(_current_request.address);
    _ram_memory.setData(_current_request.data);
    _ram_memory.writeMem();
    notifyRequestDone(request);
}

//notifies CPUs to invalidate cache data
void Bus::invalidateCaches(BusRequest request)
{
    CpuPort *port;
    CpuPortEvent event;
    event.port_id = _current_request.port_id;
    event.event_type = BUS_REQUEST;
    for (int i = 0; i < CPU_BUS_PORTS; ++i)
    {
        if (i == _current_request.port_id)
            continue;

        port = &_cpu_ports[i];
        port->setCurrentBusRequest(_current_request.request_type);
        port->setAddress(_current_request.address);
        port->notifyCPU(event);
    }

    //once all other caches have been invalidated, let the port signaling the invalidation it is complete
    notifyRequestDone(request);
}

//notifies the current port that their request is complete
void Bus::notifyRequestDone(BusRequest request)
{
    CpuPort *port = &_cpu_ports[_current_request.port_id];
    CpuPortEvent event;
    event.port_id = _current_request.port_id;
    event.event_type = REQUEST_READY;
    port->setCurrentBusRequest(_current_request.request_type);
    port->notifyCPU(event);
}

//adds a request to the queue starts to process requests if there were none queued
void Bus::addRequest(BusRequest request)
{

#pragma omp critical
    {
        size_t queue_lenght = _request_queue.size();

        //add the req to the back of the queue
        _request_queue.push_back(request);

        // if(queue_lenght==0){//there were no requests on the queue
        // #pragma omp
        // }
    }
}

//this methods loops through the queue of requests and resolves each of them
void Bus::processRequests()
{

    while (true)
    {
        if (_request_queue.size() > 0) //if there are requests in the queue
        {
            _current_request = _request_queue.front();

            switch (_current_request.request_type)
            {
            case WrMiss:
                //send a message to invalidate other caches
                invalidateCaches(_current_request);
                break;
            case WrInvalidate:
                //send a message to invalidate other caches
                invalidateCaches(_current_request);
                break;
            case RdMiss:
                /* code */
                requestReadData(_current_request);
                break;

            case FLUSH:
                /* code */
                requestWriteData(_current_request);
                break;

            default:
                break;
            }

            //remove request from the queue
            _request_queue.pop_front();
        }
        //else no requests to process, just continue
    }
}

void Bus::onNotify(CpuPortEvent event)
{
    switch (event.event_type)
    {
    case ADD_REQUEST:
    {
        BusRequest req;
        CpuPort *current_port = &_cpu_ports[event.port_id];
        req.data = current_port->getData();
        req.address = current_port->getAddress();
        req.request_type = current_port->getCpuRequest();
        req.port_id = event.port_id;
        addRequest(req);
    }
    default:
        break;
    }
}

} // namespace simulationcomputer