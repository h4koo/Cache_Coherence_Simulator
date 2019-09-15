#include "../include/Cache.hpp"

namespace cpucore
{

Cache::Cache()
{
}

//outputs the contents of the Cache
void Cache::dumpToFile()
{
    printf("\n The contents of the cache are:\n");
    for (int i = 0; i < CACHE_SIZE; ++i)
    {
        printf("Cache block %d contains data: %s, tag: %d, valid: %d, shared: %d, dirty: %d \n", i, _blocks[i].data.c_str(), _blocks[i].tag, _blocks[i].valid, _blocks[i].shared, _blocks[i].dirty);
    }
}

// returns cache block at requested address
CacheBlock *Cache::readCacheBlock(int address)
{
    return &_blocks[address % CACHE_SIZE];
}

//writes a cache block to the
void Cache::writeCacheBlock(int address, CacheBlock block)
{
}

//writes a cache block to the
void Cache::writeCacheBlock(int address, std::string data, bool shared, bool dirty)
{
    _blocks[address % CACHE_SIZE].data = data;
    _blocks[address % CACHE_SIZE].tag = address;
    _blocks[address % CACHE_SIZE].shared = shared;
    _blocks[address % CACHE_SIZE].dirty = dirty;
    _blocks[address % CACHE_SIZE].valid = true;
}

//invalidate cached data
void Cache::invalidateBlock(int address)
{
    _blocks[address % CACHE_SIZE].valid = false;
}

//returns true if the requested address is stored in the cache and is valid
bool Cache::lookupAddress(int address)
{
    CacheBlock block = _blocks[address % CACHE_SIZE];
    if (block.tag == address && block.valid)
        return true;
    else
        return false;
}

} // namespace cpucore