#include "wait4p.h"

wait4P::wait4P(UHV4PVICollectorDB *database)
{
    anIf(UHV4PVICollectorStateDbgEn, anTrk("Construct Object"));
    timer.setParent(this);
    timer.setInterval(database->waitIntervalMSecs);
    timer.setSingleShot(true);
    QObject::connect(&timer, &QTimer::timeout, this, [database](){
        anIf(UHV4PVICollectorStateDbgEn, anWarn("Wait For P Timed Out !"));
        database->saveP();
    });
}

void wait4P::onEntry(QEvent *)
{
    anIf(UHV4PVICollectorStateDbgEn, anTrk("Enter State"));
    timer.start();
}

void wait4P::onExit(QEvent *)
{
    anIf(UHV4PVICollectorStateDbgEn, anTrk("Leave State"));
    timer.stop();
}
