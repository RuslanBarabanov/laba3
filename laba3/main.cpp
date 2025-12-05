#include "modAlphaCipher.h"
#include <iostream>
#include <locale>
#include <algorithm>

void check(const std::wstring& Text, const std::wstring& key, bool testErrors = false)
{
    try {
        std::wstring cipherText;
        std::wstring decryptedText;
        modAlphaCipher cipher(key);
        cipherText = cipher.encrypt(Text);

        decryptedText = cipher.decrypt(cipherText);
        
        std::wcout << L"key=" << key << std::endl;
        std::wcout << L"Original: " << Text << std::endl;
        std::wcout << L"Encrypt: " << cipherText << std::endl;
        std::wcout << L"Decrypt: " << decryptedText << std::endl;

        std::wstring textWithoutSpaces = Text;
        textWithoutSpaces.erase(std::remove(textWithoutSpaces.begin(), textWithoutSpaces.end(), ' '), textWithoutSpaces.end());
        
        for (auto& c : textWithoutSpaces) {
            c = std::towupper(c);
        }

        if(decryptedText == textWithoutSpaces)
            std::wcout << L"Ok\n";
        else
            std::wcout << L"Err\n";
            
    } catch (const cipher_error& e) {
        std::wcout << L"Ошибка шифрования:Неправильные символы в ключе";
        std::cout << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::wcout << L"Неожиданная ошибка: ";
        std::cout << e.what() << std::endl;
    }
    std::wcout << L" " << std::endl;
}

int main()
{
    std::locale::global(std::locale(""));
    std::wcout.imbue(std::locale());

    std::wcout << L"ТЕСТИРОВАНИЕ ШИФРА ГРОНСФЕЛЬДА С ИСКЛЮЧЕНИЯМИ" << std::endl;
    
    // Позитивные тесты
    //std::wcout << L"\n1. Позитивные тесты:" << std::endl;
    //check(L"во все", L"тяжкие");
    //check(L"фылдовыдлао", L"отлично");

    // Тесты с ошибками
    std::wcout << L"\n2. Тест:" << std::endl;
    //check(L"", L"КЛЮЧ");                    // Пустой текст
    //check(L"ПРИВЕТ", L"");                  // Пустой ключ
    //check(L"Hello123", L"КЛЮЧ");            // Неправильные символы в тексте
    check(L"ПРИВЕТ", L"123");               // Неправильные символы в ключе
    //check(L"ПРИВЕТ", L"ААА");               // Вырожденный ключ

    return 0;
}