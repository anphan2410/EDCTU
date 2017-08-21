#include "uhv4pvicollectordb.h"

UHV4PVICollectorDB::UHV4PVICollectorDB(QObject *parent) : QObject(parent), currentWC(new WindowCommand(0))
{
    anIf(UHV4PVICollectorDBDbgEn, anTrk("Construct Object"));
}

bool UHV4PVICollectorDB::initialize()
{
    localDb = QSqlDatabase::addDatabase("QMYSQL");
    localDb.setHostName("localhost");
    localDb.setDatabaseName("raspberry");
    localDb.setUserName("root");
    localDb.setPassword("Ascenx123");
    localDb.setPort(3306);
    if (localDb.open())
    {
        anIf(UHV4PVICollectorDBDbgEn, anAck("OK Local Database Connected !"));
        currentGlobalID = 0;
        currentQuery = QSqlQuery();        
    }
    else
    {
        anIf(UHV4PVICollectorDBDbgEn, anError("Failed To Connect Local Database !"));
        emit errorOccurred();
        return false;
    }
    anIf(UHV4PVICollectorDBDbgEn, anAck("OK Initialized !"));
    return true;
}

bool UHV4PVICollectorDB::gotoNextRecord()
{
    anIf(UHV4PVICollectorDBDbgEn, anTrk("Retrieve New Record"));
    currentQuery.exec("SELECT GlobalID,pumpAddr,pumpCH FROM stations WHERE GlobalID>" +
                        QString::number(currentGlobalID) + " ORDER BY GlobalID ASC LIMIT 1");
    if (!currentQuery.next())
    {
        currentGlobalID = 0;
        currentQuery.exec("SELECT GlobalID,pumpAddr,pumpCH FROM stations WHERE GlobalID>" +
                            QString::number(currentGlobalID) + " ORDER BY GlobalID ASC LIMIT 1");
        if (!currentQuery.next())
            return false;
    }
    currentGlobalID = currentQuery.value("GlobalID").toInt();
    currentWCNo = currentQuery.value("pumpAddr").toInt();
    currentCH = currentQuery.value("pumpCH").toInt();
    currentWC->SetWCNo(currentWCNo);
    currentWC->SetChannel(currentCH);
    anIf(UHV4PVICollectorDBDbgEn, anVar(currentGlobalID));
    return true;
}

void UHV4PVICollectorDB::emitReadP()
{
    if (gotoNextRecord())
    {
        currentWC->ReadP();
        emit CommandOut(currentWC->GenerateMSG());
    }
    else
    {
        anIf(UHV4PVICollectorDBDbgEn, anWarn("Not Found Data!"));
        emit pause();
    }
}

void UHV4PVICollectorDB::emitReadV()
{
    currentWC->ReadV();
    emit CommandOut(currentWC->GenerateMSG());
}

void UHV4PVICollectorDB::emitReadI()
{
    currentWC->ReadI();
    emit CommandOut(currentWC->GenerateMSG());
}

void UHV4PVICollectorDB::emitMsgToDatabaseUpdatePVI()
{
    anIf(UHV4PVICollectorDBDbgEn,
            anAck("Emit PVI To Update Database !");
            anVar(currentGlobalID);
            anVar(currentPressure);
            anVar(currentVoltage);
            anVar(currentCurrent));
    emit MessageToDatabase(currentGlobalID,currentPressure,currentVoltage,currentCurrent);
}

void UHV4PVICollectorDB::saveP()
{
    if (currentReplyFromPump.isEmpty())
    {
        currentPressure = "0";
    }
    else
    {
        currentWC = &WindowCommand::WC(currentReplyFromPump);
        currentPressure = QString(currentWC->GetDATA());
    }
    anIf(UHV4PVICollectorDBDbgEn, anVar(currentPressure));
    emit DataObtained();
}

void UHV4PVICollectorDB::saveV()
{
    if (currentReplyFromPump.isEmpty())
    {
        currentVoltage = "0";
    }
    else
    {
        currentWC = &WindowCommand::WC(currentReplyFromPump);
        currentVoltage = QString(currentWC->GetDATA());
    }
    anIf(UHV4PVICollectorDBDbgEn, anVar(currentVoltage));
    emit DataObtained();
}

void UHV4PVICollectorDB::saveI()
{
    if (currentReplyFromPump.isEmpty())
    {
        currentCurrent = "0";
    }
    else
    {
        currentWC = &WindowCommand::WC(currentReplyFromPump);
        currentCurrent = QString(currentWC->GetDATA());
    }
    anIf(UHV4PVICollectorDBDbgEn, anVar(currentCurrent));
    emit DataObtained();
}

void UHV4PVICollectorDB::setPreviousReadState(const QString &StateObjName)
{
    previousReadState = StateObjName;
}

void UHV4PVICollectorDB::resume()
{
    emit directTransitionRequest(previousReadState);
}

void UHV4PVICollectorDB::processDataFromPump(const QByteArray &data)
{
    anIf(UHV4PVICollectorDBDbgEn, anAck("Data From Pump Received !");anVar(data.toHex()););
    currentReplyFromPump = data;
    if (previousReadState=="emitReadP")
        saveP();
    else if (previousReadState=="emitReadV")
        saveV();
    else
        saveI();
}


