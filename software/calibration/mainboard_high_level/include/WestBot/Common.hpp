// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_COMMON_HPP_
#define WESTBOT_COMMON_HPP_

#include <QDebug>

namespace WestBot
{
    /*!
     * \brief List of available digital value.
     */
    enum class DigitalValue
    {
        OFF = 0,
        ON
    };

    /*!
    * \brief List of available color.
    */
    enum class Color
    {
        Unknown,
        Red,
        Blue
    };

    /*!
    * \brief Overload stream operator for color enum class.
    */
    inline QDebug operator<<( QDebug debug, const Color& color )
    {
        switch( color )
        {
        case Color::Unknown:
            debug << "Unknown";
            break;

        case Color::Red:
            debug << "Red";
            break;

        case Color::Blue:
            debug << "Blue";
            break;
        }
        return debug;
    }

    /*!
    * \brief Overload stream operator for digital value enum class.
    */
    inline QDebug operator<<( QDebug debug, const DigitalValue& value )
    {
        switch( value )
        {
        case DigitalValue::OFF:
            debug << "OFF";
            break;

        case DigitalValue::ON:
            debug << "ON";
            break;
        }
        return debug;
    }
}

#endif // WESTBOT_COMMON_HPP_

