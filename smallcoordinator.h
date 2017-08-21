#ifndef SMALLCOORDINATOR_H
#define SMALLCOORDINATOR_H

#define SmallCoordinatorDbgEn 1

#include <QCoreApplication>
#include <QObject>
#include <QMap>
#include <QHash>
#include <QList>
#include <QString>
#include <QVariant>
#include "commonthings.h"
#include "anlogger.h"
#include "smallcoordinatordb.h"

class SmallCoordinator : public QObject
{
    Q_OBJECT
public:
    explicit SmallCoordinator(QObject *parent = nullptr);
signals:
    void ToUHV2Worker(SmallCoordinatorDB::GlobalSignal *);
    void ToUHV4Worker(SmallCoordinatorDB::GlobalSignal *);
    void ToCanBusWorker(SmallCoordinatorDB::GlobalSignal *);
    void TopiLocalDBWorker(SmallCoordinatorDB::GlobalSignal *);
    void ToUHV2PVICollector(SmallCoordinatorDB::GlobalSignal *);
    void ToUHV4PVICollector(SmallCoordinatorDB::GlobalSignal *);
    void ToCanCollector(SmallCoordinatorDB::GlobalSignal *);
public slots:
    void In(SmallCoordinatorDB::GlobalSignal * aSignal);
    void start();
    void stop();
private:
    void work();

    QMap<quint8,QList<QString>*> GlobalSignalKeysSortedByPriority;
    QHash<QString,SmallCoordinatorDB::GlobalSignal*> GlobalSignalsByGlobalSignalKey;
    bool isRunning = false;
    QObject * UHV2WorkerObj;
    QObject * UHV4WorkerObj;
    QObject * CanBusWorkerObj;
    QObject * piLocalDBWorkerObj;
    QObject * UHV2PVICollectorObj;
    QObject * UHV4PVICollectorObj;
    QObject * CanCollectorObj;
    SmallCoordinatorDB::GlobalSignal * currentSignal;
    QString tmpStr;
    QString tmpStr1;
    SmallCoordinatorDB * currentDb;

};

#endif // SMALLCOORDINATOR_H
