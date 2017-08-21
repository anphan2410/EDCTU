#include "pilocaldbworker.h"

piLocalDBWorker::piLocalDBWorker(QObject *parent) :
    QObject(parent), tmpBP(BinaryProtocol(0)), tmpWC(WindowCommand(0)),
    NAManager(new QNetworkAccessManager())
{
    anIf(piLclDbWrkDbgEn, anTrk("Construct Object"));
    currentDb = new piLocalDBWorkerDB();
    localDb = QSqlDatabase::addDatabase("QMYSQL");
    localDb.setHostName("localhost");
    localDb.setDatabaseName("raspberry");
    localDb.setUserName("root");
    localDb.setPassword("Ascenx123");
    localDb.setPort(3306);
    if (localDb.open())
    {
        anIf(piLclDbWrkDbgEn, anAck("OK Local Database Connected !"));
        currentQuery = QSqlQuery();
        tmpQuery = QSqlQuery();
        tmpQuery2 = QSqlQuery();
    }
    else
    {
        anIf(piLclDbWrkDbgEn, anError("Failed To Connect Local Database !"));
        emit errorOccurred();
    }
    timer.setParent(this);
    timer.setInterval(1000);
    JsonSendTimer.setParent(this);
    JsonSendTimer.setInterval(3000);
    JsonSendTimer.setSingleShot(true);
    QObject::connect(&timer, &QTimer::timeout, this, [&](){
        if (isAControlTaskDone)
            doControlTask();
    });
    QObject::connect(&JsonSendTimer, &QTimer::timeout, this, [&](){
        if (!isJsonSendFinished)
        {
            isOnline = false;
            isJsonSendFinished = true;
            anIf(piLclDbWrkDbgEn, anWarn("Failed To Send Json Package !"));
        }
    });
}

void piLocalDBWorker::Test()
{

}

void piLocalDBWorker::start()
{
    anIf(piLclDbWrkDbgEn, anTrk("Start Working"));
    timer.start();
    isRunning = true;
}

void piLocalDBWorker::stop()
{
    anIf(piLclDbWrkDbgEn, anTrk("Stop Working"));
    timer.stop();
    isRunning = false;
}

