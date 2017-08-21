QT += core serialport serialbus sql network
QT -= gui

CONFIG += c++11

TARGET = EDCTU
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    binaryprotocol.cpp \
    canprotocol.cpp \
    windowcommand.cpp \
    UHVWorker/errorannouncement.cpp \
    UHVWorker/messagereceiveandemitout.cpp \
    UHVWorker/serialportconnectionestablishment.cpp \
    UHVWorker/serialportinforequest.cpp \
    UHVWorker/serialportinfovalidation.cpp \
    UHVWorker/solitarymessagetransmission.cpp \
    UHVWorker/uhvworker.cpp \
    UHVWorker/uhvworkerdirectstatetransition.cpp \
    UHVWorker/uhvworkervarset.cpp \
    smallcoordinator.cpp \
    CanBusWorker/canbusworker.cpp \
    CanBusWorker/canbusworkerdb.cpp \
    CanBusWorker/connectdevice.cpp \
    CanBusWorker/createdevice.cpp \
    CanBusWorker/deviceconnected.cpp \
    CanBusWorker/directtransition.cpp \
    CanBusWorker/framesent.cpp \
    CanBusWorker/readframe.cpp \
    CanBusWorker/waitforerrorhandler.cpp \
    CanBusWorker/waitforpluginandinterface.cpp \
    CanBusWorker/writeframe.cpp \
    pilocaldbworker.cpp \
    smallcoordinatordb.cpp \
    pilocaldbworkerdb.cpp \
    UHV2PVICollector/directtransition.cpp \
    UHV2PVICollector/emitreadi.cpp \
    UHV2PVICollector/emitreadp.cpp \
    UHV2PVICollector/emitreadv.cpp \
    UHV2PVICollector/idle.cpp \
    UHV2PVICollector/uhv2pvicollector.cpp \
    UHV2PVICollector/uhv2pvicollectordb.cpp \
    UHV2PVICollector/wait4i.cpp \
    UHV2PVICollector/wait4p.cpp \
    UHV2PVICollector/wait4v.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    anlogger.h \
    binaryprotocol.h \
    canprotocol.h \
    windowcommand.h \
    UHVWorker/errorannouncement.h \
    UHVWorker/messagereceiveandemitout.h \
    UHVWorker/serialportconnectionestablishment.h \
    UHVWorker/serialportinforequest.h \
    UHVWorker/serialportinfovalidation.h \
    UHVWorker/solitarymessagetransmission.h \
    UHVWorker/uhvworker.h \
    UHVWorker/uhvworkerdirectstatetransition.h \
    UHVWorker/uhvworkervarset.h \
    smallcoordinator.h \
    commonthings.h \
    CanBusWorker/canbusworker.h \
    CanBusWorker/canbusworkerdb.h \
    CanBusWorker/connectdevice.h \
    CanBusWorker/createdevice.h \
    CanBusWorker/deviceconnected.h \
    CanBusWorker/directtransition.h \
    CanBusWorker/framesent.h \
    CanBusWorker/readframe.h \
    CanBusWorker/waitforerrorhandler.h \
    CanBusWorker/waitforpluginandinterface.h \
    CanBusWorker/writeframe.h \
    pilocaldbworker.h \
    smallcoordinatordb.h \
    pilocaldbworkerdb.h \
    UHV2PVICollector/directtransition.h \
    UHV2PVICollector/emitreadi.h \
    UHV2PVICollector/emitreadp.h \
    UHV2PVICollector/emitreadv.h \
    UHV2PVICollector/idle.h \
    UHV2PVICollector/uhv2pvicollector.h \
    UHV2PVICollector/uhv2pvicollectordb.h \
    UHV2PVICollector/wait4i.h \
    UHV2PVICollector/wait4p.h \
    UHV2PVICollector/wait4v.h
