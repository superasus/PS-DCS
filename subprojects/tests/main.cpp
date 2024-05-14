#include <QTest>
#include "serializatortests.h"
#include "mathtokenizertests.h"
int main()
{
    SerializatorTests st;
    MathTokenizerTests mt;
    return QTest::qExec(&st) || QTest::qExec(&mt);
}
