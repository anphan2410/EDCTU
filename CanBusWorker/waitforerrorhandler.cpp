#include "waitforerrorhandler.h"

waitForErrorHandler::waitForErrorHandler(CanBusWorkerDB *database, quint32 TimerIntervalInMiliSecond) :
    TimerIntervalMSecs(TimerIntervalInMiliSecond),
    dbPtr(database)
{
    anIf(CanBusWorkerDBDbgEn, anAck("Construct A New State !"));
    if (TimerIntervalInMiliSecond > 0)
    {
        timer.setParent(this);
        timer.setInterval(TimerIntervalInMiliSecond);
        QObject::connect(&timer, &QTimer::timeout,
                         this, [database](){
            anIf(CanBusWorkerDBDbgEn, anAck("Emit <currentCanBusWorkerDB>->ErrorType"));
            anIf(CanBusWorkerDBDbgEn, anInfo("ErrorType : " +
                     QString(CanBusWorkerDB::ErrorMetaEnum.valueToKey(static_cast<int>(*(database->ErrorType))))));
            anIf(CanBusWorkerDBDbgEn, anInfo("ErrorInfo : " + *(database->ErrorInfo)));
            emit database->Out(new QVariant(QVariant::fromValue(*(database->ErrorType))));
        }, CanBusWorkerDB::uniqueQtConnectionType);
    }
}

void waitForErrorHandler::onEntry(QEvent *)
{
    anIf(CanBusWorkerDBDbgEn, anAck("Enter State ..."));
    if (TimerIntervalMSecs > 0)
        timer.start();
}

void waitForErrorHandler::onExit(QEvent *)
{
    if (TimerIntervalMSecs > 0)
        timer.stop();
    dbPtr->clearError();
}
