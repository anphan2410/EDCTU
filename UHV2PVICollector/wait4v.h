#ifndef WAIT4V_H
#define WAIT4V_H

#include <QState>
#include <QTimer>
#include "uhv2pvicollectordb.h"

class wait4V : public QState
{
public:
    wait4V(UHV2PVICollectorDB * database);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    QTimer timer;
};

#endif // WAIT4V_H
