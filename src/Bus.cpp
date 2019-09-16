#include "../include/Bus.hpp"

namespace simulationcomputer
{

Bus::Bus() : _ram_memory(0, (Observer *)this)
{

    //create the CPU ports and assign self as an observer
    for (int i = 0; i < CPU_BUS_PORTS; ++i)
    {
        CpuPort port(i, (Observer *)this);
        _cpu_ports.push_back(port);
    }

    _connected_ports = 0;
    // start();
    //initiate thread for processing data
}

//sends request to read data to all CPUs and ultimately Memory (RAM)
void Bus::requestReadData()
{
    CpuPort *port;
    CpuPortEvent event;

    //set the event in the ports to BUS_REQUESTS so they know they need to snoop
    event.port_id = _current_request->port_id;
    event.event_type = BUS_REQUEST;

    CpuPort *request_port = &(_cpu_ports[_current_request->port_id]);

    //for each of the CPU ports
    for (int i = 0; i < CPU_BUS_PORTS; ++i)
    {
        //don't send a read request to the CPU requesting it
        if (i == _current_request->port_id)
            continue;

        //set the current request on the port with the address
        port = &(_cpu_ports[i]);
        port->setCurrentBusRequest(_current_request->request_type);
        port->setAddress(_current_request->address);

        //notifies cachecontrolller to snoop the request in the port
        port->notifyCPU(event);

        //after the cachecontroller snoops and responds

        if (port->getCpuSnoopResponse() == DATA_CACHED) //if the CPU had the data cached
        {
            request_port->setData(port->getData());
            request_port->setAddress(_current_request->address);
            request_port->setCpuRequest(_current_request->request_type);
            request_port->setShared(true);
            notifyRequestDone();
            return;
        }
    }

    //if none of the other CPUs had the data, fetch it from memory
    _ram_memory.setAddress(_current_request->address);
    _ram_memory.readMem();

    request_port->setAddress(_current_request->address);
    request_port->setData(_ram_memory.getData());
    request_port->setShared(false);

    notifyRequestDone();
}

//sends a request to write data to Memory
void Bus::requestWriteData()
{
    _ram_memory.setAddress(_current_request->address);
    _ram_memory.setData(_current_request->data);
    _ram_memory.writeMem();
    notifyRequestDone();
}

//notifies CPUs to invalidate cache data
void Bus::invalidateCaches()
{
    CpuPort *port;
    CpuPortEvent event;
    event.port_id = _current_request->port_id;
    event.event_type = BUS_REQUEST;
    for (int i = 0; i < CPU_BUS_PORTS; ++i)
    {
        if (i == _current_request->port_id)
            continue;

        port = &(_cpu_ports[i]);
        port->setCurrentBusRequest(_current_request->request_type);
        port->setAddress(_current_request->address);
        port->notifyCPU(event);
    }

    //once all other caches have been invalidated, let the port signaling the invalidation it is complete
    notifyRequestDone();
}

//notifies the current port that their request is complete
void Bus::notifyRequestDone()
{
    CpuPort *port = &(_cpu_ports[_current_request->port_id]);
    CpuPortEvent event;
    event.port_id = _current_request->port_id;
    event.event_type = REQUEST_READY;
    port->setCurrentBusRequest(_current_request->request_type);
    port->notifyCPU(event);
}

//adds a request to the queue
void Bus::addRequest(BusRequest request)
{

    std::lock_guard<std::mutex> lock(mut);
    _request_queue.push_back(request);
}

//this method loops through the queue of requests and resolves each of them
void Bus::processRequests()
{

    while (_is_on)
    {
        // printf("processing requests in the bus\n");
        if (_request_queue.size() > 0) //if there are requests in the queue
        {
            _current_request = &_request_queue.front();

            switch (_current_request->request_type)
            {
            case WrMiss:
                //send a message to invalidate other caches
                invalidateCaches();
                break;
            case WrInvalidate:
                //send a message to invalidate other caches
                invalidateCaches();
                break;
            case RdMiss:
                /* code */
                requestReadData();
                break;

            case FLUSH:
                /* code */
                requestWriteData();
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
        CpuPort *current_port = &(_cpu_ports[event.port_id]);
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