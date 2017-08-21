#include "wait4v.h"

wait4V::wait4V(UHV4PVICollectorDB *database)
{
    anIf(UHV4PVICollectorStateDbgEn, anTrk("Construct Object"));
    timer.setParent(this);
    timer.setInterval(database->waitIntervalMSecs);
    timer.setSingleShot(true);
    QObject::connect(&timer, &QTimer::timeout, this, [database](){
        anIf(UHV4PVICollectorStateDbgEn, anWarn("Wait For V Timed Out !"));
        database->saveV();
    });
}

void wait4V::onEntry(QEvent *)
{
    anIf(UHV4PVICollectorStateDbgEn, anTrk("Enter State"));
    timer.start();
}

void wait4V::onExit(QEvent *)
{
    anIf(UHV4PVICollectorStateDbgEn, anTrk("Leave State"));
    timer.stop();
}
