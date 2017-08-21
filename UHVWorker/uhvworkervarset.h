#ifndef UHVWORKERVARSET_H
#define UHVWORKERVARSET_H

#define UHVWorkerVarSetDbgEn 1

#include <QCoreApplication>
#include <QObject>
#include <QPair>
#include <QThread>
#include <QMap>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QStateMachine>
#include <QMetaType>
#include "anlogger.h"

class UHVWorkerVarSet: public QObject
{
    Q_OBJECT
public:
    explicit UHVWorkerVarSet(QObject * parent = Q_NULLPTR);
    ~UHVWorkerVarSet();
    enum MessageTopic {
        Nothing = 0,
        ANewPortName,
        AnUHVPrioritizedCommandMessage,
        SerialPortError,
        SerialPortDisconnect,
        SerialPortConnect,
        SerialPortRestart,
        MessageSendTimedOut,
        MessageReadTimedOut,
        PendingMessageListClear
    };
    Q_ENUM(MessageTopic)
    typedef QPair<QByteArray*,QString*> CommandMessage;
    typedef QPair<qint8,CommandMessage*> PrioritizedCommandMessage;

    QString * PortName = Q_NULLPTR;
    QSerialPort * SerialPort = Q_NULLPTR;
    QMap<qint8,QList<CommandMessage*>*> * PendingMessageList = Q_NULLPTR;
    qint8 lastTransmittedMessagePriority = 0;
    CommandMessage * lastTransmittedMessage = Q_NULLPTR;
    CommandMessage * lastReceivedMessage = Q_NULLPTR;
    void configSerialPort();
    void initialize();
    MessageTopic ErrorStatus = Nothing;
signals:
    void DirectStateTransitionRequest(const QString &);
    void PortNameChanged();
    void RestartSerialPortConnection();
    void AFirstPrioritizedCommandMessageReceived();
    void Out(QVariant *, QVariant * = Q_NULLPTR);
};
Q_DECLARE_METATYPE(UHVWorkerVarSet::PrioritizedCommandMessage)
#endif // UHVWORKERVARSET_H
