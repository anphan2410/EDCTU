#ifndef WAIT4P_H
#define WAIT4P_H

#include <QState>
#include <QTimer>
#include "uhv4pvicollectordb.h"

class wait4P : public QState
{
public:
    wait4P(UHV4PVICollectorDB * database);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    QTimer timer;
};

#endif // WAIT4P_H
