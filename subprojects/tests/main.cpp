#include <QTest>
#include "serializatortests.h"
#include "mathtokenizertests.h"
#include "mathsyntaxtreetest.h"
int main()
{
    MathSyntaxTreeTest mstt;
    return QTest::qExec(&mstt);
}
