#include <QTest>
#include "serializatortests.h"
#include "mathtokenizertests.h"
int main()
{
    MathTokenizerTests tt;
    return QTest::qExec(&tt);
}
