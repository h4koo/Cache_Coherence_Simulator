#include <iostream>
#include <string>

namespace CPU_CORE
{

const int RAM_SIZE = 16;
enum RAM_STATUS
{
    READY,
    WAITING,
    WORKING
};

class RAM
{
private:
    std::string _status;
    std::string _data[RAM_SIZE]; //this is the memory of the RAM
    int _address_line;
    std::string _data_line;

public:
    RAM()
    {
        //_data = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    }

    /**
     * @brief Looks for the data from the internal memory, if address is present and valid in the cache a cache_hit(true) is returned, otherwise a cache miss(false) is returned
     * 
     * @param address The address we want to read the data from
     */
    void readData(int address)
    {
        if (address > 0 && address < RAM_SIZE)
        {
            _data_line = _data[address];
        }
    }

    /**
     * @brief Writes data specified in _data to the address specified in _address
     * 
     * @return true 
     * @return false 
     */
    bool writeData()
    {

        return false;
    }
};

} // namespace CPU_CORE