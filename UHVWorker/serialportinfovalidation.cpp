#include "serialportinfovalidation.h"

SerialPortInfoValidation::SerialPortInfoValidation(UHVWorkerVarSet *VarSet)
    : VarSetPtr(VarSet)
{
    anIf(UHVWorkerVarSetDbgEn, anAck("Construct A New State !"));
}

SerialPortInfoValidation::~SerialPortInfoValidation()
{
    VarSetPtr = Q_NULLPTR;
    delete VarSetPtr;
}

void SerialPortInfoValidation::onEntry(QEvent *)
{
    anIf(UHVWorkerVarSetDbgEn, anAck("Enter State ..."));
    if (VarSetPtr->SerialPort)
    {
        if (VarSetPtr->SerialPort->isOpen())
        {
            VarSetPtr->SerialPort->close();
            anIf(UHVWorkerVarSetDbgEn, anInfo("SerialPort Disconnected !"));
        }
        else
        {
            anIf(UHVWorkerVarSetDbgEn, anInfo("SerialPort Already Not Connected !"));
        }
    }
    else
    {
        anIf(UHVWorkerVarSetDbgEn, anInfo("SerialPort Already Not Exist !"));
    }
    QSerialPortInfo SerialPortInfo(*(VarSetPtr->PortName));
    if (SerialPortInfo.isNull())
    {
        anIf(UHVWorkerVarSetDbgEn, anWarn("SerialPortInfo Is Null !"));
        emit VarSetPtr->DirectStateTransitionRequest("SerialPortInfoRequest");
    }
    else
    {
        anIf(UHVWorkerVarSetDbgEn, anAck("SerialPortInfo Exists !"));
        emit VarSetPtr->DirectStateTransitionRequest("SerialPortConnectionEstablishment");
    }
}
