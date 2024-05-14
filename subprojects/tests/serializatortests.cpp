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

void SerializatorTests::MIXED_TYPES_TEST()
{
    quint32 a = 10;
    quint64 b = 90;
    QVector<float> vec = {10.43, 5, 95.10};
    QList<quint32> list = {1,2,3, 10};
    auto serializatorResult =
        Serializator::binaryDeserialize<quint32, quint64, QVector<float>, QList<quint32>>(
            Serializator::binarySerialize(a, b, vec, list));
    QCOMPARE(std::get<0>(serializatorResult), a);
    QCOMPARE(std::get<1>(serializatorResult), b);
    QCOMPARE(std::get<2>(serializatorResult), vec);
    QCOMPARE(std::get<3>(serializatorResult), list);
}

void SerializatorTests::MIXED_TYPES_TEST_2()
{
    struct point{
        quint32 x;
        quint32 y;
        quint32 z;
        bool operator==(const point& other) const noexcept
        {return x==other.x && y==other.y && z==other.z;}
    };

    QVector<point> points = {{0, 0, 0},{1, 2, 3},{10, 15, 20}};
    quint32 a = 10;
    quint64 b = 90;
    point p = {15, 42, 1388};
    QString string = "I'm very cool";
    QByteArray bytes = "Cool bytes";

    auto serializatorResult =
        Serializator::binaryDeserialize<QVector<point>, quint32, quint64, point, QString, QByteArray>(
            Serializator::binarySerialize(points, a, b, p, string, bytes));

    QCOMPARE(std::get<0>(serializatorResult), points);
    QCOMPARE(std::get<1>(serializatorResult), a);
    QCOMPARE(std::get<2>(serializatorResult), b);
    QCOMPARE(std::get<3>(serializatorResult), p);
    QCOMPARE(std::get<4>(serializatorResult), string);
    QCOMPARE(std::get<5>(serializatorResult), bytes);
}

void SerializatorTests::CONTAINER_WITH_CONTAINERS_TEST()
{
    QVector<QVector<float>> vec =
        {
            {10.43, 5, 95.10},
            {3.44, 123.123},
            {45.455, 23.23, 12355, 10, 10, 12}
        };
    auto serializatorResult =
        Serializator::binaryDeserialize<QVector<QVector<float>>>(
        Serializator::binarySerialize(vec));

    QCOMPARE(std::get<0>(serializatorResult), vec);
}
