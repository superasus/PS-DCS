#include "discoveryservicetests.h"
#include "../main/discoveryService.h"

void DiscoveryServiceTests::WORK_TEST()
{
    DiscoveryService* ds = new DiscoveryService(45454);
    DiscoveryService* ds1 = new DiscoveryService(45455);
    ds->findNodes(45455);
    qint64 ans = 0;
    // TODO: Нормальная реализация
    QTimer::singleShot(500, [ds, ans](){ QCOMPARE(ds->getNodeCount(), 1); });
    QTimer::singleShot(1000, [ds, ds1](){ delete ds; delete ds1; });
}
