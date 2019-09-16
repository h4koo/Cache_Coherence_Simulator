#include "../include/CacheController.hpp"

using namespace simulationcomputer;

namespace cpucore
{

CacheController::CacheController()
{
}
CacheController::CacheController(simulationcomputer::CpuPort *bus_port)
{
}
bool CacheController::tryReadCacheData(int address)
{

    return false;
}

bool CacheController::tryWriteCacheData()
{

    return false;
}

void CacheController::memWriteback(int address, std::string data)
{
    _status = WAITING_WB;

    _bus_port->setAddress(address);
    _bus_port->setData(data);
    _bus_port->setCpuRequest(FLUSH);
    _bus_port->sendRequest();

    while (_status == WAITING_WB)
    {
        // simply wait for the _status to change back to BSY when the request is served
    }

    //_status shoud be BSY here
    return;
}

/**
 * @brief Returns current status of the CacheController
 * 
 * @return CacheControllerStatus `BSY`/`RDY` for Busy or Ready states
 */
CacheControllerStatus CacheController::getStatus()
{

    return _status;
}

//method called by the cpu on a write to memory
void CacheController::cpuWrite(int address, std::string data)
{
    _status = BSY; //set status to busy
    _cpu_data_line = data;
    _cpu_address_line = address;

    CacheBlock *block = _cache.readCacheBlock(address % CACHE_SIZE);

    if (block->valid)
    {
        if (block->tag == address) //a cache write hit
        {
            if (block->shared) //the block is shared with other caches
            {
                //send request to invalidate other caches
                cpuWriteInvalidate(address);
            }
            else //the block is exclusive, no need to notify the bus
            {
                //write the cache as not shared and dirty
                _cache.writeCacheBlock(address, data, false, true);
                _status = RDY;
            }
        }
        else //cache write miss
        {
            if (block->dirty) //we have to evict the block to memory
                this->memWriteback(block->tag, block->data);
            cpuWriteMiss(address);
        }
    }
    else
    {
        cpuWriteMiss(address);
    }
}

//method called by the CPU on a read to memory
void CacheController::cpuRead(int address)
{
    _status = BSY; //set status to busy
    _cpu_address_line = address;

    CacheBlock *block = _cache.readCacheBlock(address % CACHE_SIZE);
    if (block->valid)
    {
        if (block->tag == address)
        { // cache hit
            _cpu_data_line = block->data;
            _status = RDY;
            return;
        }
        else
        {                     //cache miss
            if (block->dirty) //we have to evict the block to memory
                this->memWriteback(block->tag, block->data);

            cpuReadMiss(address);
        }
    }
    else //cache miss no eviction
    {
        cpuReadMiss(address);
    }
}

/**
 * @brief Sets the values in the ports to add a request for a CPU readMiss on the bus, waits for the response and updates the cache with the returned value
 * 
 * @param address 
 */
void CacheController::cpuReadMiss(int address)
{
    _bus_port->setCpuRequest(RdMiss);
    _bus_port->setAddress(address);
    _bus_port->sendRequest();
}

/**
 * @brief Sets the values in the ports to add a request for a CPU readMiss on the bus, waits for the response and updates the cache with the returned value
 * 
 * @param address 
 */
void CacheController::cpuWriteMiss(int address)
{
    _bus_port->setCpuRequest(WrMiss);
    _bus_port->setAddress(address);
    _bus_port->sendRequest();
}

/**
 * @brief Sets the values in the ports to add a request for a CPU readMiss on the bus, waits for the response and updates the cache with the returned value
 * 
 * @param address 
 */
void CacheController::cpuWriteInvalidate(int address)
{
    _bus_port->setCpuRequest(WrInvalidate);
    _bus_port->setAddress(address);
    _bus_port->sendRequest();
}

//checks the operation in the bus, acts accordingly and acknowledges the bus
void CacheController::snoop()
{
    int address = _bus_port->getAddress();
    if (_cache.lookupAddress(address)) //if the address is in the cache
    {
        CacheBlock *block = _cache.readCacheBlock(address);

        switch (_bus_port->getCurrentBusRequest())
        {
        case WrMiss: //another cache is trying to write the cache block
        {
            //invalidate the block
            block->valid = false;
            // set response to ACK?
            _bus_port->setCpuSnoopResponse(ACKNOWLEDGE);
            break;
        }
        case RdMiss: //another cache is trying to read a block

            //set block as shared and share it
            block->shared = true;

            _bus_port->setData(block->data);
            _bus_port->setCpuSnoopResponse(DATA_CACHED);

            break;

        case WrInvalidate:
            //invalidate copy of the block

            block->valid = false;
            _bus_port->setCpuSnoopResponse(ACKNOWLEDGE);
            break;

        case FLUSH: // we may check to see if copy is the same and update
            break;
        default:
            break;
        }
    }
    //do nothing since we don't have address or it is invalid
}

/**
 * @brief Checks the event on the port and acts accordingly
 * 
 * @param event the event posted by the Bus
 */
void CacheController::onNotify(simulationcomputer::CpuPortEvent event)
{
    switch (event.event_type)
    {
    //in case the Bus notifies of a bus request
    case BUS_REQUEST:
        snoop();
        break;

    //in case the bus notifies that one of our requests is ready
    case REQUEST_READY:
    {
        BusRequestType current_bus_request = this->_bus_port->getCurrentBusRequest();
        //data in port is requested data
        switch (current_bus_request)
        {
        case WrMiss:
        {

            //finalize write to cache
            //write the cache as not shared and dirty
            _cache.writeCacheBlock(_cpu_address_line, _cpu_data_line, false, true);

            //set status to ready so that the CPU knows data has been written
            _status = RDY;
            break;
        }

        case RdMiss:
        { //received a response from a read miss
            //retrieve data from the bus
            _cpu_address_line = _bus_port->getAddress();
            _cpu_data_line = _bus_port->getData();

            //Sava data in the cache
            //if data was supplied by another cache the data is shared
            _bus_port->getShared() ? _cache.writeCacheBlock(_cpu_address_line, _cpu_data_line, true) : _cache.writeCacheBlock(_cpu_address_line, _cpu_data_line, false);

            //set status to ready so that the CPU knows it can read the data
            _status = RDY;
            break;
        }
        case WrInvalidate:

            //when all other caches have been invalidated finalize write to cache
            //write the cache as not shared and dirty
            _cache.writeCacheBlock(_cpu_address_line, _cpu_data_line, false, true);

            //set status to ready so that the CPU knows data has been written
            _status = RDY;
            break;
        case FLUSH:
        {
            if (_status == WAITING_WB)
            {
                _status = BSY;
            }

            break;
        }

        default:
            break;
        }
        break;
    }
    default:
        return;
    }
}

} // namespace cpucore