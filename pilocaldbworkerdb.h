#ifndef PILOCALDBWORKERDB_H
#define PILOCALDBWORKERDB_H

#include <QObject>


class piLocalDBWorkerDB : public QObject
{
    Q_OBJECT
public:
    explicit piLocalDBWorkerDB(QObject *parent = nullptr);
    enum DataGet
    {
        NoDataGet = 0
    };
    Q_ENUM(DataGet)

    enum DataSet
    {
        NoDataSet = 0,
        updatePVIData,
        updateRFIDData
    };
    Q_ENUM(DataSet)

    enum Error
    {
        NoError = 0
    };
    Q_ENUM(Error)

    enum Warning
    {

    };
    Q_ENUM(Warning)

    enum Notification
    {

    };
    Q_ENUM(Notification)

    typedef struct
    {
        quint32 GlobalID;
        QString Pressure;
        QString Voltage;
        QString Current;
    } IdPVI;
    typedef struct
    {
        quint32 GlobalID;
        QString srcRFID;
    } IdRFID;
signals:

public slots:
};
Q_DECLARE_METATYPE(piLocalDBWorkerDB::IdPVI)
Q_DECLARE_METATYPE(piLocalDBWorkerDB::IdRFID)
#endif // PILOCALDBWORKERDB_H
