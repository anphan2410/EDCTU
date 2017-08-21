#include "writeframe.h"

writeFrame::writeFrame(CanBusWorkerDB *database) :
    dbPtr(database)
{
    anIf(CanBusWorkerDBDbgEn, anAck("Construct A New State"));
}

void writeFrame::onEntry(QEvent *)
{
    anIf(CanBusWorkerDBDbgEn, anAck("Enter State ..."));
    if (!(dbPtr->pendingFrameList->isEmpty()))
    {
        dbPtr->lastFrameTransmitted = dbPtr->pendingFrameList->takeFirst();
        dbPtr->currentDev->writeFrame(*(dbPtr->lastFrameTransmitted));
    }
}
