// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_INPUT_HPP_
#define WESTBOT_INPUT_HPP_

#include <memory>

#include <QDebug>
#include <QObject>
#include <QString>

#include "ItemRegister.hpp"

class QTimer;

namespace WestBot {

/*!
 * \brief The Input class allow to create a proxy of a FPGA
 *        input. It use the layer 1 registers mapping.
 */
class Input : public QObject
{
    Q_OBJECT

public:
    using Ptr = std::shared_ptr< Input >;

    /*!
     * \brief List of available InputType.
     */
    enum class InputType
    {
        Start,
        Color,
        Stop,
    };

    enum class Value
    {
        OFF = 0,
        ON
    };

    /*!
     * \brief Constructor of Input.
     * \param inputRegister A reference to the input register.
     * \param inputType Type of the input object.
     * \param name Name of the input object.
     */
    Input( const ItemRegister& inputRegister,
           InputType type,
           const QString& name );
    ~Input() override = default;

    /*!
     * \brief Return the name of the input object.
     * \return Return QString.
     */
    const QString& name() const;

    /*!
     * \brief Allow to check if the input is toogled on or off.
     * \return Return a value from the enum state.
     *
     */
    Value digitalRead();

    void check();

signals:
    /*!
     * \brief Notify of a stateChanged off the input.
     */
    void stateChanged( const Value& value );

private:
    ItemRegister _inputRegister;
    InputType _type;
    QString _name;
    Value _digitalValue;
    QTimer* _eventTimer;
};

}

#endif // WESTBOT_INPUT_HPP_
