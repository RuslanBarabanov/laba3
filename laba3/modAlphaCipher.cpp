/**
 * @file modAlphaCipher.cpp
 * @brief Реализация класса modAlphaCipher для шифрования методом Гронсфельда
 * @details 
 * Содержит реализацию методов класса modAlphaCipher, включая валидацию данных,
 * шифрование, дешифрование и вспомогательные функции.
 * 
 * @author BarabanovRuslan
 * @date 20.12.25
 * @version 1.0
 */

#include "modAlphaCipher.h"
#include <locale>
#include <codecvt>
#include <algorithm>

/**
 * @brief Валидация ключа шифрования
 * @param s Исходный ключ для проверки
 * @return Валидированный ключ в верхнем регистре
 * @throw cipher_error Если ключ пустой, содержит недопустимые символы или все символы одинаковые
 * 
 * @details Алгоритм валидации:
 * 1. Проверка на пустоту ключа
 * 2. Преобразование в верхний регистр
 * 3. Проверка всех символов на принадлежность русскому алфавиту
 * 4. Проверка на "слабый" ключ (все символы одинаковые)
 */
std::wstring modAlphaCipher::getValidKey(const std::wstring& s)
{
    if (s.empty()) {
        throw cipher_error("Пустой ключ");
    }
    
    std::wstring tmp(s);
    tmp = toUpperCase(tmp);
    
    for (auto & c:tmp) {
        if (numAlpha.find(c) == std::wstring::npos) {
            throw cipher_error("Недопустимый символ в ключе");
        }
    }
    
    bool all_same = true;
    for (size_t i = 1; i < tmp.size(); i++) {
        if (tmp[i] != tmp[0]) {
            all_same = false;
            break;
        }
    }
    if (all_same) {
        throw cipher_error("Слабый ключ: все символы одинаковые");
    }
    
    return tmp;
}

/**
 * @brief Валидация открытого текста
 * @param s Исходный текст для проверки
 * @return Очищенный текст (без неалфавитных символов, в верхнем регистре)
 * @throw cipher_error Если текст пустой или не содержит допустимых символов
 * 
 * @details Алгоритм валидации:
 * 1. Проверка на пустоту текста
 * 2. Преобразование в верхний регистр
 * 3. Фильтрация: сохраняются только символы русского алфавита (игнорируются пробелы и другие символы)
 * 4. Проверка, что остался хотя бы один символ
 */
std::wstring modAlphaCipher::getValidOpenText(const std::wstring& s)
{
    if (s.empty()) {
        throw cipher_error("Пустой открытый текст");
    }
    
    std::wstring tmp;
    std::wstring upperText = toUpperCase(s);
    
    for (auto c:upperText) {
        if (numAlpha.find(c) != std::wstring::npos && c != L' ') {
            tmp.push_back(c);
        }
    }
    
    if (tmp.empty()) {
        throw cipher_error("Открытый текст не содержит допустимых символов");
    }
    
    return tmp;
}

/**
 * @brief Валидация зашифрованного текста
 * @param s Зашифрованный текст для проверки
 * @return Текст в верхнем регистре
 * @throw cipher_error Если текст пустой или содержит недопустимые символы
 * 
 * @details Алгоритм валидации:
 * 1. Проверка на пустоту текста
 * 2. Преобразование в верхний регистр
 * 3. Проверка, что все символы принадлежат русскому алфавиту
 */
std::wstring modAlphaCipher::getValidCipherText(const std::wstring& s)
{
    if (s.empty()) {
        throw cipher_error("Пустой зашифрованный текст");
    }
    
    std::wstring tmp = toUpperCase(s);
    
    for (auto c:tmp) {
        if (numAlpha.find(c) == std::wstring::npos) {
            throw cipher_error("Недопустимый символ в зашифрованном тексте");
        }
    }
    
    return tmp;
}

/**
 * @brief Конструктор класса modAlphaCipher
 * @param skey Ключ шифрования
 * @throw cipher_error Если ключ невалиден
 * 
 * @details Инициализирует объект шифра:
 * 1. Создает карту соответствия символов алфавита их индексам
 * 2. Валидирует ключ с помощью getValidKey()
 * 3. Преобразует ключ в числовую форму с помощью convert()
 */
