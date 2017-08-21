#include "uhv4pvicollector.h"

UHV4PVICollector::UHV4PVICollector(QObject *parent) : QStateMachine(parent)
{
    anIf(UHV4PVICollectorDbgEn, anTrk("Contruct A State Machine"));
    currentDb = new UHV4PVICollectorDB(this);

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

    state1->addTransition(currentDb, &UHV4PVICollectorDB::CommandOut, state2);
    state2->addTransition(currentDb, &UHV4PVICollectorDB::DataObtained, state3);
    state3->addTransition(currentDb, &UHV4PVICollectorDB::CommandOut, state4);
    state4->addTransition(currentDb, &UHV4PVICollectorDB::DataObtained, state5);
    state5->addTransition(currentDb, &UHV4PVICollectorDB::CommandOut, state6);
    state6->addTransition(currentDb, &UHV4PVICollectorDB::DataObtained, state1);

    state1->addTransition(currentDb, &UHV4PVICollectorDB::pause, state7);
    state3->addTransition(currentDb, &UHV4PVICollectorDB::pause, state7);
    state5->addTransition(currentDb, &UHV4PVICollectorDB::pause, state7);
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

    QObject::connect(this, &UHV4PVICollector::started, currentDb, &UHV4PVICollectorDB::initialize);

}

void UHV4PVICollector::pause()
{
    emit currentDb->pause();
}

void UHV4PVICollector::resume()
{
    currentDb->resume();
}

void UHV4PVICollector::DataFromPump(const QByteArray &data)
{
    currentDb->processDataFromPump(data);
}
