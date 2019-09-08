#include <iostream>
#include <string>

namespace CPU_CORE
{

const int CACHE_SIZE = 8;
enum CACHE_STATUS
{
    READY,
    WAITING,
    WORKING
};

class Cache
{
private:
    std::string _status;
    std::string _data[CACHE_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0}; //this is the memory of the cache
    bool _valid[CACHE_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0};
    int _tags[CACHE_SIZE];
    int _address_line;
    std::string _data_line;

public:
    Cache()
    {
    }

    /**
     * @brief Looks for the data from the internal memory, if address is present and valid in the cache a cache_hit(true) is returned, otherwise a cache miss(false) is returned
     * 
     * @param address The address
     * @return true If data is encountered in memory _data_line is set to the data (Cache Hit)
     * @return false If data is not encountered (Cache Miss)
     */
    bool try_readData(int address)
    {
        if (_tags[address % CACHE_SIZE] == address)
        {
            _data_line = _data[address % CACHE_SIZE];
        }
        return false;
    }

    /**
     * @brief Tries to write data to the cache, if address is present in the cache it writes the data and returns true, if the address is not present it returns false
     * 
     * @return true 
     * @return false 
     */
    bool try_writeData()
    {

        return false;
    }

    /**
     * @brief Stores data in the cache, sets the valid, tag, and data for the provided address
     * 
     * @param address 
     * @param data 
     */
    void cacheData(int address, std::string data)
    {
    }
};

} // namespace CPU_CORE