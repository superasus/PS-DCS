#include <QTest>
#include "serializatortests.h"
#include "mathtokenizertests.h"
#include "mathsyntaxtreetest.h"
#include "calculatortests.h"
#include "discoveryservicetests.h"
int main()
{
    SerializatorTests st;
    CalculatorTests ct;
    MathSyntaxTreeTest mstt;
    MathTokenizerTests mtt;
    DiscoveryServiceTests dst;
    return QTest::qExec(&ct) || QTest::qExec(&mstt) || QTest::qExec(&mtt) || QTest::qExec(&st) || QTest::qExec(&dst);
}
