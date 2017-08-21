#include "uhv2pvicollectordb.h"

UHV2PVICollectorDB::UHV2PVICollectorDB(QObject *parent) : QObject(parent), currentBP(new BinaryProtocol(0))
{
    anIf(UHV2PVICollectorDBDbgEn, anTrk("Construct Object"));
}

bool UHV2PVICollectorDB::initialize()
{
    localDb = QSqlDatabase::addDatabase("QMYSQL");
    localDb.setHostName("localhost");
    localDb.setDatabaseName("raspberry");
    localDb.setUserName("root");
    localDb.setPassword("Ascenx123");
    localDb.setPort(3306);
    if (localDb.open())
    {
        anIf(UHV2PVICollectorDBDbgEn, anAck("OK Local Database Connected !"));
        currentGlobalID = 0;
        currentQuery = QSqlQuery();        
    }
    else
    {
        anIf(UHV2PVICollectorDBDbgEn, anError("Failed To Connect Local Database !"));
        emit errorOccurred();
        return false;
    }
    anIf(UHV2PVICollectorDBDbgEn, anAck("OK Initialized !"));
    return true;
}

bool UHV2PVICollectorDB::gotoNextRecord()
{
    anIf(UHV2PVICollectorDBDbgEn, anTrk("Retrieve New Record"));
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
    currentBPNo = currentQuery.value("pumpAddr").toInt();
    currentCH = currentQuery.value("pumpCH").toInt();
    currentBP->SetBPNo(currentBPNo);
    currentBP->Ch(currentCH);
    anIf(UHV2PVICollectorDBDbgEn, anVar(currentGlobalID));
    return true;
}

void UHV2PVICollectorDB::emitReadP()
{
    if (gotoNextRecord())
    {
        currentBP->ReadP();
        emit CommandOut(currentBP->GenMsg());
    }
    else
    {
        anIf(UHV2PVICollectorDBDbgEn, anWarn("Not Found Data!"));
        emit pause();
    }
}

void UHV2PVICollectorDB::emitReadV()
{
    currentBP->ReadV();
    emit CommandOut(currentBP->GenMsg());
}

void UHV2PVICollectorDB::emitReadI()
{
    currentBP->ReadI();
    emit CommandOut(currentBP->GenMsg());
}

void UHV2PVICollectorDB::emitMsgToDatabaseUpdatePVI()
{
    anIf(UHV2PVICollectorDBDbgEn,
            anAck("Emit PVI To Update Database !");
            anVar(currentGlobalID);
            anVar(currentPressure);
            anVar(currentVoltage);
            anVar(currentCurrent));
    emit MessageToDatabase(currentGlobalID,currentPressure,currentVoltage,currentCurrent);
}

void UHV2PVICollectorDB::saveP()
{
    if (currentReplyFromPump.isEmpty())
    {
        currentPressure = "0";
    }
    else
    {
        currentBP = &BinaryProtocol::BP(currentReplyFromPump);
        currentPressure = QString(currentBP->GetData());
    }
    anIf(UHV2PVICollectorDBDbgEn, anVar(currentPressure));
    emit DataObtained();
}

void UHV2PVICollectorDB::saveV()
{
    if (currentReplyFromPump.isEmpty())
    {
        currentVoltage = "0";
    }
    else
    {
        currentBP = &BinaryProtocol::BP(currentReplyFromPump);
        currentVoltage = QString(currentBP->GetData());
    }
    anIf(UHV2PVICollectorDBDbgEn, anVar(currentVoltage));
    emit DataObtained();
}

void UHV2PVICollectorDB::saveI()
{
    if (currentReplyFromPump.isEmpty())
    {
        currentCurrent = "0";
    }
    else
    {
        currentBP = &BinaryProtocol::BP(currentReplyFromPump);
        currentCurrent = QString(currentBP->GetData());
    }
    anIf(UHV2PVICollectorDBDbgEn, anVar(currentCurrent));
    emit DataObtained();
}

void UHV2PVICollectorDB::setPreviousReadState(const QString &StateObjName)
{
    previousReadState = StateObjName;
}

void UHV2PVICollectorDB::resume()
{
    emit directTransitionRequest(previousReadState);
}

void UHV2PVICollectorDB::processDataFromPump(const QByteArray &data)
{
    anIf(UHV2PVICollectorDBDbgEn, anAck("Data From Pump Received !");anVar(data.toHex()););
    currentReplyFromPump = data;
    if (previousReadState=="emitReadP")
        saveP();
    else if (previousReadState=="emitReadV")
        saveV();
    else
        saveI();
}


