#include "emitreadv.h"

emitReadV::emitReadV(UHV4PVICollectorDB *database) : dbPtr(database)
{
    anIf(UHV4PVICollectorStateDbgEn, anTrk("Construct Object"));
    timer.setParent(this);
    timer.setInterval(database->breakIntervalMSecs);
    timer.setSingleShot(true);
    QObject::connect(&timer, &QTimer::timeout, this, [database](){
        database->emitReadV();
    });
}

void emitReadV::onEntry(QEvent *)
{
    anIf(UHV4PVICollectorStateDbgEn, anTrk("Enter State"));
    timer.start();
}

void emitReadV::onExit(QEvent *)
{
    anIf(UHV4PVICollectorStateDbgEn, anTrk("Leave State"));
    timer.stop();
    dbPtr->previousReadState = this->objectName();
}
