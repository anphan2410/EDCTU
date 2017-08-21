#include "uhvworker.h"

UHVWorker::UHVWorker()
{
    anIf(UHVWorkerDbgEn, anTrk("Construct A New UHVWorker !"));
    WorkingVarSet = new UHVWorkerVarSet(this);
    SerialPortInfoRequest * state0 = new SerialPortInfoRequest(WorkingVarSet);
    state0->setObjectName("SerialPortInfoRequest");
    SerialPortInfoValidation * state1 = new SerialPortInfoValidation(WorkingVarSet);
    state1->setObjectName("SerialPortInfoValidation");
    SerialPortConnectionEstablishment * state2 = new SerialPortConnectionEstablishment(WorkingVarSet);
    state2->setObjectName("SerialPortConnectionEstablishment");
    SolitaryMessageTransmission * state3 = new SolitaryMessageTransmission(WorkingVarSet);
    state3->setObjectName("SolitaryMessageTransmission");
    MessageReceiveAndEmitOut * state4 = new MessageReceiveAndEmitOut(WorkingVarSet);
    state4->setObjectName("MessageReceiveAndEmitOut");
    ErrorAnnouncement * state7 = new ErrorAnnouncement(WorkingVarSet);
    state7->setObjectName("ErrorAnnouncement");

    state0->addTransition(WorkingVarSet, &UHVWorkerVarSet::PortNameChanged, state1);
    state1->addTransition(WorkingVarSet, &UHVWorkerVarSet::PortNameChanged, state1);
    state1->addTransition(new UHVWorkerDirectStateTransition(WorkingVarSet, state0));
    state1->addTransition(new UHVWorkerDirectStateTransition(WorkingVarSet, state2));
    state2->addTransition(WorkingVarSet->SerialPort, &QSerialPort::errorOccurred, state7);
    state2->addTransition(new UHVWorkerDirectStateTransition(WorkingVarSet, state3));
    state2->addTransition(WorkingVarSet, &UHVWorkerVarSet::PortNameChanged, state1);
    state3->addTransition(WorkingVarSet->SerialPort, &QSerialPort::errorOccurred, state7);
    state3->addTransition(new UHVWorkerDirectStateTransition(WorkingVarSet, state4));
    state3->addTransition(WorkingVarSet, &UHVWorkerVarSet::PortNameChanged, state1);
    state3->addTransition(WorkingVarSet, &UHVWorkerVarSet::AFirstPrioritizedCommandMessageReceived, state3);
    state4->addTransition(WorkingVarSet->SerialPort, &QSerialPort::errorOccurred, state7);
    state4->addTransition(WorkingVarSet, &UHVWorkerVarSet::PortNameChanged, state1);
    state4->addTransition(new UHVWorkerDirectStateTransition(WorkingVarSet, state3));
    state7->addTransition(WorkingVarSet, &UHVWorkerVarSet::PortNameChanged, state1);
    state7->addTransition(WorkingVarSet, &UHVWorkerVarSet::RestartSerialPortConnection, state1);

    this->addState(state0);
    this->addState(state1);
    this->addState(state2);
    this->addState(state3);
    this->addState(state4);
    this->addState(state7);
    this->setInitialState(state0);
    this->setErrorState(state7);

    connect(WorkingVarSet->SerialPort, &QSerialPort::errorOccurred,
            this, [&](){ WorkingVarSet->ErrorStatus = UHVWorkerVarSet::SerialPortError;});
    connect(WorkingVarSet, &UHVWorkerVarSet::Out, this, &UHVWorker::Out);
    connect(state0, &QState::entered,
            this, [&](){ emit Out(new QVariant(QVariant::fromValue(UHVWorkerVarSet::SerialPortDisconnect)));});
    connect(this, &QStateMachine::stopped,
            this, [&](){
        anIf(UHVWorkerDbgEn, anWarn("UHVWorker Stopped !"));
        WorkingVarSet->initialize();
    });
    connect(WorkingVarSet, &UHVWorkerVarSet::Out, this, [&](QVariant * MsgTopic, QVariant * MsgData){
        currentSignal->Type = *MsgTopic;
        currentSignal->Data = *MsgData;
        emit ToSmallCoordinator(currentSignal);
    });
}

void UHVWorker::In(QVariant *AnUHVWorkerEnumValue, QVariant *rawData)
{
    anIf(UHVWorkerDbgEn, anInfo("An External Message Received !"));
    if (QString(AnUHVWorkerEnumValue->typeName()) == "UHVWorkerVarSet::MessageTopic")
    {
        anIf(UHVWorkerDbgEn, anAck("UHVWorkerVarSet::MessageTopic Parsed !"));
        switch (AnUHVWorkerEnumValue->toInt()) {
        case UHVWorkerVarSet::ANewPortName:
        {
            QString newPortName = rawData->toString();
            if (newPortName != "")
            {
                anIf(UHVWorkerDbgEn, anAck("ANewPortName Received !"));
                *(WorkingVarSet->PortName) = newPortName;
                anVar(*(WorkingVarSet->PortName));
                anIf(UHVWorkerDbgEn, anInfo("Emit PortNameChanged() ..."));
                emit WorkingVarSet->PortNameChanged();
            }
            break;
        }
        case UHVWorkerVarSet::AnUHVPrioritizedCommandMessage:
        {
            anIf(UHVWorkerDbgEn, anAck("AnUHVPrioritizedCommandMessage Received !"));
            UHVWorkerVarSet::PrioritizedCommandMessage * newCmdMsg
                    = new UHVWorkerVarSet::PrioritizedCommandMessage(rawData->value<UHVWorkerVarSet::PrioritizedCommandMessage>());
            if (WorkingVarSet->PendingMessageList->contains(newCmdMsg->first))
            {
                WorkingVarSet->PendingMessageList->value(newCmdMsg->first)->append(newCmdMsg->second);
            }
            else
            {
                WorkingVarSet->PendingMessageList->insert(newCmdMsg->first, new QList<UHVWorkerVarSet::CommandMessage*>({newCmdMsg->second}));
            }
            if (WorkingVarSet->PendingMessageList->size() == 1)
            {
                if (WorkingVarSet->PendingMessageList->first()->size() == 1)
                {
                    emit WorkingVarSet->AFirstPrioritizedCommandMessageReceived();
                }
            }
            break;
        }
        case UHVWorkerVarSet::SerialPortRestart:
        {
            emit WorkingVarSet->RestartSerialPortConnection();
            break;
        }
        case UHVWorkerVarSet::SerialPortDisconnect:
        {
            anIf(UHVWorkerDbgEn, anAck("SerialPortDisconnect Received !"));
            *(WorkingVarSet->PortName) = "";
            emit WorkingVarSet->PortNameChanged();
            break;
        }
        case UHVWorkerVarSet::PendingMessageListClear:
        {
            anIf(UHVWorkerDbgEn, anAck("PendingMessageListClear Received !"));
            WorkingVarSet->PendingMessageList->clear();
            anIf(UHVWorkerDbgEn, anInfo("PendingMessageList Cleared !"));
            emit Out(new QVariant(QVariant::fromValue(UHVWorkerVarSet::PendingMessageListClear)));
            break;
        }
        default:
            break;
        }
    }
}

void UHVWorker::FromSmallCoordinator(SmallCoordinatorDB::GlobalSignal *aSignal)
{
    currentSignal = aSignal;
    In(&(aSignal->Type),&(aSignal->Data));
}
