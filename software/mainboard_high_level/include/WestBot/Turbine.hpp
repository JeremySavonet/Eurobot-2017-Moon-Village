// Copyright (c) 2017 All Rights Reserved WestBot

#ifndef WESTBOT_TURBINE_HPP_
#define WESTBOT_TURBINE_HPP_

#include <QString>

#include "Hal.hpp"
#include "ItemRegister.hpp"

namespace WestBot {

class Turbine
{
public:
    Turbine( const QString& name );

    bool attach( Hal& hal );

    void reset();

    const QString& name() const;

    void enable( bool enable );

    void setValue( int16_t value );
    int16_t value() const;

private:
    const QString _name;

    ItemRegister::Ptr _escOverride;
    ItemRegister::Ptr _escEnable;
    ItemRegister::Ptr _escValue;

    int16_t _value;

};

}

#endif // WESTBOT_TURBINE_HPP_
