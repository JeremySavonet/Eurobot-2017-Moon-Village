// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_OUTPUT_HPP_
#define WESTBOT_OUTPUT_HPP_

#include <memory>

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
    using Ptr = std::shared_ptr< Output >;

    /*!
     * \brief Constructor of Output.
     * \param outputRegister A reference to the output register.
     * \param name Name of the output object.
     */
    Output( const ItemRegister::Ptr& outputRegister, const QString& name );
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
    ItemRegister::Ptr _outputRegister;
    QString _name;
    DigitalValue _digitalValue;
};

}

#endif // WESTBOT_OUTPUT_HPP_
