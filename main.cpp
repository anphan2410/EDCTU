#include <QCoreApplication>
#include "UHVWorker/uhvworker.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    UHVWorker test;
    test.start();
    return a.exec();
}
