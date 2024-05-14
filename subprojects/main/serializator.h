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
    /*!
     * \brief Выполняет двоичную сериализацию одного элемента
     * \details Элемент должен быть фундаментальным типом или POD структурой.
     * Сложные типы (классы, управляющие данными в куче, другие контейнеры и т.д) не поддерживаются
     * \param <element> - элемент для сериализации
     * \param <answer> - ранее созданный QByteArray, куда будет помещён результат сериализации
     */
    template <typename T>
    void binarySerializeElement(
        const T& element,
        QByteArray& answer,
        typename std::enable_if_t<!is_container<T>::value, void**> = nullptr)
    {
        const quint8* buf = reinterpret_cast<const quint8*>(&element);
        for (quint32 i = 0; i < sizeof(element); i++) answer.append(buf[i]);
    }

    /*!
     * \brief Выполняет двоичную сериализацию одного контейнера
     * \details Контейнер должен содержать фундаментальные языковые типы или POD структуры.
     * Контейнеры, содержащие сложные типы (классы, управляющие данными в куче, другие контейнеры и т.д)
     * не поддерживаются
     * \param <container> - контейнер для сериализации
     * \param <answer> - ранее созданный QByteArray, куда будет помещён результат сериализации
     */

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

    /*!
     * \brief Выполняет двоичную десериализацию одиночного элемента
     * \details Элемент должен быть фундаментальным типом или POD структурой.
     * Сложные типы (классы, управляющие данными в куче, другие контейнеры и т.д) не поддерживаются
     * \tparam <T> - выходной элемент
     * \param <bytes> - набор байтов для десериализации, содержащий десериализуемый элемент
     * \param <byteIndex> - Индекс первого байта десериализуемого элемента
     * \return элемент типа T, полученный в результате десериализации
     */
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
    /*!
     * \brief Выполняет двоичную десериализацию одиночного элемента
     * \details Элемент должен быть фундаментальным типом или POD структурой.
     * Сложные типы (классы, управляющие данными в куче, другие контейнеры и т.д) не поддерживаются
     * \tparam <T> - выходной элемент
     * \param <bytes> - набор байтов для десериализации, содержащий десериализуемый элемент
     * \param <byteIndex> - Индекс первого байта десериализуемого элемента
     * \param <allocatedBuf> - Аллоцированный буфер размера sizeof(T), в котором можно работать.
     * По завершению функции память не освобождается
     * \return элемент типа T, полученный в результате десериализации
     */

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

    /*!
     * \brief Выполняет двоичную десериализацию одного контейнера
     * \details Контейнер должен содержать фундаментальные языковые типы или POD структуры.
     * Контейнеры, содержащие сложные типы (классы, управляющие данными в куче, другие контейнеры и т.д)
     * не поддерживаются
     * \tparam <T> - выходной котейнер
     * \param <bytes> - набор байтов для десериализации, содержащий QT контейнер
     * \param <byteIndex> - Индекс первого байта десериализуемого контейнера
     * \return контейнер T, полученный в результате десериализации
     */
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

    /*!
     * \brief запрашивает двоичную десериализацию каждого контейнера в отдельности в том порядке,
     * в котором они идут в выходном кортеже и добавляет результат в него
     * \details Контейнеры должны содержать фундаментальные языковые типы или POD структуры.
     * Контейнеры, содержащие сложные типы (классы, управляющие данными в куче, другие контейнеры и т.д)
     * не поддерживаются
     * \param <tuple> - выходной кортеж
     * \param <bytes> - набор байтов для десериализации, содержащий QT контейнеры
     * \param <byteIndex> - Индекс первого байта десериализуемого контейнера
     */

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


