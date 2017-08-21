#ifndef DIRECTTRANSITION_H
#define DIRECTTRANSITION_H

#include <QSignalTransition>
#include <QAbstractState>
#include <QStateMachine>
#include "uhv2pvicollectordb.h"

class directTransition : public QSignalTransition
{
public:
    directTransition(UHV2PVICollectorDB * database, QAbstractState * destinationState);
protected:
    bool eventTest(QEvent *e) override;
};

#endif // DIRECTTRANSITION_H
