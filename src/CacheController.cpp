#include "../include/CacheController.hpp"

namespace cpucore
{

void CacheController::onNotify(simulationcomputer::CpuPortEvent event)
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

void CacheController::memWriteback()
{
}

CacheControllerStatus CacheController::getStatus()
{
}

void CacheController::cpuWrite(int address, std::string data)
{
}

void CacheController::cpuRead(int address)
{
}

//checks the operation in the bus, acts accordingly and acknowledges the bus
void CacheController::snoop()
{
}
} // namespace cpucore