#include <QTest>
#include "serializatortests.h"
#include "mathtokenizertests.h"
#include "mathsyntaxtreetest.h"
#include "calculatortests.h"
int main()
{
    CalculatorTests ct;
    return QTest::qExec(&ct);
}
