// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_ENTITY_HPP_
#define WESTBOT_ENTITY_HPP_

#include <QObject>
#include <QPoint>

namespace WestBot {

/*!
 * \brief The Entity class represents a moving entity from the game.
 */
class Entity : public QObject
{
    Q_OBJECT

public:
    /*!
     * \brief List of available entity type.
     */
    enum class Type
    {
        NONE,
        ENNEMI,
        MINERAI,
        MODULE_LUNAIRE,
        ROCHE_LUNAIRE
    };

    Entity( int x, int y, Type type = Type::NONE, QObject* parent = nullptr );
    ~Entity() override = default;

    /*!
     * \brief Get entity type.
     *
     * \return Return the type.
     */
    Type type() const;

    /*!
     * \brief Set entity type.
     *
     * \param type The type of the entity.
     */
    void setType( Type type );

    /*!
     * \brief The cost of the entity.
     *        This cost is an relation between the distance and the gain of the
     *        entity.
     *
     * \return Return an int representing the cost of the entity.
     */
    int cost() const;

    /*!
     * \brief Get the position of the entity on the map.
     *
     * \return Return a QPoint representing the position of the entity.
     */
    const QPoint& position() const;

    /*!
     * \brief Set the position of the entity.
     *
     * \param x X pos of the entity.
     * \param y Y pos of the entity.
     */
    void setPosition( int x, int y );

signals:
    void updated();

private:
    Type _type;
    QPoint _position;
};

}
#endif // WESTBOT_ENTITY_HPP_
