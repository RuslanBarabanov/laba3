#include <iostream>
#include <string>
#include "anotherCipher.h"

int getSafeIntegerInput(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Ошибка: Введите корректное целое число!" << std::endl;
        } else {
            std::cin.ignore(10000, '\n');
            return value;
        }
    }
}

std::string getSafeTextInput(const std::string& prompt) {
    std::string text;
    std::cout << prompt;
    std::getline(std::cin, text);
    return text;
}

void handleEncryption() {
    try {
        std::string text = getSafeTextInput("Введите текст для шифрования: ");
        int num_cols = getSafeIntegerInput("Введите количество столбцов: ");
        
        std::string ciphertext = encrypt(text, num_cols);
        std::cout << "Зашифрованный текст: " << ciphertext << std::endl;
        std::cout << "Успешно зашифровано!" << std::endl;
        
    } catch (const cipher_error& e) {
        std::cerr << "Ошибка шифрования: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Неожиданная ошибка: " << e.what() << std::endl;
    }
}

// Функция для обработки дешифрования с исключениями
void handleDecryption() {
    try {
        std::string text = getSafeTextInput("Введите текст для дешифрования: ");
        int num_cols = getSafeIntegerInput("Введите количество столбцов: ");
        
        std::string decrypted_text = decrypt(text, num_cols);
        std::cout << "Расшифрованный текст: " << decrypted_text << std::endl;
        std::cout << "Успешно расшифровано!" << std::endl;
        
    } catch (const cipher_error& e) {
        std::cerr << "Ошибка дешифрования: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Неожиданная ошибка: " << e.what() << std::endl;
    }
}

int main() {
    int choice;
    
    std::cout << "Добро пожаловать в программу шифрования табличной маршрутной перестановкой!" << std::endl;
    std::cout << "С ИСКЛЮЧЕНИЯМИ" << std::endl;

    do {
        std::cout << "\nВыберите действие:" << std::endl;
        std::cout << "1. Шифрование текста" << std::endl;
        std::cout << "2. Дешифрование текста" << std::endl;
        std::cout << "0. Выход" << std::endl;
        std::cout << "Ваш выбор: ";
        std::cin >> choice;
        std::cin.ignore(); 

        switch (choice) {
            case 1: {
                handleEncryption();
                break;
            }
            case 2: {
                handleDecryption();
                break;
            }

            case 0: {
                // Выход
                std::cout << "До свидания!" << std::endl;
                break;
            }
            default: {
                // Некорректный ввод
                std::cout << "Некорректный выбор. Пожалуйста, попробуйте еще раз." << std::endl;
            }
        }
    } while (choice != 0);

    return 0;
}