#include "wait4p.h"

wait4P::wait4P(UHV2PVICollectorDB *database)
{
    anIf(UHV2PVICollectorStateDbgEn, anTrk("Construct Object"));
    timer.setParent(this);
    timer.setInterval(database->waitIntervalMSecs);
    timer.setSingleShot(true);
    QObject::connect(&timer, &QTimer::timeout, this, [database](){
        anIf(UHV2PVICollectorStateDbgEn, anWarn("Wait For P Timed Out !"));
        database->saveP();
    });
}

void wait4P::onEntry(QEvent *)
{
    anIf(UHV2PVICollectorStateDbgEn, anTrk("Enter State"));
    timer.start();
}

void wait4P::onExit(QEvent *)
{
    anIf(UHV2PVICollectorStateDbgEn, anTrk("Leave State"));
    timer.stop();
}
