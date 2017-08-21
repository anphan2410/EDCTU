#ifndef CANBUSWORKER_H
#define CANBUSWORKER_H

#include <QStateMachine>
#include "canbusworkerdb.h"
#include "waitforpluginandinterface.h"
#include "createdevice.h"
#include "connectdevice.h"
#include "readframe.h"
#include "writeframe.h"
#include "waitforerrorhandler.h"
#include "directtransition.h"
#include "deviceconnected.h"
#include "framesent.h"
#include "commonthings.h"
#include "smallcoordinatordb.h"

class CanBusWorker : public QStateMachine
{
    Q_OBJECT
public:
    explicit CanBusWorker(QObject *parent = 0);
    ~CanBusWorker();
signals:
    void Out(QVariant *, QVariant * = Q_NULLPTR);
    void ToSmallCoordinator(SmallCoordinatorDB::GlobalSignal *);
public slots:
    void In(QVariant * enumVar, QVariant * dataVar = Q_NULLPTR);
    void FromSmallCoordinator(SmallCoordinatorDB::GlobalSignal * aSignal);
private:
    CanBusWorkerDB * currentDB = Q_NULLPTR;
    SmallCoordinatorDB::GlobalSignal * currentSignal;
};

#endif // CANBUSWORKER_H
