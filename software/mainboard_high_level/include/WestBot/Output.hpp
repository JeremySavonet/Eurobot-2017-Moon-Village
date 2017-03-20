// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_OUTPUT_HPP_
#define WESTBOT_OUTPUT_HPP_

#include <QObject>
#include <QString>

#include "Common.hpp"
#include "ItemRegister.hpp"

namespace WestBot {

/*!
 * \brief The Output class allow to create a proxy of a FPGA
 *        output. It use the layer 1 registers mapping.
 */
class Output : public QObject
{
    Q_OBJECT

public:
    /*!
     * \brief List of available Output type.
     */
    enum class OutputType
    {
        Led,
        IO1,
        IO2,
        IO3
    };

    /*!
     * \brief Constructor of Output.
     * \param outputRegister A reference to the output register.
     * \param type Type of the output object.
     * \param name Name of the output object.
     */
    Output( const ItemRegister& outputRegister,
            OutputType type,
            const QString& name );
    ~Output() override = default;

    /*!
     * \brief Return the name of the output object.
     * \return Return QString.
     */
    const QString& name() const;

    /*!
     * \brief Write digital value on IO.
     * \param val The value to be written.
     */
    void digitalWrite( DigitalValue val );
    DigitalValue digitalRead();

private:
    ItemRegister _outputRegister;
    OutputType _type;
    QString _name;
    DigitalValue _digitalValue;
};

}

#endif // WESTBOT_OUTPUT_HPP_
