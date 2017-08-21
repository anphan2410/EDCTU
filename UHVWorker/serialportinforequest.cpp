#include "serialportinforequest.h"

SerialPortInfoRequest::SerialPortInfoRequest(UHVWorkerVarSet *VarSet, quint32 TimerIntervalInMilisecond) :
    TimerIntervalMSecs(TimerIntervalInMilisecond)
{
    anIf(UHVWorkerVarSetDbgEn, anAck("Construct A New State !"));
    if (TimerIntervalInMilisecond > 0)
    {
        timer.setParent(this);
        timer.setInterval(TimerIntervalInMilisecond);
        QObject::connect(&timer, &QTimer::timeout
                        , this
                        , [VarSet](){
                                anIf(UHVWorkerVarSetDbgEn, anAck("Emit UHVWorkerVarSet::ANewPortName!"));
                                emit VarSet->Out(new QVariant(QVariant::fromValue(UHVWorkerVarSet::ANewPortName)));
                            }
                        , static_cast<Qt::ConnectionType>(Qt::AutoConnection | Qt::UniqueConnection));
    }
}

void SerialPortInfoRequest::onEntry(QEvent *)
{
    anIf(UHVWorkerVarSetDbgEn, anAck("Enter State ..."));
    if (TimerIntervalMSecs > 0)
        timer.start();
}

void SerialPortInfoRequest::onExit(QEvent *)
{
    if (TimerIntervalMSecs > 0)
        timer.stop();
}
