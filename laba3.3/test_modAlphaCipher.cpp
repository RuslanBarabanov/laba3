#include <UnitTest++/UnitTest++.h>
#include "anotherCipher.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <algorithm>

// Вспомогательная функция для обрезки пробелов в конце
std::string rtrim(const std::string& s) {
    size_t end = s.find_last_not_of(' ');
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

SUITE(KeyValidationTest)
{
    TEST(ValidKey) {
        CHECK_EQUAL(5, getValidKey(5));
        CHECK_EQUAL(1, getValidKey(1));
        CHECK_EQUAL(100, getValidKey(100));
    }
    
    TEST(InvalidKey) {
        CHECK_THROW(getValidKey(0), cipher_error);
        CHECK_THROW(getValidKey(-5), cipher_error);
        CHECK_THROW(getValidKey(101), cipher_error);
    }
}

SUITE(TextValidationTest)
{
    TEST(ValidOpenText) {
        CHECK_EQUAL("Hello", getValidOpenText("Hello"));
        CHECK_EQUAL("Test 123", getValidOpenText("Test 123"));
    }
    
    TEST(InvalidOpenText) {
        CHECK_THROW(getValidOpenText(""), cipher_error);
        CHECK_THROW(getValidOpenText("   "), cipher_error);
    }
    
    TEST(ValidCipherText) {
        CHECK_EQUAL("Hello", getValidCipherText("Hello"));
        CHECK_EQUAL("!@#$", getValidCipherText("!@#$"));
    }
    
    TEST(InvalidCipherText) {
        CHECK_THROW(getValidCipherText(""), cipher_error);
    }
}

SUITE(EncryptionTest)
{
    TEST(BasicEncryption) {
        CHECK_EQUAL("ETTS", encrypt("TEST", 2));
        CHECK_EQUAL("EL HLO", encrypt("HELLO", 2));
    }
    
    TEST(EncryptionWithDifferentColumns) {
        CHECK_EQUAL("S E TT", encrypt("TEST", 3));
        CHECK_EQUAL("L EOHL", encrypt("HELLO", 3));
    }
    
    TEST(EncryptionWithSpaces) {
        CHECK_EQUAL("L R EOODHLWL", encrypt("HELLO WORLD", 3));
    }
    
    TEST(EncryptionSpecialCharacters) {
        CHECK_EQUAL("2!#13@", encrypt("123!@#", 2));
    }
    
    TEST(EncryptionShortText) {
        CHECK_EQUAL("A", encrypt("A", 1));
        CHECK_EQUAL("BA", encrypt("AB", 2));
    }
    
    TEST(EncryptionInvalidCases) {
        CHECK_THROW(encrypt("", 5), cipher_error);
        CHECK_THROW(encrypt("Hello", 0), cipher_error);
        CHECK_THROW(encrypt("   ", 3), cipher_error);
    }
}

SUITE(DecryptionTest)
{
    TEST(BasicDecryption) {
        std::string encrypted = encrypt("TEST", 2);
        CHECK_EQUAL("TEST", rtrim(decrypt(encrypted, 2)));
        
        encrypted = encrypt("HELLO", 2);
        CHECK_EQUAL("HELLO", rtrim(decrypt(encrypted, 2)));
    }
    
    TEST(DecryptionWithDifferentColumns) {
        std::string encrypted = encrypt("TEST", 3);
        CHECK_EQUAL("TEST", rtrim(decrypt(encrypted, 3)));
        
        encrypted = encrypt("HELLO", 3);
        CHECK_EQUAL("HELLO", rtrim(decrypt(encrypted, 3)));
    }
    
    TEST(DecryptionWithSpaces) {
        std::string encrypted = encrypt("HELLO WORLD", 3);
        CHECK_EQUAL("HELLO WORLD", rtrim(decrypt(encrypted, 3)));
    }
    
    TEST(DecryptionSpecialCharacters) {
        std::string encrypted = encrypt("123!@#", 2);
        CHECK_EQUAL("123!@#", rtrim(decrypt(encrypted, 2)));
    }
    
    TEST(DecryptionShortText) {
        std::string encrypted = encrypt("AB", 2);
        CHECK_EQUAL("AB", rtrim(decrypt(encrypted, 2)));
    }
    
    TEST(DecryptionInvalidCases) {
        CHECK_THROW(decrypt("", 5), cipher_error);
        CHECK_THROW(decrypt("Hello", 0), cipher_error);
        CHECK_THROW(decrypt("ABCD", 3), cipher_error);
    }
}

SUITE(ConsistencyTest)
{
    TEST(ConsistencyShortText) {
        std::string text = "ABCD";
        std::string encrypted = encrypt(text, 2);
        std::string decrypted = decrypt(encrypted, 2);
        CHECK_EQUAL(text, rtrim(decrypted));
    }
    
    TEST(ConsistencyMediumText) {
        std::string text = "HELLO WORLD";
        std::string encrypted = encrypt(text, 4);
        std::string decrypted = decrypt(encrypted, 4);
        CHECK_EQUAL(text, rtrim(decrypted));
    }
    
    TEST(ConsistencyLongText) {
        std::string text = "THE QUICK BROWN FOX";
        std::string encrypted = encrypt(text, 5);
        std::string decrypted = decrypt(encrypted, 5);
        CHECK_EQUAL(text, rtrim(decrypted));
    }
    
    TEST(ConsistencySpecialText) {
        std::string text = "123!@# ABC";
        std::string encrypted = encrypt(text, 3);
        std::string decrypted = decrypt(encrypted, 3);
        CHECK_EQUAL(text, rtrim(decrypted));
    }
    
    TEST(ConsistencyDifferentColumns) {
        std::string text = "TESTING";
        for (int columns = 1; columns <= 5; ++columns) {
            std::string encrypted = encrypt(text, columns);
            std::string decrypted = decrypt(encrypted, columns);
            CHECK_EQUAL(text, rtrim(decrypted));
        }
    }
}

int main()
{
    std::cout << "Running Columnar Transposition Cipher Tests..." << std::endl;
    return UnitTest::RunAllTests();
}