void piLocalDBWorker::doControlTask()
{
    isAControlTaskDone = false;
    if (isRunning)
    {
        anIf(piLclDbWrkDbgEn, anTrk("GATE"));
        currentQuery.exec("SELECT GlobalID,HV,Valve FROM control WHERE (GlobalID Is Not Null And GlobalID>0) ORDER BY Time ASC");
        while (currentQuery.next())
        {
            tmpQuery.exec("SELECT pumpType,pumpAddr,pumpCH,sdcsAddr,sdcsCH,RFID FROM stations WHERE GlobalID=" + currentQuery.value("GlobalID").toString() + " LIMIT 1");
            if (tmpQuery.next())
            {
                if (tmpQuery.value("pumpType").toInt()==0)
                {
                    tmpBP.SetBPNo(tmpQuery.value("pumpAddr").toInt());
                    tmpBP.Ch(tmpQuery.value("pumpCH").toInt()).HVSwitch();
                    if (currentQuery.value("HV").toInt()==0)
                    {
                        tmpBP.Off();
                    }
                    else
                    {
                        tmpBP.On();
                    }
                    tmpBP.GenMsg();
                    anIf(piLclDbWrkDbgEn, anAck("Control UHV2: " << tmpBP.GetMessageTranslation()));
                    emit controlUHV2(tmpBP);
                }
                else if (tmpQuery.value("pumpType").toInt()==1)
                {
                    tmpWC.SetWCNo(tmpQuery.value("pumpAddr").toInt());
                    if (currentQuery.value("HV").toInt()==0)
                    {
                        tmpWC.HVSwitch(tmpQuery.value("pumpCH").toInt(),"OFF");
                    }
                    else
                    {
                        tmpWC.HVSwitch(tmpQuery.value("pumpCH").toInt(),"ON");
                    }
                    tmpWC.GenerateMSG();
                    anIf(piLclDbWrkDbgEn, anAck("Control UHV4: " << tmpWC.GetMSG().toHex()));
                    emit controlUHV4(tmpWC);
                }
                else
                {
                    anIf(piLclDbWrkDbgEn, anWarn("Invalid pumpType at GlobalID=" + currentQuery.value("GlobalID").toString()));
                }
                tmpCP.SetSdcsId(tmpQuery.value("sdcsAddr").toInt()).SetChId(tmpQuery.value("sdcsCH").toInt()).SetRFID(QByteArray::fromHex(tmpQuery.value("RFID").toByteArray().mid(1)));
                if (currentQuery.value("Valve").toInt()==0)
                {
                    tmpCP.SetValveOff();
                }
                else
                {
                    tmpCP.SetValveOn();
                }
                anIf(piLclDbWrkDbgEn, anAck("Control Valve: " << tmpCP.GetMsgStr()));
                emit controlValve(tmpCP);
            }  
            tmpQuery.exec("UPDATE stations SET setRoughValveOn=" +
                          currentQuery.value("Valve").toString() +
                          ",setHVOn=" + currentQuery.value("HV").toString() +
                          ",time=NOW() WHERE GlobalID=" + currentQuery.value("GlobalID").toString());
            if (isOnline)
            {
                tmpQuery.exec("SELECT setHVOn,setRoughValveOn,RFID FROM stations WHERE GlobalID=" + currentQuery.value("GlobalID").toString() + " LIMIT 1");
                tmpQuery.next();
                tmpStr = tmpQuery.value("RFID").toString();
                tmpQuery2.exec("SELECT * FROM " + tmpStr.toLower() + " ORDER BY Time DESC LIMIT 1");
                tmpQuery2.next();
                tmpStr5 = tmpQuery2.value("Time").toString();
                tmpData = QJsonDocument(createAJsonDataPackage(currentQuery.value("GlobalID").toString(),
                                                                 tmpQuery.value("setHVOn").toString(),
                                                                 tmpQuery.value("setRoughValveOn").toString(),
                                                                 tmpQuery2.value("Pressure").toString(),
                                                                 tmpQuery2.value("Voltage").toString(),
                                                                 tmpQuery2.value("Current").toString(),
                                                                 tmpStr,
                                                                 tmpStr5)).toJson(QJsonDocument::Compact);
                while (!sendJsonDataPackage(tmpData))
                {
                    qApp->processEvents();
                }
                if (isOnline)
                    tmpQuery.exec("UPDATE " + tmpStr.toLower() + " SET isSent=1 WHERE Time='" + tmpStr5 + "'");
            }
        }
        tmpQuery.exec("DELETE FROM control");
    }
    isAControlTaskDone = true;
}

void piLocalDBWorker::updatePVIData(quint32 GlobalID, const QString &Pressure, const QString &Voltage, const QString &Current)
{
    anIf(piLclDbWrkDbgEn, anTrk("GATE"));
    tmpStr1 = GlobalID;
    tmpStr2 = Pressure;
    tmpStr3 = Voltage;
    tmpStr4 = Current;
    currentQuery.exec("SELECT setHVOn,setRoughValveOn,RFID FROM stations WHERE GlobalID=" + tmpStr1 + " LIMIT 1");
    if (currentQuery.next())
    {
        tmpStr = currentQuery.value("RFID").toString();
        tmpQuery.prepare("INSERT INTO " + tmpStr.toLower() + " (Time,Pressure,Voltage,Current) "
                            "VALUES (NOW()," + tmpStr2 + "," + tmpStr3 + "," + tmpStr4 + ")");
        if (tmpQuery.exec())
        {
            if (isOnline)
            {
                tmpQuery2.exec("SELECT * FROM " + tmpStr.toLower() + " ORDER BY Time DESC LIMIT 1");
                tmpQuery2.next();
                tmpStr5 = tmpQuery2.value("Time").toString();
                tmpData = QJsonDocument(createAJsonDataPackage(tmpStr1,
                                                               currentQuery.value("setHVOn").toString(),
                                                               currentQuery.value("setRoughValveOn").toString(),
                                                               tmpStr2,
                                                               tmpStr3,
                                                               tmpStr4,
                                                               tmpStr,
                                                               tmpStr5)).toJson(QJsonDocument::Compact);
                while (!sendJsonDataPackage(tmpData))
                {
                    qApp->processEvents();
                }
                if (isOnline)
                    tmpQuery.exec("UPDATE " + tmpStr.toLower() + " SET isSent=1 WHERE Time='" + tmpStr5 + "'");
                if (isPendingJsonSendRequested)
                {
                    isPendingJsonSendRequested = false;
                    anIf(piLclDbWrkDbgEn, anInfo("Try Syncing Pending Offline Data ..."));
                    tmpData = QJsonDocument(getPendingJsonDataPackage()).toJson(QJsonDocument::Compact);
                    while (!sendJsonDataPackage(tmpData))
                    {
                        qApp->processEvents();
                    }
                    if (isOnline)
                    {
                        tmpQuery.exec("SELECT RFID FROM stations");
                        while (tmpQuery.next())
                        {
                            tmpQuery2.exec("UPDATE " + tmpQuery.value("RFID").toString().toLower() + " SET isSent=1");
                        }
                        anIf(piLclDbWrkDbgEn, anAck("OK Offline Data Synchronized"));
                    }
                }
            }
        }
        else
        {
            anIf(piLclDbWrkDbgEn, anWarn("Not Found Table " + tmpStr.toLower()));
        }
    }
}

