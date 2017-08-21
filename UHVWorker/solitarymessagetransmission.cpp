#include "solitarymessagetransmission.h"

SolitaryMessageTransmission::SolitaryMessageTransmission(UHVWorkerVarSet *VarSet, quint16 WriteTimeOutInMilisecond)
    : VarSetPtr(VarSet), TimeOut4WriteInMilisecond(WriteTimeOutInMilisecond)
{
    anIf(UHVWorkerVarSetDbgEn, anAck("Construct A New State !"));
}

SolitaryMessageTransmission::~SolitaryMessageTransmission()
{
    VarSetPtr = Q_NULLPTR;
    delete VarSetPtr;
}

void SolitaryMessageTransmission::onEntry(QEvent *)
{
    anIf(UHVWorkerVarSetDbgEn, anAck("Enter State ..."));
    qApp->processEvents();
    while (VarSetPtr->PendingMessageList->size())
    {
        if (VarSetPtr->PendingMessageList->last()->size())
            break;
        else
            VarSetPtr->PendingMessageList->remove(VarSetPtr->PendingMessageList->lastKey());
    }
    if (VarSetPtr->PendingMessageList->size())
    {
        VarSetPtr->lastTransmittedMessagePriority = VarSetPtr->PendingMessageList->lastKey();
        VarSetPtr->lastTransmittedMessage = VarSetPtr->PendingMessageList->last()->takeFirst();
        if (VarSetPtr->lastTransmittedMessage)
        {
            VarSetPtr->SerialPort->write(*(VarSetPtr->lastTransmittedMessage->first));
            if (VarSetPtr->SerialPort->waitForBytesWritten(TimeOut4WriteInMilisecond))
            {
                anIf(UHVWorkerVarSetDbgEn, anAck("Successfully Write Message !"));
                emit VarSetPtr->Out(new QVariant(QVariant::fromValue(UHVWorkerVarSet::AnUHVPrioritizedCommandMessage)),
                                    new QVariant(QVariant::fromValue(UHVWorkerVarSet::PrioritizedCommandMessage(VarSetPtr->lastTransmittedMessagePriority, VarSetPtr->lastTransmittedMessage))));
            }
            else
            {
                anIf(UHVWorkerVarSetDbgEn, anWarn("Writing Message Timed Out !"));
                VarSetPtr->lastTransmittedMessage = Q_NULLPTR;
                emit VarSetPtr->Out(new QVariant(QVariant::fromValue(UHVWorkerVarSet::MessageSendTimedOut)));
            }
            emit VarSetPtr->DirectStateTransitionRequest("MessageReceiveAndEmitOut");
        }
    }
    else
    {
        anIf(UHVWorkerVarSetDbgEn, anAck("Wait For A Message !"));
    }
}
