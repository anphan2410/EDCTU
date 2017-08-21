#include "framesent.h"

FrameSent::FrameSent(CanBusWorkerDB * database) :
    QSignalTransition(database, &CanBusWorkerDB::FrameWritten),
    dbPtr(database)
{

}

void FrameSent::onTransition(QEvent *)
{
    if (dbPtr->lastFrameTransmitted)
    {
        anIf(CanBusWorkerDBDbgEn, anAck("Successfully Send A Frame !"));
#if QT_VERSION >= QT_VERSION_CHECK(5,8,0)
        anIf(CanBusWorkerDBDbgEn, anInfo("" + dbPtr->lastFrameTransmitted->toString()));
#else
        anIf(CanBusWorkerDBDbgEn, anInfo("" + QString().setNum(dbPtr->lastFrameTransmitted->frameId(), 16)
                 + " | " + dbPtr->lastFrameTransmitted->payload().toHex()));
#endif
        emit dbPtr->Out(new QVariant(QVariant::fromValue(CanBusWorkerDB::CanFrameTransmitted)),
                        new QVariant(QVariant::fromValue(*(dbPtr->lastFrameTransmitted))));
        dbPtr->lastFrameTransmitted = Q_NULLPTR;
    }
}