modAlphaCipher::modAlphaCipher(const std::wstring& skey)
{
    // Инициализация карты соответствия символов индексам
    for (unsigned i=0; i<numAlpha.size(); i++) {
        alphaNum[numAlpha[i]]=i;
    }
    
    // Валидация и преобразование ключа
    std::wstring valid_key = getValidKey(skey);
    key = convert(valid_key);
}

/**
 * @brief Шифрует открытый текст
 * @param open_text Текст для шифрования
 * @return Зашифрованный текст
 * @throw cipher_error Если текст невалиден или произошла ошибка при шифровании
 * 
 * @details Алгоритм шифрования:
 * 1. Валидация и очистка текста
 * 2. Преобразование символов в числовые индексы
 * 3. Циклическое применение ключа: (символ + ключ) mod размер_алфавита
 * 4. Обратное преобразование в строку
 */
std::wstring modAlphaCipher::encrypt(const std::wstring& open_text)
{
    try {
        // Валидация открытого текста
        std::wstring valid_text = getValidOpenText(open_text);
        
        // Преобразование текста в числовые индексы
        std::vector<int> work;
        for (auto c : valid_text) {
            work.push_back(alphaNum[c]);
        }
        
        // Применение ключа шифрования
        for(unsigned i=0; i < work.size(); i++) {
            work[i] = (work[i] + key[i % key.size()]) % numAlpha.size();
        }
        
        // Преобразование обратно в строку
        return convert(work);
        
    } catch (const std::exception& e) {
        // Обработка и переброс исключения с контекстом
        throw cipher_error(std::string("Ошибка при шифровании: ") + e.what());
    }
}

/**
 * @brief Дешифрует зашифрованный текст
 * @param cipher_text Текст для дешифрования
 * @return Расшифрованный текст
 * @throw cipher_error Если текст невалиден или произошла ошибка при дешифровании
 * 
 * @details Алгоритм дешифрования:
 * 1. Валидация зашифрованного текста
 * 2. Преобразование символов в числовые индексы
 * 3. Циклическое применение ключа: (символ + алфавит - ключ) mod размер_алфавита
 * 4. Обратное преобразование в строку
 */
std::wstring modAlphaCipher::decrypt(const std::wstring& cipher_text)
{
    try {
        // Валидация зашифрованного текста
        std::wstring valid_text = getValidCipherText(cipher_text);
        
        // Преобразование текста в числовые индексы
        std::vector<int> work = convert(valid_text);
        
        // Применение ключа для дешифрования
        for(unsigned i=0; i < work.size(); i++) {
            work[i] = (work[i] + numAlpha.size() - key[i % key.size()]) % numAlpha.size();
        }
        
        // Преобразование обратно в строку
        return convert(work);
        
    } catch (const std::exception& e) {
        // Обработка и переброс исключения с контекстом
        throw cipher_error(std::string("Ошибка при дешифровании: ") + e.what());
    }
}

/**
 * @brief Преобразует строку в вектор числовых индексов
 * @param s Входная строка (должна содержать только символы алфавита)
 * @return Вектор индексов символов в алфавите
 * 
 * @note Предполагается, что строка уже валидирована и содержит только символы алфавита
 */
std::vector<int> modAlphaCipher::convert(const std::wstring& s)
{
    std::vector<int> result;
    for(auto c:s) {
        result.push_back(alphaNum[c]);
    }
    return result;
}

/**
 * @brief Преобразует вектор числовых индексов в строку
 * @param v Вектор индексов символов
 * @return Строка, составленная из символов алфавита по заданным индексам
 * 
 * @note Индексы должны быть в диапазоне [0, размер_алфавита-1]
 */
std::wstring modAlphaCipher::convert(const std::vector<int>& v)
{
    std::wstring result;
    for(auto i:v) {
        result.push_back(numAlpha[i]);
    }
    return result;
}

/**
 * @brief Преобразует строку в верхний регистр
 * @param s Входная строка
 * @return Строка в верхнем регистре
 * 
 * @details Использует функцию std::towupper() для преобразования
 * каждого символа строки в верхний регистр.
 */
std::wstring modAlphaCipher::toUpperCase(const std::wstring& s)
{
    std::wstring result = s;
    for (auto& c : result) {
        c = std::towupper(c);
    }
    return result;
}