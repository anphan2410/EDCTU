#ifndef WAIT4I_H
#define WAIT4I_H

#include <QState>
#include <QTimer>
#include "uhv4pvicollectordb.h"

class wait4I : public QState
{
public:
    wait4I(UHV4PVICollectorDB * database);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    QTimer timer;
    UHV4PVICollectorDB * dbPtr = Q_NULLPTR;
};
#endif // WAIT4I_H
