#ifndef UHV4PVICOLLECTOR_H
#define UHV4PVICOLLECTOR_H

#define UHV4PVICollectorDbgEn 1

#include <QStateMachine>
#include <QObject>
#include "uhv4pvicollectordb.h"
#include "emitreadp.h"
#include "wait4p.h"
#include "emitreadv.h"
#include "wait4v.h"
#include "emitreadi.h"
#include "wait4i.h"
#include "idle.h"
#include "directtransition.h"

class UHV4PVICollector : public QStateMachine
{
    Q_OBJECT
public:
    UHV4PVICollector(QObject *parent = 0);
signals:
public slots:
    void pause();
    void resume();
    void DataFromPump(const QByteArray &data);
private:
    UHV4PVICollectorDB * currentDb = Q_NULLPTR;
};

#endif // UHV4PVICOLLECTOR_H
