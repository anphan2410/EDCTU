#include "wait4i.h"

wait4I::wait4I(UHV4PVICollectorDB *database) : dbPtr(database)
{
    anIf(UHV4PVICollectorStateDbgEn, anTrk("Construct Object"));
    timer.setInterval(database->waitIntervalMSecs);
    timer.setSingleShot(true);
    QObject::connect(&timer, &QTimer::timeout, this, [database](){
        anIf(UHV4PVICollectorStateDbgEn, anWarn("Wait For I Timed Out !"));
        database->saveI();
    });
}

void wait4I::onEntry(QEvent *)
{
    anIf(UHV4PVICollectorStateDbgEn, anTrk("Enter State"));
    timer.start();
}

void wait4I::onExit(QEvent *)
{
    anIf(UHV4PVICollectorStateDbgEn, anTrk("Leave State"));
    timer.stop();
    dbPtr->emitMsgToDatabaseUpdatePVI();
}
