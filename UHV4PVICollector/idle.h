#ifndef IDLE_H
#define IDLE_H

#include <QState>
#include "uhv4pvicollectordb.h"

class idle : public QState
{
public:
    idle();
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
};

#endif // IDLE_H
