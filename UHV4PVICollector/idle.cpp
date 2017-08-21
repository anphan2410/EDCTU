#include "idle.h"

idle::idle()
{
    anIf(UHV4PVICollectorStateDbgEn, anTrk("Construct Object"));
}

void idle::onEntry(QEvent *)
{
    anIf(UHV4PVICollectorStateDbgEn, anTrk("Enter State"));
}

void idle::onExit(QEvent *)
{
    anIf(UHV4PVICollectorStateDbgEn, anTrk("Leave State"));
}
