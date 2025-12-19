#include <UnitTest++/UnitTest++.h>
#include "modAlphaCipher.h"
#include <iostream>
#include <locale>
#include <codecvt>
#include <string>

// Функция для конвертации wstring в string
std::string ws2s(const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

// Оператор для вывода wstring в ostream
std::ostream& operator<<(std::ostream& os, const std::wstring& ws) {
    return os << ws2s(ws);
}

SUITE(KeyTest)
{
    TEST(ValidKey) {
        std::locale::global(std::locale("ru_RU.UTF-8"));
        modAlphaCipher cipher(L"КЛЮЧ");
        CHECK_EQUAL(L"ПРИВЕТ", cipher.decrypt(cipher.encrypt(L"ПРИВЕТ")));
    }
    
    TEST(LongKey) {
        modAlphaCipher cipher(L"ДЛИННЫЙКЛЮЧ");
        CHECK_EQUAL(L"ТЕКСТ", cipher.decrypt(cipher.encrypt(L"ТЕКСТ")));
    }
    
    TEST(LowCaseKey) {
        modAlphaCipher cipher(L"ключ");
        CHECK_EQUAL(L"ПРИВЕТ", cipher.decrypt(cipher.encrypt(L"ПРИВЕТ")));
    }
    
    TEST(DigitsInKey) {
        CHECK_THROW(modAlphaCipher cipher(L"КЛЮЧ123"), cipher_error);
    }
    
    TEST(PunctuationInKey) {
        CHECK_THROW(modAlphaCipher cipher(L"КЛЮЧ!"), cipher_error);
    }
    
    TEST(WhitespaceInKey) {
        CHECK_THROW(modAlphaCipher cipher(L"КЛЮ Ч"), cipher_error);
    }
    
    TEST(EmptyKey) {
        CHECK_THROW(modAlphaCipher cipher(L""), cipher_error);
    }
    
    TEST(WeakKey) {
        CHECK_THROW(modAlphaCipher cipher(L"ААА"), cipher_error);
    }
}

SUITE(EncryptTest)
{
    TEST(UpCaseString) {
        modAlphaCipher cipher(L"КЛЮЧ");
        std::wstring encrypted = cipher.encrypt(L"ПРИВЕТМИР");
        // Проверяем, что шифрование работает
        CHECK(!encrypted.empty());
    }
    
    TEST(LowCaseString) {
        modAlphaCipher cipher(L"КЛЮЧ");
        std::wstring encrypted1 = cipher.encrypt(L"ПРИВЕТМИР");
        std::wstring encrypted2 = cipher.encrypt(L"приветмир");
        CHECK_EQUAL(encrypted1, encrypted2);
    }
    
    TEST(StringWithWhitespaceAndPunct) {
        modAlphaCipher cipher(L"КЛЮЧ");
        std::wstring encrypted1 = cipher.encrypt(L"ПРИВЕТМИР");
        std::wstring encrypted2 = cipher.encrypt(L"ПРИВЕТ, МИР!");
        CHECK_EQUAL(encrypted1, encrypted2);
    }
    
    TEST(StringWithNumbers) {
        modAlphaCipher cipher(L"КЛЮЧ");
        std::wstring encrypted1 = cipher.encrypt(L"СНОВЫМГОДОМ");
        std::wstring encrypted2 = cipher.encrypt(L"С НОВЫМ 2024 ГОДОМ");
        CHECK_EQUAL(encrypted1, encrypted2);
    }
    
    TEST(EmptyString) {
        modAlphaCipher cipher(L"КЛЮЧ");
        CHECK_THROW(cipher.encrypt(L""), cipher_error);
    }
    
    TEST(NoAlphaString) {
        modAlphaCipher cipher(L"КЛЮЧ");
        CHECK_THROW(cipher.encrypt(L"123 !@#"), cipher_error);
    }
    
    TEST(MaxShiftKey) {
        modAlphaCipher cipher(L"ЯБВ");
        std::wstring encrypted = cipher.encrypt(L"АБВ");
    }
}

SUITE(DecryptTest)
{
    TEST(ValidCipherText) {
        modAlphaCipher cipher(L"КЛЮЧ");
        std::wstring encrypted = cipher.encrypt(L"ПРИВЕТМИР");
        CHECK_EQUAL(L"ПРИВЕТМИР", cipher.decrypt(encrypted));
    }
    
    TEST(WhitespaceInCipherText) {
        modAlphaCipher cipher(L"КЛЮЧ");
        CHECK_THROW(cipher.decrypt(L"ПРИ ВЕТ"), cipher_error);
    }
    
    TEST(DigitsInCipherText) {
        modAlphaCipher cipher(L"КЛЮЧ");
        CHECK_THROW(cipher.decrypt(L"ПРИ2024"), cipher_error);
    }
    
    TEST(PunctuationInCipherText) {
        modAlphaCipher cipher(L"КЛЮЧ");
        CHECK_THROW(cipher.decrypt(L"ПРИВЕТ!"), cipher_error);
    }
    
    TEST(EmptyCipherText) {
        modAlphaCipher cipher(L"КЛЮЧ");
        CHECK_THROW(cipher.decrypt(L""), cipher_error);
    }
    
    TEST(MaxShiftKeyDecrypt) {
        modAlphaCipher cipher(L"ЯБВ");
        std::wstring encrypted = cipher.encrypt(L"АБВ");
        CHECK_EQUAL(L"АБВ", cipher.decrypt(encrypted));
    }
}

int main()
{
    // Устанавливаем локаль для работы с русскими символами
    std::locale::global(std::locale("ru_RU.UTF-8"));
    std::wcout.imbue(std::locale("ru_RU.UTF-8"));
    
    return UnitTest::RunAllTests();
}
