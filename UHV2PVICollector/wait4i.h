#ifndef WAIT4I_H
#define WAIT4I_H

#include <QState>
#include <QTimer>
#include "uhv2pvicollectordb.h"

class wait4I : public QState
{
public:
    wait4I(UHV2PVICollectorDB * database);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    QTimer timer;
    UHV2PVICollectorDB * dbPtr = Q_NULLPTR;
};
#endif // WAIT4I_H
