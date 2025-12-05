#include "UnitTest++.h"
#include "RouteCipher.h"

SUITE(KeyTest)
{
    TEST(ValidKey) {
        CHECK_EQUAL(L"ПРИВЕТ", RouteCipher(3, 2).decrypt(
            RouteCipher(3, 2).encrypt(L"ПРИВЕТ")));
    }
    TEST(InvalidKey) {
        CHECK_THROW(RouteCipher cipher(0, 0), cipher_error);
        CHECK_THROW(RouteCipher cipher(-1, 2), cipher_error);
        CHECK_THROW(RouteCipher cipher(2, -1), cipher_error);
    }
}

SUITE(EncryptTest)
{
    TEST(SimpleEncrypt) {
        RouteCipher cipher(3, 2);
        CHECK_EQUAL(L"ПВТРЕИ", cipher.encrypt(L"ПРИВЕТ"));
    }
    TEST(DifferentDimensions) {
        RouteCipher cipher(2, 3);
        CHECK_EQUAL(L"ПРВТЕИ", cipher.encrypt(L"ПРИВЕТ"));
    }
    TEST(StringWithSpaces) {
        RouteCipher cipher(3, 2);
        CHECK_EQUAL(L"ПВТРЕИ", cipher.encrypt(L"ПРИВЕТ "));
    }
    TEST(EmptyString) {
        RouteCipher cipher(3, 2);
        CHECK_THROW(cipher.encrypt(L""), cipher_error);
    }
    TEST(LargeGrid) {
        RouteCipher cipher(10, 10);
        CHECK_EQUAL(L"ПРИВЕТ", cipher.encrypt(L"ПРИВЕТ"));
    }
    TEST(NumbersAndSymbols) {
        RouteCipher cipher(3, 2);
        CHECK_THROW(cipher.encrypt(L"ПРИВЕТ123"), cipher_error);
    }
}

SUITE(DecryptTest)
{
    TEST(SimpleDecrypt) {
        RouteCipher cipher(3, 2);
        std::wstring encrypted = cipher.encrypt(L"ПРИВЕТ");
        CHECK_EQUAL(L"ПРИВЕТ", cipher.decrypt(encrypted));
    }
    TEST(InvalidCipherText) {
        RouteCipher cipher(3, 2);
        CHECK_THROW(cipher.decrypt(L"ПРИ ВЕТ"), cipher_error);
    }
    TEST(InvalidCipherText2) {
        RouteCipher cipher(3, 2);
        CHECK_THROW(cipher.decrypt(L"123456"), cipher_error);
    }
    TEST(EmptyCipher) {
        RouteCipher cipher(3, 2);
        CHECK_THROW(cipher.decrypt(L""), cipher_error);
    }
}

int main()
{
    return UnitTest::RunAllTests();
}