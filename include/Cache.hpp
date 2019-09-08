#include <iostream>
#include <string>

#ifndef CACHE
#define CACHE

namespace cpucore
{

const int CACHE_SIZE = 8;

typedef struct
{
    std::string _data;
    bool _valid;
    bool _shared;
    bool _dirty;
    int _tag;
} CacheBlock;

class Cache
{
private:
    CacheBlock _blocks[CACHE_SIZE]; //this is the memory of the cache
    int _address_line;
    std::string _data_line;

    bool dumpToFile();

public:
    Cache();

    // returns cache block at requested address
    CacheBlock readCacheBlock(int address);

    //writes a cache block to the
    void writeCacheBlock(int address, CacheBlock block);
    /**
     * @brief Looks for the data from the internal memory, if address is present and valid in the cache a cache_hit(true) is returned, otherwise a cache miss(false) is returned
     * 
     * @param address The address
     * @return true If data is encountered in memory _data_line is set to the data (Cache Hit)
     * @return false If data is not encountered (Cache Miss)
     */
    bool try_readData(int address);

    /**
     * @brief Tries to write data to the cache, if address is present in the cache it writes the data and returns true, if the address is not present it returns false and sets address_line to the tag of current address in block and data_line to the current data
     * 
     * @return true 
     * @return false 
     */
    bool try_writeData();

    /**
     * @brief Stores data in the cache, sets the valid, tag, and data for the provided address
     * 
     * @param address 
     * @param data 
     */
    void cacheData(int address, std::string data);
};

} // namespace cpucore

#endif