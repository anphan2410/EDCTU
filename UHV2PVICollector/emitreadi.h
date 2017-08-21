#ifndef EMITREADI_H
#define EMITREADI_H

#include <QState>
#include <QTimer>
#include "uhv2pvicollectordb.h"

class emitReadI : public QState
{
public:
    emitReadI(UHV2PVICollectorDB * database);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    QTimer timer;
    UHV2PVICollectorDB * dbPtr = Q_NULLPTR;
};

#endif // EMITREADI_H
