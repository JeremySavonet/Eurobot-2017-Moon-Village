// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_ACTION_HPP_
#define WESTBOT_ACTION_HPP_

#include <memory>

#include <QObject>
#include <QString>

namespace WestBot {

/*!
 * \brief The Action class is a base class of all system actions.
 */
class Action : public QObject
{
    Q_OBJECT

public:
    using Ptr = std::shared_ptr< Action >;

    /*!
     * \brief List of available state of an action.
     */
    enum class State
    {
        Pending = 0,
        Running,
        Finished,
        InError,
        Flushed
    };

    /*!
     * \brief Constructor of Action.
     * \param name Name of the action.
     * \param parent Parent class for lifetime management.
     */
    Action( const QString& name,
            QObject* parent = nullptr );
    ~Action() override = default;

    /*!
     * \brief Allow to get the action name.
     * \return Return a QString.
     */
    const QString& name() const;

    virtual void execute();
    virtual void skip();

    State state() const;
    void setState( State state );

    bool hasError() const;

signals:
    void stateChanged();
    void complete();
    void skipped();

private:
    QString _name;
    State _state;
};

}

#endif // WESTBOT_ACTION_HPP_
