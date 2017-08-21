#include "smallcoordinatordb.h"

SmallCoordinatorDB::SmallCoordinatorDB(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<SmallCoordinatorDB::GlobalSignal>("SmallCoordinatorDB::GlobalSignal");
}
