#include "uhvworkervarset.h"

UHVWorkerVarSet::UHVWorkerVarSet(QObject *parent)
    : QObject(parent)
{
    qRegisterMetaType<UHVWorkerVarSet::PrioritizedCommandMessage>("UHVWorkerVarSet::PrioritizedCommandMessage");
    initialize();
    anIf(UHVWorkerVarSetDbgEn, anAck("Construct A New UHVWorkerVarSet !"));
}

UHVWorkerVarSet::~UHVWorkerVarSet()
{
    PendingMessageList->clear();
    PendingMessageList = Q_NULLPTR;
    PortName = Q_NULLPTR;
    SerialPort = Q_NULLPTR;
    lastTransmittedMessage = Q_NULLPTR;
    lastReceivedMessage = Q_NULLPTR;
    delete PortName;
    delete SerialPort;
    delete lastTransmittedMessage;
    delete lastReceivedMessage;
    delete PendingMessageList;
}

void UHVWorkerVarSet::configSerialPort()
{
    SerialPort->setReadBufferSize(64);
    SerialPort->setDataBits(QSerialPort::Data8);
    SerialPort->setBaudRate(QSerialPort::Baud9600);
    SerialPort->setStopBits(QSerialPort::OneStop);
    SerialPort->setParity(QSerialPort::NoParity);
    SerialPort->setFlowControl(QSerialPort::NoFlowControl);
    SerialPort->setPortName(*PortName);
}

void UHVWorkerVarSet::initialize()
{
    PortName = new QString();
    if (SerialPort)
        if (SerialPort->isOpen())
            SerialPort->close();
    SerialPort = new QSerialPort();
    if (PendingMessageList)
        PendingMessageList->clear();
    else
        PendingMessageList = new QMap<qint8,QList<CommandMessage*>*>();
    ErrorStatus = Nothing;
}
