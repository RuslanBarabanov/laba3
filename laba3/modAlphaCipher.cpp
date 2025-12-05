#include "modAlphaCipher.h"
#include <locale>
#include <codecvt>
#include <algorithm>

// Валидация ключа
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

// Валидация открытого текста
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

// Валидация зашифрованного текста
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

modAlphaCipher::modAlphaCipher(const std::wstring& skey)
{
    // Инициализация алфавита
    for (unsigned i=0; i<numAlpha.size(); i++) {
        alphaNum[numAlpha[i]]=i;
    }
    
    // Валидация ключа
    std::wstring valid_key = getValidKey(skey);
    key = convert(valid_key);
}

std::wstring modAlphaCipher::encrypt(const std::wstring& open_text)
{
    try {
        // Валидация открытого текста
        std::wstring valid_text = getValidOpenText(open_text);
        
        std::vector<int> work;
        for (auto c : valid_text) {
            work.push_back(alphaNum[c]);
        }
        
        for(unsigned i=0; i < work.size(); i++) {
            work[i] = (work[i] + key[i % key.size()]) % numAlpha.size();
        }
        return convert(work);
        
    } catch (const std::exception& e) {
        throw cipher_error(std::string("Ошибка при шифровании: ") + e.what());
    }
}

std::wstring modAlphaCipher::decrypt(const std::wstring& cipher_text)
{
    try {
        // Валидация зашифрованного текста
        std::wstring valid_text = getValidCipherText(cipher_text);
        
        std::vector<int> work = convert(valid_text);
        for(unsigned i=0; i < work.size(); i++) {
            work[i] = (work[i] + numAlpha.size() - key[i % key.size()]) % numAlpha.size();
        }
        return convert(work);
        
    } catch (const std::exception& e) {
        throw cipher_error(std::string("Ошибка при дешифровании: ") + e.what());
    }
}

std::vector<int> modAlphaCipher::convert(const std::wstring& s)
{
    std::vector<int> result;
    for(auto c:s) {
        result.push_back(alphaNum[c]);
    }
    return result;
}

std::wstring modAlphaCipher::convert(const std::vector<int>& v)
{
    std::wstring result;
    for(auto i:v) {
        result.push_back(numAlpha[i]);
    }
    return result;
}

std::wstring modAlphaCipher::toUpperCase(const std::wstring& s)
{
    std::wstring result = s;
    for (auto& c : result) {
        c = std::towupper(c);
    }
    return result;
}