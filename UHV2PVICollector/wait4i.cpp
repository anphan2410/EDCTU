#include "wait4i.h"

wait4I::wait4I(UHV2PVICollectorDB *database) : dbPtr(database)
{
    anIf(UHV2PVICollectorStateDbgEn, anTrk("Construct Object"));
    timer.setInterval(database->waitIntervalMSecs);
    timer.setSingleShot(true);
    QObject::connect(&timer, &QTimer::timeout, this, [database](){
        anIf(UHV2PVICollectorStateDbgEn, anWarn("Wait For I Timed Out !"));
        database->saveI();
    });
}

void wait4I::onEntry(QEvent *)
{
    anIf(UHV2PVICollectorStateDbgEn, anTrk("Enter State"));
    timer.start();
}

void wait4I::onExit(QEvent *)
{
    anIf(UHV2PVICollectorStateDbgEn, anTrk("Leave State"));
    timer.stop();
    dbPtr->emitMsgToDatabaseUpdatePVI();
}
