// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_MEMORYMANAGER_HPP_
#define WESTBOT_MEMORYMANAGER_HPP_

#include <stdint.h>

namespace WestBot {

/*!
* \brief MemoryManager is a manager which open the memory via a FD and mmap it.
*/
class MemoryManager
{
public:
    /*!
    * \brief Default constructor.
    */
    MemoryManager();
    /*!
    * \brief Desctuctor.
    */
    ~MemoryManager();

    /*!
    * \brief Helper to map a module in the memory.
    *
    * \param moduleBaseAddress
    *
    * \return Return a pointer in the memory where the module is mapped.
    */
    void* mapModuleToMemory( uint32_t moduleBaseAddress );

private:
    /*!
    * \brief Private: Init the module.
    *
    * \return Return an int based on the operation failure.
    */
    int init();
    /*!
    * \brief Private: Deinit the module in memory.
    */
    void deInit();

private:
    void* _lwBridgeMap;
    int _devMemFd;
};

}

#endif // WESTBOT_MEMORYMANAGER_HPP__
