#include "serialportconnectionestablishment.h"

SerialPortConnectionEstablishment::SerialPortConnectionEstablishment(UHVWorkerVarSet *VarSet)
    : VarSetPtr(VarSet)
{
    anIf(UHVWorkerVarSetDbgEn, anAck("Construct A New State !"));
}

SerialPortConnectionEstablishment::~SerialPortConnectionEstablishment()
{
    VarSetPtr = Q_NULLPTR;
    delete VarSetPtr;
}

void SerialPortConnectionEstablishment::onEntry(QEvent *)
{
    anIf(UHVWorkerVarSetDbgEn, anAck("Enter State ..."));
    VarSetPtr->SerialPort = new QSerialPort();
    VarSetPtr->configSerialPort();
    if (VarSetPtr->SerialPort->open(QIODevice::ReadWrite))
    {
        anIf(UHVWorkerVarSetDbgEn, anAck("Successfully Open A Serial Port !"));
        emit VarSetPtr->Out(new QVariant(QVariant::fromValue(UHVWorkerVarSet::SerialPortConnect)));
        emit VarSetPtr->DirectStateTransitionRequest("SolitaryMessageTransmission");
    }
    else
    {
        anIf(UHVWorkerVarSetDbgEn, anWarn("Failed To Open A Serial Port !"));
    }
}
