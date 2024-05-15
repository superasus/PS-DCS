#include <QTest>
#include "serializatortests.h"
#include "mathtokenizertests.h"
#include "mathsyntaxtreetest.h"
#include "calculatortests.h"
int main()
{
    CalculatorTests ct;
    MathSyntaxTreeTest mstt;
    return QTest::qExec(&ct) || QTest::qExec(&mstt);
}
