// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_ITEMREGISTER_HPP_
#define WESTBOT_ITEMREGISTER_HPP_

#include "Memory.hpp"

namespace WestBot {

/*!
* \brief This class represents a memory item. Wraps a register from any layer.
*/
class ItemRegister
{
public:
    /*!
    * \brief Construct an ItemRegister.
    *
    * \param layer The parent layer in memory (1,2 or 3).
    * \param offset The offset in the register.
    * \param size The size of the register item.
    */
    ItemRegister( Memory& layer, int offset, int size );

    /*!
    * \brief Write a float value in the register.
    *
    * \param data The float value to be written.
    */
    void writeFloat( float data );
    /*!
    * \brief Write a unsigned value in the register.
    *
    * \param data The uint value to be written.
    */
    void write( uint32_t data );

    /*!
    * \brief Read a unsigned int value in the register.
    *
    * \return Return an unsigned int value.
    */
    uint32_t read() const;
    /*!
    * \brief Read a float value in the register.
    *
    * \return Return a float value.
    */
    float readFloat() const;

private:
    Memory _layer;
    int _offset;
    int _size;
};

}

#endif // WESTBOT_ITEMREGISTER_HPP_
