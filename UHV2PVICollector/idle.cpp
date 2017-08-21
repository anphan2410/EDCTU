#include "idle.h"

idle::idle()
{
    anIf(UHV2PVICollectorStateDbgEn, anTrk("Construct Object"));
}

void idle::onEntry(QEvent *)
{
    anIf(UHV2PVICollectorStateDbgEn, anTrk("Enter State"));
}

void idle::onExit(QEvent *)
{
    anIf(UHV2PVICollectorStateDbgEn, anTrk("Leave State"));
}
