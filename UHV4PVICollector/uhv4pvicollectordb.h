#ifndef UHV4PVICOLLECTORDB_H
#define UHV4PVICOLLECTORDB_H

#define UHV4PVICollectorDBDbgEn 1
#define UHV4PVICollectorStateDbgEn 1

#include <QObject>
#include "anlogger.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QByteArray>
#include "windowcommand.h"

class UHV4PVICollectorDB : public QObject
{
    Q_OBJECT
public:
    explicit UHV4PVICollectorDB(QObject *parent = nullptr);
    quint32 currentGlobalID;
    quint32 breakIntervalMSecs = 1000;
    quint32 waitIntervalMSecs = 5000;
    QSqlDatabase localDb;
    QSqlQuery currentQuery;
    WindowCommand * currentWC;
    QByteArray currentReplyFromPump;
    QString currentPressure;
    QString currentVoltage;
    QString currentCurrent;
    QString previousReadState;
    quint8 currentWCNo;
    quint8 currentCH;
signals:
    void errorOccurred();
    void pause();
    void directTransitionRequest(const QString &);
    void CommandOut(const QByteArray &);
    void DataObtained();
    void MessageToDatabase(quint32 GlobalID, const QString &Pressure, const QString &Voltage, const QString &Current);
public slots:
    bool initialize();
    bool gotoNextRecord();
    void emitReadP();
    void emitReadV();
    void emitReadI();
    void emitMsgToDatabaseUpdatePVI();
    void saveP();
    void saveV();
    void saveI();
    void setPreviousReadState(const QString &StateObjName);
    void resume();
    void processDataFromPump(const QByteArray &data);
};

#endif // UHV4PVICOLLECTORDB_H
