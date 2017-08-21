#include "uhv2pvicollector.h"

UHV2PVICollector::UHV2PVICollector(QObject *parent) : QStateMachine(parent)
{
    anIf(UHV2PVICollectorDbgEn, anTrk("Contruct A State Machine"));
    currentDb = new UHV2PVICollectorDB(this);

    emitReadP * state1 = new emitReadP(currentDb);
    state1->setObjectName("emitReadP");
    wait4P * state2 = new wait4P(currentDb);
    state2->setObjectName("wait4P");
    emitReadV * state3 = new emitReadV(currentDb);
    state3->setObjectName("emitReadV");
    wait4V * state4 = new wait4V(currentDb);
    state4->setObjectName("wait4V");
    emitReadI * state5 = new emitReadI(currentDb);
    state5->setObjectName("emitReadI");
    wait4I * state6 = new wait4I(currentDb);
    state6->setObjectName("wait4I");
    idle * state7 = new idle();
    state7->setObjectName("idle");

    state1->addTransition(currentDb, &UHV2PVICollectorDB::CommandOut, state2);
    state2->addTransition(currentDb, &UHV2PVICollectorDB::DataObtained, state3);
    state3->addTransition(currentDb, &UHV2PVICollectorDB::CommandOut, state4);
    state4->addTransition(currentDb, &UHV2PVICollectorDB::DataObtained, state5);
    state5->addTransition(currentDb, &UHV2PVICollectorDB::CommandOut, state6);
    state6->addTransition(currentDb, &UHV2PVICollectorDB::DataObtained, state1);

    state1->addTransition(currentDb, &UHV2PVICollectorDB::pause, state7);
    state3->addTransition(currentDb, &UHV2PVICollectorDB::pause, state7);
    state5->addTransition(currentDb, &UHV2PVICollectorDB::pause, state7);
    state7->addTransition(new directTransition(currentDb, state1));
    state7->addTransition(new directTransition(currentDb, state3));
    state7->addTransition(new directTransition(currentDb, state5));

    this->addState(state1);
    this->addState(state2);
    this->addState(state3);
    this->addState(state4);
    this->addState(state5);
    this->addState(state6);
    this->addState(state7);
    this->setInitialState(state1);
    this->setErrorState(state7);

    QObject::connect(this, &UHV2PVICollector::started, currentDb, &UHV2PVICollectorDB::initialize);
    QObject::connect(currentDb, &UHV2PVICollectorDB::MessageToDatabase, this, [&](){

    });
}

void UHV2PVICollector::pause()
{
    emit currentDb->pause();
}

void UHV2PVICollector::resume()
{
    currentDb->resume();
}

void UHV2PVICollector::DataFromPump(const QByteArray &data)
{
    currentDb->processDataFromPump(data);
}

void UHV2PVICollector::FromSmallCoordinator(SmallCoordinatorDB::GlobalSignal *aSignal)
{
    anIf(UHV2PVICollectorDbgEn, anAck("An External Signal Received !"));
    currentMsgEnumTypeName = QString(aSignal->Type.typeName());
    if (currentMsgEnumTypeName=="UHV2PVICollectorDB::DataGet")
    {
        anIf(UHV2PVICollectorDbgEn, anAck("" + currentMsgEnumTypeName + " Parsed !"));
        switch (aSignal->Type.toInt()) {
        case UHV2PVICollectorDB::replyPumpData:
        {
            DataFromPump(aSignal->Data.value<QByteArray>());
            break;
        }
        default:
            break;
        }

    }
}
