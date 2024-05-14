#ifndef SERIALIZATOR_H
#define SERIALIZATOR_H
#include <QString>
#include <QByteArray>
#include <tuple>
#include <type_traits>
namespace Serializator
{
namespace __service{

    template <typename ... >
    using to_void = void;

    template<typename T, typename = void>
    struct is_container : public std::false_type {};

    template <typename T>
    struct is_container<T,
        to_void<
            decltype(std::declval<T>().begin()),
            decltype(std::declval<T>().end()),
            typename T::value_type>> : public std::true_type {};

    template <typename T>
    void binarySerializeElement(
        const T& element,
        QByteArray& answer,
        typename std::enable_if_t<!is_container<T>::value, void**> = nullptr)
    {
        const quint8* buf = reinterpret_cast<const quint8*>(&element);
        for (quint32 i = 0; i < sizeof(element); i++) answer.append(buf[i]);
    }

    template <typename T>
    void binarySerializeElement(
        const T& container,
        QByteArray& answer,
        typename std::enable_if_t<is_container<T>::value, void**> = nullptr)
    {
        quint32 containerSize = container.size();
        quint32 containerElementSize = sizeof(typename T::value_type);
        binarySerializeElement(containerSize, answer);
        answer.append(containerElementSize);
        for (auto& el : container) binarySerializeElement(el, answer);
    }

    template <typename T>
    T binaryDeserializeElement(
        const QByteArray& bytes,
        quint32& byteIndex,
        std::enable_if_t<!is_container<T>::value, void**> = nullptr)
    {
        quint8* buf = new quint8[sizeof(T)];
        for (quint32 j = 0; j < sizeof(T); ++j, ++byteIndex) buf[j] = bytes[byteIndex];
        T answer = *(reinterpret_cast<T*>(buf));
        delete[] buf;
        return answer;
    }

    template <typename T>
    T binaryDeserializeElement(
        const QByteArray& bytes,
        quint32& byteIndex,
        quint8* allocatedBuf)
    {
        for (quint32 j = 0; j < sizeof(T); ++j, ++byteIndex) allocatedBuf[j] = bytes[byteIndex];
        T answer = *(reinterpret_cast<T*>(allocatedBuf));
        return answer;
    }

    template <typename T>
    T binaryDeserializeElement(
        const QByteArray& bytes,
        quint32& byteIndex,
        std::enable_if_t<is_container<T>::value, void**> = nullptr)
    {
        T answer;
        quint32 containerSize = binaryDeserializeElement<quint32>(bytes, byteIndex);
        quint32 elementSize = bytes[byteIndex++];

        if (elementSize != sizeof(typename T::value_type))
        {
            QString errorMessage("Can't serialize bytes to");
            errorMessage.append(typeid(T).name());
            throw std::invalid_argument(errorMessage.toStdString());
        }

        if constexpr (is_container<typename T::value_type>::value)
        {
            for (quint32 i = 0; i < containerSize; ++i)
                answer.append(binaryDeserializeElement<typename T::value_type>(bytes, byteIndex));
        }
        else
        {
            quint8* buf = new quint8[sizeof(typename T::value_type)];
            for (quint32 i = 0; i < containerSize; ++i)
            {
                answer.append(binaryDeserializeElement<typename T::value_type>(bytes, byteIndex, buf));
            }
            delete[] buf;
        }
        return answer;
    }

    template<
        quint32 Lim,
        quint32 I = 0,
        typename Tuple,
        typename Element = std::tuple_element_t <I, Tuple>>

    void binaryDeserializeElementsProcess(Tuple& tuple, const QByteArray& bytes, quint32& byteIndex)
    {
        std::get<I>(tuple) = binaryDeserializeElement<Element>(bytes, byteIndex);
        if constexpr (I + 1 != Lim)
            binaryDeserializeElementsProcess<Lim, I + 1>(tuple, bytes, byteIndex);
    }
}

/*!
 * \brief Выпоняет двоичную сериализацию пакета переданных элементов.
 * Пользовательские типы данных не поддерживаются, за исключением POD!
 * \param <el> - первый элемент для сериализации
 * \param <elements> - пакет последующих элементов для сериализации (Может быть пустым)
 * \return Возвращает массив байтов, содержащий сериализованные данные в том порядке,
 * в котором они передавались в пакете
 */

template <typename Element, typename ... OtherElements>
QByteArray binarySerialize(const Element& el, const OtherElements& ... elements)
{
    QByteArray answer;
    __service::binarySerializeElement(el, answer);
    if constexpr (sizeof...(OtherElements))
    {
        answer.append(binarySerialize(elements...));
    }
    return answer;
}

/*!
 * \brief Десерилиализует массив байтов в элементы, тип которых соответсвует переданному пакету
 * типов в том порядке, в котором они идут в пакете
 * \tparam <...Types> - Пакет выходных типов, в которые будет производиться десериализация
 * \param <bytes> - Массив байтов для десерилизации
 * \return Возвращает кортеж, содержащий запрошенные в пакете типов элементы в том порядке,
 * в котором они были запрошены в пакете
 */
template <typename ... Types>
std::tuple<Types...> binaryDeserialize(const QByteArray& bytes)
{
    std::tuple<Types...> answer;
    quint32 byteIndex = 0;
    __service::binaryDeserializeElementsProcess<sizeof...(Types)>(answer, bytes, byteIndex);
    return answer;
}

};



#endif // !SERIALIZATOR_H


