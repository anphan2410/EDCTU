#include "connectdevice.h"

connectDevice::connectDevice(CanBusWorkerDB *database) :
    dbPtr(database)
{
    anIf(CanBusWorkerDBDbgEn, anAck("Construct A New State !"));
}

void connectDevice::onEntry(QEvent *)
{
    anIf(CanBusWorkerDBDbgEn, anAck("Enter State ..."));
    if (dbPtr->currentDev->connectDevice())
    {
        anIf(CanBusWorkerDBDbgEn, anInfo("Successfully Connect Device !"));
    }
    else
    {
        anIf(CanBusWorkerDBDbgEn, anInfo("Failed To Connect Device !"));
    }
}
