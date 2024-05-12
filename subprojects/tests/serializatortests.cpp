#include "serializatortests.h"

void SerializatorTests::QSTRING_TEST()
{
    QString testString = "I'm test string";
    auto serializatorResult =
        Serializator::binaryDeserialize<QString>(
            Serializator::binarySerialize(testString));
    QString answerString = std::get<0>(serializatorResult);
    QCOMPARE(answerString, testString);
}

void SerializatorTests::QVECTOR_TEST()
{
    QVector<float> testVector = {10.43, 5, 95.10};
    auto serializatorResult =
        Serializator::binaryDeserialize<QVector<float>>(
            Serializator::binarySerialize(testVector));
    QVector<float> answerString = std::get<0>(serializatorResult);
    QCOMPARE(answerString, testVector);
}

void SerializatorTests::QLIST_TEST()
{
    QList<float> testList = {10.43, 5, 95.10};
    auto serializatorResult =
        Serializator::binaryDeserialize<QList<float>>(
            Serializator::binarySerialize(testList));
    QList<float> answerString = std::get<0>(serializatorResult);
    QCOMPARE(answerString, testList);
}

void SerializatorTests::EMPTY_CONYAINER_TEST()
{
    QList<float> testList = {};
    auto serializatorResult =
        Serializator::binaryDeserialize<QList<float>>(
            Serializator::binarySerialize(testList));
    QList<float> answerString = std::get<0>(serializatorResult);
    QCOMPARE(answerString, testList);
}

void SerializatorTests::MULTIPLE_CONTAINERS_TEST()
{
    QString string = "I'm test string";
    QVector<float> vec = {10.43, 5, 95.10};
    QList<quint32> list = {1,2,3, 10};

    auto serializatorResult =
        Serializator::binaryDeserialize<QString, QVector<float>, QList<quint32>>(
            Serializator::binarySerialize(string, vec, list));

    QCOMPARE(std::get<0>(serializatorResult), string);
    QCOMPARE(std::get<1>(serializatorResult), vec);
    QCOMPARE(std::get<2>(serializatorResult), list);
}

void SerializatorTests::SIMPLE_STRUCT_VECTOR_TEST()
{
    struct point{
        quint32 x;
        quint32 y;
        quint32 z;
        bool operator==(const point& other) const noexcept
        {return x==other.x && y==other.y && z==other.z;}
    };

    QVector<point> points = {{0, 0, 0},{1, 2, 3},{10, 15, 20}};
    auto serializatorResult =
        Serializator::binaryDeserialize<QVector<point>>(
            Serializator::binarySerialize(points));
    QCOMPARE(std::get<0>(serializatorResult), points);
}

void SerializatorTests::FUNDAMENTAL_TYPES_TEST()
{
    quint32 a = 10;
    quint64 b = 90;
    float c = 10.54;
    auto serializatorResult =
        Serializator::binaryDeserialize<quint32, quint64, float>(
        Serializator::binarySerialize(a, b, c));
    QCOMPARE(std::get<0>(serializatorResult), a);
    QCOMPARE(std::get<1>(serializatorResult), b);
    QCOMPARE(std::get<2>(serializatorResult), c);
}
