#include "messagereceiveandemitout.h"

MessageReceiveAndEmitOut::MessageReceiveAndEmitOut(UHVWorkerVarSet *VarSet, quint16 ReadTimeOutInMilisecond)
    : VarSetPtr(VarSet), TimeOut4ReadInMilisecond(ReadTimeOutInMilisecond)
{
    anIf(UHVWorkerVarSetDbgEn, anAck("Construct A New State !"));
}

MessageReceiveAndEmitOut::~MessageReceiveAndEmitOut()
{
    VarSetPtr = Q_NULLPTR;
    delete VarSetPtr;
}

void MessageReceiveAndEmitOut::onEntry(QEvent *)
{
    anIf(UHVWorkerVarSetDbgEn, anAck("Enter State ..."));
    qApp->processEvents();
    if (VarSetPtr->SerialPort->waitForReadyRead(TimeOut4ReadInMilisecond*10))
    {
        QByteArray tmpRead(VarSetPtr->SerialPort->readAll());
        while (VarSetPtr->SerialPort->waitForReadyRead(TimeOut4ReadInMilisecond))
        {
            tmpRead+=VarSetPtr->SerialPort->readAll();
        }
        VarSetPtr->lastReceivedMessage = new UHVWorkerVarSet::CommandMessage();
        VarSetPtr->lastReceivedMessage->first = new QByteArray(tmpRead);
        VarSetPtr->lastReceivedMessage->second = VarSetPtr->lastTransmittedMessage->second;
        anIf(UHVWorkerVarSetDbgEn, anAck("Successfully To Read Message !"));
        emit VarSetPtr->Out(new QVariant(QVariant::fromValue(UHVWorkerVarSet::AnUHVPrioritizedCommandMessage)),
                            new QVariant(QVariant::fromValue(UHVWorkerVarSet::PrioritizedCommandMessage(VarSetPtr->lastTransmittedMessagePriority, VarSetPtr->lastReceivedMessage))));
    }
    else
    {
        anIf(UHVWorkerVarSetDbgEn, anWarn("Reading Message Timed Out !"));
        VarSetPtr->lastReceivedMessage = Q_NULLPTR;
        emit VarSetPtr->Out(new QVariant(QVariant::fromValue(UHVWorkerVarSet::MessageReadTimedOut)));
    }
    emit VarSetPtr->DirectStateTransitionRequest("SolitaryMessageTransmission");
}
