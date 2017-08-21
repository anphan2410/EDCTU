#include "emitreadi.h"

emitReadI::emitReadI(UHV2PVICollectorDB *database) : dbPtr(database)
{
    anIf(UHV2PVICollectorStateDbgEn, anTrk("Construct Object"));
    timer.setParent(this);
    timer.setInterval(database->breakIntervalMSecs);
    timer.setSingleShot(true);
    QObject::connect(&timer, &QTimer::timeout, this, [database](){
        database->emitReadI();
    });
}

void emitReadI::onEntry(QEvent *)
{
    anIf(UHV2PVICollectorStateDbgEn, anTrk("Enter State"));
    timer.start();
}

void emitReadI::onExit(QEvent *)
{
    anIf(UHV2PVICollectorStateDbgEn, anTrk("Leave State"));
    timer.stop();
    dbPtr->previousReadState = this->objectName();
}
