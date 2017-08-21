#include "emitreadp.h"

emitReadP::emitReadP(UHV2PVICollectorDB *database) : dbPtr(database)
{
    anIf(UHV2PVICollectorStateDbgEn, anTrk("Construct Object"));
    timer.setParent(this);
    timer.setInterval(database->breakIntervalMSecs);
    timer.setSingleShot(true);
    QObject::connect(&timer, &QTimer::timeout, this, [database](){
        database->emitReadP();
    });
}

void emitReadP::onEntry(QEvent *)
{
    anIf(UHV2PVICollectorStateDbgEn, anTrk("Enter State"));
    timer.start();
}

void emitReadP::onExit(QEvent *)
{
    anIf(UHV2PVICollectorStateDbgEn, anTrk("Leave State"));
    timer.stop();
    dbPtr->previousReadState = this->objectName();
}
