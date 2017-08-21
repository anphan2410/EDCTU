#ifndef EMITREADI_H
#define EMITREADI_H

#include <QState>
#include <QTimer>
#include "uhv4pvicollectordb.h"

class emitReadI : public QState
{
public:
    emitReadI(UHV4PVICollectorDB * database);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    QTimer timer;
    UHV4PVICollectorDB * dbPtr = Q_NULLPTR;
};

#endif // EMITREADI_H
