#ifndef EMITREADV_H
#define EMITREADV_H

#include <QState>
#include <QTimer>
#include "uhv2pvicollectordb.h"

class emitReadV : public QState
{
public:
    emitReadV(UHV2PVICollectorDB * database);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    QTimer timer;
    UHV2PVICollectorDB * dbPtr = Q_NULLPTR;
};

#endif // EMITREADV_H
