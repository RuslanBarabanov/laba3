#include "UnitTest++.h"
#include "modAlphaCipher.h"

SUITE(KeyTest)
{
    TEST(ValidKey) {
        CHECK_EQUAL(L"ПРИВЕТ", modAlphaCipher(L"КЛЮЧ").decrypt(
            modAlphaCipher(L"КЛЮЧ").encrypt(L"ПРИВЕТ")));
    }
    TEST(LongKey) {
        CHECK_EQUAL(L"ТЕКСТ", modAlphaCipher(L"ДЛИННЫЙКЛЮЧ").decrypt(
            modAlphaCipher(L"ДЛИННЫЙКЛЮЧ").encrypt(L"ТЕКСТ")));
    }
    TEST(LowCaseKey) {
        CHECK_EQUAL(L"ПРИВЕТ", modAlphaCipher(L"ключ").decrypt(
            modAlphaCipher(L"ключ").encrypt(L"ПРИВЕТ")));
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
        CHECK_EQUAL(cipher.encrypt(L"ПРИВЕТМИР"), cipher.encrypt(L"ПРИВЕТМИР"));
    }
    TEST(LowCaseString) {
        modAlphaCipher cipher(L"КЛЮЧ");
        CHECK_EQUAL(cipher.encrypt(L"ПРИВЕТМИР"), cipher.encrypt(L"приветмир"));
    }
    TEST(StringWithWhitespaceAndPunct) {
        modAlphaCipher cipher(L"КЛЮЧ");
        CHECK_EQUAL(cipher.encrypt(L"ПРИВЕТМИР"), cipher.encrypt(L"ПРИВЕТ, МИР!"));
    }
    TEST(StringWithNumbers) {
        modAlphaCipher cipher(L"КЛЮЧ");
        CHECK_EQUAL(cipher.encrypt(L"СНОВЫМГОДОМ"), cipher.encrypt(L"С НОВЫМ 2024 ГОДОМ"));
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
        modAlphaCipher cipher(L"Я");
        CHECK_EQUAL(L"ЯАБ", cipher.encrypt(L"АБВ"));
    }
}

SUITE(DecryptTest)
{
    TEST(ValidCipherText) {
        modAlphaCipher cipher(L"КЛЮЧ");
        std::wstring encrypted = cipher.encrypt(L"ПРИВЕТМИР");
        CHECK_EQUAL(L"ПРИВЕТМИР", cipher.decrypt(encrypted));
    }
    TEST(LowCaseInCipherText) {
        modAlphaCipher cipher(L"КЛЮЧ");
        CHECK_THROW(cipher.decrypt(L"прИВЕТ"), cipher_error);
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
        modAlphaCipher cipher(L"Я");
        std::wstring encrypted = cipher.encrypt(L"АБВ");
        CHECK_EQUAL(L"АБВ", cipher.decrypt(encrypted));
    }
}

int main()
{
    return UnitTest::RunAllTests();
}