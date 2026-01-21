/**
 * @file anotherCipher.cpp
 * @brief Реализация шифра перестановки по столбцам
 * @details 
 * Содержит реализацию функций шифрования, дешифрования и валидации
 * для шифра перестановки по столбцам.
 * 
 * @author BarabanovRuslan
 * @date 20.12.25
 * @version 1.0
 */

#include "anotherCipher.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cctype>

/**
 * @brief Валидирует ключ (количество столбцов)
 * @param columns Количество столбцов для проверки
 * @return Валидное количество столбцов
 * @throw cipher_error Если columns <= 0 или columns > 100
 * 
 * @details Проверяет, что количество столбцов находится в допустимом диапазоне.
 * Ограничение в 100 столбцов установлено для предотвращения создания
 * слишком больших таблиц и возможных проблем с производительностью.
 */
int getValidKey(int columns) {
    if (columns <= 0) {
        throw cipher_error("Ошибка: Количество столбцов должно быть положительным числом");
    }
    if (columns > 100) {
        throw cipher_error("Ошибка: Слишком большое количество столбцов (максимум 100)");
    }
    return columns;
}

/**
 * @brief Валидирует открытый текст
 * @param text Текст для проверки
 * @return Валидный открытый текст
 * @throw cipher_error Если текст пустой или не содержит печатных символов
 * 
 * @details Алгоритм проверки:
 * 1. Проверка на пустоту текста
 * 2. Поиск хотя бы одного непробельного печатного символа
 * 3. Возврат исходного текста (без изменений)
 * 
 * @note Функция не удаляет пробелы, так как они являются частью сообщения
 * и влияют на структуру таблицы при шифровании.
 */
std::string getValidOpenText(const std::string& text) {
    if (text.empty()) {
        throw cipher_error("Ошибка: Открытый текст не может быть пустым");
    }

    bool has_printable = false;
    for (char c : text) {
        if (std::isprint(c) && c != ' ') {
            has_printable = true;
            break;
        }
    }
    
    if (!has_printable) {
        throw cipher_error("Ошибка: Открытый текст должен содержать хотя бы один непробельный символ");
    }
    
    return text;
}

/**
 * @brief Валидирует зашифрованный текст
 * @param text Зашифрованный текст для проверки
 * @return Валидный зашифрованный текст
 * @throw cipher_error Если текст пустой или содержит непечатные символы
 * 
 * @details Проверяет, что все символы в зашифрованном тексте являются
 * печатными. Это требование необходимо для корректного заполнения
 * таблицы при дешифровании.
 */
std::string getValidCipherText(const std::string& text) {
    if (text.empty()) {
        throw cipher_error("Ошибка: Зашифрованный текст не может быть пустым");
    }
    
    for (char c : text) {
        if (!std::isprint(c)) {
            throw cipher_error("Ошибка: Зашифрованный текст содержит недопустимые символы");
        }
    }
    
    return text;
}

/**
 * @brief Шифрует текст методом перестановки по столбцам
 * @param plaintext Открытый текст для шифрования
 * @param columns Количество столбцов в таблице перестановки
 * @return Зашифрованный текст
 * @throw cipher_error Если параметры невалидны или произошла ошибка шифрования
 * 
 * @details Алгоритм шифрования:
 * 1. Валидация количества столбцов и открытого текста
 * 2. Расчет количества строк: rows = ceil(длина_текста / columns)
 * 3. Создание таблицы rows×columns, заполненной пробелами
 * 4. Запись текста в таблицу построчно
 * 5. Формирование шифротекста чтением таблицы снизу вверх по столбцам
 * 
 * @note Пример:
 * Текст: "HELLO WORLD", столбцов: 3
 * Таблица 4×3 (11 символов, 12 ячеек, последняя пустая):
 * H E L
 * L O  
 * W O R
 * L D
 * Чтение: столбец 2 снизу: D R   L
 *         столбец 1 снизу: L O O E
 *         столбец 0 снизу: W L H
 * Результат: "DRL LOOE WLH"
 */
std::string encrypt(const std::string& plaintext, int columns) {
    // Валидация входных параметров
    int valid_columns = getValidKey(columns);
    std::string valid_text = getValidOpenText(plaintext);
    
    // Расчет размеров таблицы
    int rows = (valid_text.length() + valid_columns - 1) / valid_columns;
    std::vector<std::vector<char>> table(rows, std::vector<char>(valid_columns, ' '));
    int k = 0;
    
    try {
        // Заполнение таблицы построчно
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < valid_columns; ++j) {
                if (k < valid_text.length()) {
                    table[i][j] = valid_text[k++];
                }
            }
        }

        // Формирование шифротекста чтением снизу вверх по столбцам
        std::string ciphertext = "";
        for (int j = valid_columns - 1; j >= 0; --j) {
            for (int i = 0; i < rows; ++i) {
                ciphertext += table[i][j];
            }
        }
        return ciphertext;
    } catch (const std::exception& e) {
        // Обработка и переброс исключения с контекстом
        throw cipher_error(std::string("Ошибка при шифровании: ") + e.what());
    }
}

/**
 * @brief Дешифрует текст, зашифрованный методом перестановки по столбцам
 * @param ciphertext Зашифрованный текст
 * @param columns Количество столбцов в таблице перестановки
 * @return Расшифрованный текст
 * @throw cipher_error Если параметры невалидны или произошла ошибка дешифрования
 * 
 * @details Алгоритм дешифрования:
 * 1. Валидация количества столбцов и зашифрованного текста
 * 2. Расчет количества строк: rows = ceil(длина_текста / columns)
 * 3. Проверка корректности длины текста (rows×columns == длина_текста)
 * 4. Создание таблицы rows×columns, заполненной пробелами
 * 5. Запись шифротекста в таблицу снизу вверх по столбцам
 * 6. Чтение таблицы построчно для восстановления исходного текста
 * 
 * @note Требование rows×columns == длина_текста необходимо потому,
 * что шифротекст должен полностью заполнять таблицу без лишних символов.
 */
std::string decrypt(const std::string& ciphertext, int columns) {
    // Валидация входных параметров
    int valid_columns = getValidKey(columns);
    std::string valid_text = getValidCipherText(ciphertext);
    
    // Расчет и проверка размеров таблицы
    int rows = (valid_text.length() + valid_columns - 1) / valid_columns;
    
    if (rows * valid_columns != valid_text.length()) {
        throw cipher_error("Ошибка: Некорректная длина зашифрованного текста для данного количества столбцов");
    }
    
    // Создание таблицы для дешифрования
    std::vector<std::vector<char>> table(rows, std::vector<char>(valid_columns, ' '));
    int k = 0;
    
    try {
        // Заполнение таблицы снизу вверх по столбцам
        for (int j = valid_columns - 1; j >= 0; --j) {
            for (int i = 0; i < rows; ++i) {
                table[i][j] = valid_text[k++];
            }
        }

        // Чтение таблицы построчно для восстановления текста
        std::string plaintext = "";
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < valid_columns; ++j) {
                plaintext += table[i][j];
            }
        }
        return plaintext;
    } catch (const std::exception& e) {
        // Обработка и переброс исключения с контекстом
        throw cipher_error(std::string("Ошибка при дешифровании: ") + e.what());
    }
}