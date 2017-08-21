#ifndef UHV2PVICOLLECTORDB_H
#define UHV2PVICOLLECTORDB_H

#define UHV2PVICollectorDBDbgEn 1
#define UHV2PVICollectorStateDbgEn 1

#include <QObject>
#include "anlogger.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QByteArray>
#include "binaryprotocol.h"

class UHV2PVICollectorDB : public QObject
{
    Q_OBJECT
public:
    explicit UHV2PVICollectorDB(QObject *parent = nullptr);

    enum DataGet
    {
        NoDataGet = 0,

    };
    Q_ENUM(DataGet)

    enum DataSet
    {
        NoDataSet = 0,
        replyPumpData
    };
    Q_ENUM(DataSet)

    enum Error
    {
        NoError = 0
    };
    Q_ENUM(Error)

    enum Warning
    {
        NoWarning = 0
    };
    Q_ENUM(Warning)

    enum Notification
    {
        NoNotification = 0
    };
    Q_ENUM(Notification)

    quint32 currentGlobalID;
    quint32 breakIntervalMSecs = 1000;
    quint32 waitIntervalMSecs = 5000;
    QSqlDatabase localDb;
    QSqlQuery currentQuery;
    BinaryProtocol * currentBP;
    QByteArray currentReplyFromPump;
    QString currentPressure;
    QString currentVoltage;
    QString currentCurrent;
    QString previousReadState;
    quint8 currentBPNo;
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

#endif // UHV2PVICOLLECTORDB_H
