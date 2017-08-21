#ifndef EMITREADP_H
#define EMITREADP_H

#include <QState>
#include <QTimer>
#include "uhv2pvicollectordb.h"

class emitReadP : public QState
{
public:
    emitReadP(UHV2PVICollectorDB * database);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    QTimer timer;
    UHV2PVICollectorDB * dbPtr = Q_NULLPTR;
};

#endif // EMITREADP_H
