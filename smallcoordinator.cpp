#include "smallcoordinator.h"

SmallCoordinator::SmallCoordinator(QObject *parent) : QObject(parent)
{
    currentDb = new SmallCoordinatorDB();
}

void SmallCoordinator::In(SmallCoordinatorDB::GlobalSignal *aSignal)
{
    if (GlobalSignalKeysSortedByPriority.contains(aSignal->Priority))
    {
        GlobalSignalKeysSortedByPriority.value(aSignal->Priority)->append(aSignal->Key);
    }
    else
    {
        GlobalSignalKeysSortedByPriority.insert(aSignal->Priority,new QList<QString>({aSignal->Key}));
    }
    GlobalSignalsByGlobalSignalKey.insert(aSignal->Key,aSignal);
}

void SmallCoordinator::start()
{
    isRunning = true;
    work();
}

void SmallCoordinator::stop()
{
    isRunning = false;
}

void SmallCoordinator::work()
{
    while (isRunning)
    {
        qApp->processEvents();
        if (!GlobalSignalKeysSortedByPriority.isEmpty())
        {
            currentSignal = GlobalSignalsByGlobalSignalKey.value(GlobalSignalKeysSortedByPriority.last()->first());
            tmpStr = currentSignal->DstStrs.first();
            tmpStr1 = currentSignal->Key;
            currentSignal->DstStrs.removeFirst();
            GlobalSignalKeysSortedByPriority.value(currentSignal->Priority)->removeOne(currentSignal->Key);
            if (GlobalSignalKeysSortedByPriority.value(currentSignal->Priority)->isEmpty())
                GlobalSignalKeysSortedByPriority.remove(currentSignal->Priority);
            GlobalSignalsByGlobalSignalKey.remove(currentSignal->Key);
            if (tmpStr1=="NULL")
            {
                //emit all
            }
            else
            {
                if (tmpStr==UHV2WorkerObj->objectName())
                {
                    emit ToUHV2Worker(currentSignal);
                }
                else if (tmpStr==UHV4WorkerObj->objectName())
                {
                    emit ToUHV2Worker(currentSignal);
                }
                else if (tmpStr==CanBusWorkerObj->objectName())
                {
                    emit ToUHV2Worker(currentSignal);
                }
                else if (tmpStr==piLocalDBWorkerObj->objectName())
                {
                    emit ToUHV2Worker(currentSignal);
                }
                else if (tmpStr==UHV2PVICollectorObj->objectName())
                {
                    emit ToUHV2Worker(currentSignal);
                }
                else if (tmpStr==UHV4PVICollectorObj->objectName())
                {
                    emit ToUHV2Worker(currentSignal);
                }
                else if (tmpStr==CanCollectorObj->objectName())
                {
                    emit ToUHV2Worker(currentSignal);
                }
                else
                {
                    anIf(SmallCoordinatorDbgEn, anWarn("Invalid Message Deleted !"));
                }
            }
        }
    }
}
