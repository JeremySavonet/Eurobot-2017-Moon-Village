// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_ACTION_HPP_
#define WESTBOT_ACTION_HPP_

#include <memory>

#include <QObject>
#include <QString>

namespace WestBot {

class Action : public QObject
{
    Q_OBJECT

public:
    using Ptr = std::shared_ptr< Action >;

    enum State
    {
        Pending = 0,
        Running,
        Finished,
        InError,
        Flushed
    };

    Action( const QString& name,
            QObject* parent = nullptr );
    ~Action() override = default;

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
