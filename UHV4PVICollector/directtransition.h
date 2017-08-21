#ifndef DIRECTTRANSITION_H
#define DIRECTTRANSITION_H

#include <QSignalTransition>
#include <QAbstractState>
#include <QStateMachine>
#include "uhv4pvicollectordb.h"

class directTransition : public QSignalTransition
{
public:
    directTransition(UHV4PVICollectorDB * database, QAbstractState * destinationState);
protected:
    bool eventTest(QEvent *e) override;
};

#endif // DIRECTTRANSITION_H
