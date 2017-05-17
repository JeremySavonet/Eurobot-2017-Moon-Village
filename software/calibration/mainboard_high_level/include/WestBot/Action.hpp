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
    Action( const QString& name, QObject* parent = nullptr );
    /*!
    * \brief Destructor
    */
    ~Action() override = default;

    /*!
     * \brief Allow to get the action name.
     * \return Return a QString.
     */
    const QString& name() const;

    /*!
    * \brief Virtual method to be override by daughter class.
    *        The base method allows to execute the action. This will push back
    *        the action on layer 1, 2 or 3 based on the action type.
    */
    virtual void execute();
    /*!
    * \brief Virtual method to be override by daughter class.
    *        The base method allows to skip the current action.
    */
    virtual void skip();

    /*!
    * \brief Get the current state of the action.
    *
    * \return Return an enum state element.
    */
    State state() const;
    /*!
    * \brief Set the state of the action.
    *
    * \param state The new state of the action.
    */
    void setState( State state );

    /*!
    * \brief Check if the action is in error or not.
    *
    * \return If true, the action is in error, else not.
    */
    bool hasError() const;

signals:
    /*!
    * \brief Notify of a state changed.
    */
    void stateChanged();
    /*!
    * \brief Notify of the action completion.
    */
    void complete();
    /*!
    * \brief Notify that the action was skipped.
    */
    void skipped();

private:
    QString _name;
    State _state;
};

}

#endif // WESTBOT_ACTION_HPP_