void piLocalDBWorker::updateRFIDData(quint32 GlobalID, QString srcRFID)
{
    tmpStr = QString::number(GlobalID);
    tmpStr1 = "R" + srcRFID;
    currentQuery.exec("SELECT setHVOn,setRoughValveOn,RFID FROM stations WHERE GlobalID=" + tmpStr);
    if (currentQuery.next())
    {
        if (currentQuery.value("RFID").toString()!=tmpStr1)
        {
            anIf(piLclDbWrkDbgEn, anAck("RFID Changed At GlobalID=" + tmpStr));
            tmpQuery.exec("UPDATE stations SET RFID='" + tmpStr1 + "',time=NOW() WHERE GlobalID=" + tmpStr);
            tmpQuery.exec("SELECT * FROM rfids WHERE RFID='" + tmpStr1 + "'");
            if (!tmpQuery.next())
            {
                anIf(piLclDbWrkDbgEn, anInfo("New RFID"));
                if (tmpQuery.exec("INSERT INTO rfids (RFID) VALUES ('" + tmpStr1 + "')"))
                {
                    tmpQuery.exec("CREATE TABLE IF NOT EXISTS " + tmpStr1.toLower() + " (Time DATETIME NOT NULL,Pressure DOUBLE NULL,Voltage DOUBLE NULL,Current DOUBLE NULL,`isSent` tinyint(1) NOT NULL DEFAULT '0',PRIMARY KEY (Time),UNIQUE INDEX Time_UNIQUE (Time ASC)) ENGINE = InnoDB");
                    anIf(piLclDbWrkDbgEn, anAck("New RFID Inserted Into rfids Table"));
                    if (isOnline)
                    {
                        tmpQuery2.exec("SELECT * FROM " + tmpStr1.toLower() + " ORDER BY Time DESC LIMIT 1");
                        if (tmpQuery2.next())
                        {
                            tmpStr2 = tmpQuery2.value("Pressure").toString();
                            tmpStr3 = tmpQuery2.value("Voltage").toString();
                            tmpStr4 = tmpQuery2.value("Current").toString();
                            tmpStr5 = tmpQuery2.value("Time").toString();
                        }
                        else
                        {
                            tmpStr2 = "";
                            tmpStr3 = "";
                            tmpStr4 = "";
                            tmpStr5 = "";
                        }
                        tmpData = QJsonDocument(createAJsonDataPackage(tmpStr,
                                                                       currentQuery.value("setHVOn").toString(),
                                                                       currentQuery.value("setRoughValveOn").toString(),
                                                                       tmpStr2,
                                                                       tmpStr3,
                                                                       tmpStr4,
                                                                       tmpStr1,
                                                                       tmpStr5)).toJson(QJsonDocument::Compact);
                        while (!sendJsonDataPackage(tmpData))
                        {
                            qApp->processEvents();
                        }
                        if (isOnline && (tmpStr5 != ""))
                            tmpQuery.exec("UPDATE " + tmpStr1.toLower() + " SET isSent=1 WHERE Time='" + tmpStr5 + "'");
                    }
                }
            }
        }
    }
}

