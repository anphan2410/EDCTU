#include "pilocaldbworkerdb.h"

piLocalDBWorkerDB::piLocalDBWorkerDB(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<piLocalDBWorkerDB::IdPVI>("piLocalDBWorkerDB::IdPVI");
    qRegisterMetaType<piLocalDBWorkerDB::IdRFID>("piLocalDBWorkerDB::IdRFID");

}
