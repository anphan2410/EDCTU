#ifndef EMITREADV_H
#define EMITREADV_H

#include <QState>
#include <QTimer>
#include "uhv4pvicollectordb.h"

class emitReadV : public QState
{
public:
    emitReadV(UHV4PVICollectorDB * database);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    QTimer timer;
    UHV4PVICollectorDB * dbPtr = Q_NULLPTR;
};

#endif // EMITREADV_H
