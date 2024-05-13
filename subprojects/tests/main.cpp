#include <QTest>
#include "serializatortests.h"
#include "discoveryservicetests.h"
int main()
{
    DiscoveryServiceTests st;
    return QTest::qExec(&st);
}
