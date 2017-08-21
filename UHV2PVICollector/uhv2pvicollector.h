#ifndef UHV2PVICOLLECTOR_H
#define UHV2PVICOLLECTOR_H

#define UHV2PVICollectorDbgEn 1

#include <QStateMachine>
#include <QObject>
#include "uhv2pvicollectordb.h"
#include "emitreadp.h"
#include "wait4p.h"
#include "emitreadv.h"
#include "wait4v.h"
#include "emitreadi.h"
#include "wait4i.h"
#include "idle.h"
#include "directtransition.h"
#include "smallcoordinatordb.h"
#include "pilocaldbworkerdb.h"

class UHV2PVICollector : public QStateMachine
{
    Q_OBJECT
public:
    UHV2PVICollector(QObject *parent = 0);
signals:
    void ToSmallCoordinator(SmallCoordinatorDB::GlobalSignal *);
public slots:
    void pause();
    void resume();
    void DataFromPump(const QByteArray &data);
    void FromSmallCoordinator(SmallCoordinatorDB::GlobalSignal * aSignal);
private:
    UHV2PVICollectorDB * currentDb = Q_NULLPTR;
    SmallCoordinatorDB::GlobalSignal * currentSignal;
    QString currentMsgEnumTypeName;

};

#endif // UHV2PVICOLLECTOR_H
