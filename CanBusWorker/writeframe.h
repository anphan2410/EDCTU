#ifndef WRITEFRAME_H
#define WRITEFRAME_H

#include <QState>
#include "canbusworkerdb.h"

class writeFrame : public QState
{
public:
    writeFrame(CanBusWorkerDB * database);
protected:
    void onEntry(QEvent *) override;
private:
    CanBusWorkerDB * dbPtr = Q_NULLPTR;
};

#endif // WRITEFRAME_H
