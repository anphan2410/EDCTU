#ifndef PILOCALDBWORKER_H
#define PILOCALDBWORKER_H

#define piLclDbWrkDbgEn 1

#include <QObject>
#include <QCoreApplication>
#include <QTimer>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QByteArray>
#include <QUrl>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QtNetwork>
#include <QNetworkReply>
#include "binaryprotocol.h"
#include "windowcommand.h"
#include "canprotocol.h"
#include "anlogger.h"
#include "commonthings.h"
#include "smallcoordinatordb.h"
#include "pilocaldbworkerdb.h"

class piLocalDBWorker : public QObject
{
    Q_OBJECT
public:
    explicit piLocalDBWorker(QObject *parent = nullptr);

signals:
    void errorOccurred();
    void controlUHV2(BinaryProtocol);
    void controlUHV4(WindowCommand);
    void controlValve(CanProtocol);
public slots:
    void Test();
    void start();
    void stop();
    void doControlTask();
    void updatePVIData(quint32 GlobalID, const QString &Pressure, const QString &Voltage, const QString &Current);
    void updateRFIDData(quint32 GlobalID, QString srcRFID);
    void FromSmallCoordinator(SmallCoordinatorDB::GlobalSignal * aSignal);
    bool sendJsonDataPackage(QByteArray data);
    const QJsonObject createAJsonDataPackage(const QString &GlobalID, const QString &HV, const QString &Valve, const QString &Pressure, const QString &Voltage, const QString &Current, const QString &dstRFID, const QString &Time);
    const QJsonArray getPendingJsonDataPackage();
private:
    static const QNetworkRequest onlineDb;
    static constexpr const Qt::ConnectionType AutoUniqueConnection = static_cast<Qt::ConnectionType>(Qt::AutoConnection | Qt::UniqueConnection);
    QTimer timer;
    QTimer JsonSendTimer;
    QSqlDatabase localDb;
    bool isRunning = false;
    bool isOnline = true;
    bool isJsonSendFinished = true;
    bool isAControlTaskDone = true;
    bool isPendingJsonSendRequested = false;
    QSqlQuery currentQuery;
    QSqlQuery tmpQuery;
    QSqlQuery tmpQuery2;
    QByteArray tmpData;
    BinaryProtocol tmpBP;
    WindowCommand tmpWC;
    CanProtocol tmpCP;
    QString tmpStr;
    QString tmpStr1;
    QString tmpStr2;
    QString tmpStr3;
    QString tmpStr4;
    QString tmpStr5;
    QNetworkAccessManager * NAManager = Q_NULLPTR;
    QNetworkReply * reply = Q_NULLPTR;
    SmallCoordinatorDB::GlobalSignal * currentSignal;
    piLocalDBWorkerDB * currentDb;
    QString currentMsgEnum;
    piLocalDBWorkerDB::IdPVI * currentIdPVI;
    piLocalDBWorkerDB::IdRFID * currentIdRFID;
};

#endif // PILOCALDBWORKER_H