void piLocalDBWorker::FromSmallCoordinator(SmallCoordinatorDB::GlobalSignal *aSignal)
{
    anIf(piLclDbWrkDbgEn, anAck("An External Message Received !"));
    currentMsgEnum = QString(aSignal->Type.typeName());
    if ( currentMsgEnum == "piLocalDBWorkerDB::DataSet")
    {
        anIf(piLclDbWrkDbgEn, anAck("" + currentMsgEnum + " Parsed !"));
        switch (aSignal->Type.toInt()) {
        case piLocalDBWorkerDB::updatePVIData:
        {
            currentIdPVI = aSignal->Data.value<piLocalDBWorkerDB::IdPVI>();
            updatePVIData(currentIdPVI->GlobalID,currentIdPVI->Pressure,currentIdPVI->Voltage,currentIdPVI->Current);
            break;
        }
        case piLocalDBWorkerDB::updateRFIDData:
        {
            currentIdRFID = aSignal->Data.value<piLocalDBWorkerDB::IdRFID>();
            updateRFIDData(currentIdRFID->GlobalID,currentIdRFID->srcRFID);
            break;
        }
        default:
            break;
        }
    }
}

bool piLocalDBWorker::sendJsonDataPackage(QByteArray data)
{
    if (isJsonSendFinished)
    {
        delete reply;
        reply = NAManager->post(onlineDb, data);
        QObject::connect(reply, &QNetworkReply::finished, this, [&](){
            JsonSendTimer.stop();
            isJsonSendFinished = true;
            if (!isOnline)
                isPendingJsonSendRequested = true;
            isOnline = true;
            anIf(piLclDbWrkDbgEn, anAck("OK Json Package Transmitted !"));
        }, AutoUniqueConnection);
        JsonSendTimer.start();
        isJsonSendFinished = false;
        anIf(piLclDbWrkDbgEn, anInfo("Try Sending Json Package " << data));
        return true;
    }
    else
        return false;
}

const QJsonObject piLocalDBWorker::createAJsonDataPackage(const QString &GlobalID, const QString &HV, const QString &Valve, const QString &Pressure, const QString &Voltage, const QString &Current, const QString &dstRFID, const QString &Time)
{
    QJsonObject value
    {
        {"hv",HV,},
        {"valve",Valve,},
        {"pressure",Pressure,},
        {"current",Current,},
        {"voltage",Voltage,},
        {"rfid",dstRFID,},
        {"time",Time,}
    };
    QJsonObject pump
    {
        {GlobalID,value},
    };
    return pump;
}

const QJsonArray piLocalDBWorker::getPendingJsonDataPackage()
{
    QJsonArray tmp;
    currentQuery.exec("SELECT RFID, setRoughValveOn, setHVOn, GlobalID FROM stations");
    while (currentQuery.next())
    {
        tmpStr = currentQuery.value("RFID").toString();
        tmpQuery.exec("SELECT * FROM " + tmpStr.toLower() + "  WHERE isSent<>1 ORDER BY Time ASC");
        while (tmpQuery.next())
        {
            tmp << createAJsonDataPackage(currentQuery.value("GlobalID").toString(),
                                          currentQuery.value("setHVOn").toString(),
                                          currentQuery.value("setRoughValveOn").toString(),
                                          tmpQuery.value("Pressure").toString(),
                                          tmpQuery.value("Current").toString(),
                                          tmpQuery.value("Voltage").toString(),
                                          tmpStr,
                                          tmpQuery.value("Time").toString());
        }
    }
    return tmp;
}

const QNetworkRequest piLocalDBWorker::onlineDb = [](){
    QNetworkRequest tmp(QUrl("http://10.10.1.218:82/KTproj/update.php"));
    tmp.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    return tmp;
}();
