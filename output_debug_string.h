#pragma once

#include <string>
#include <utility>
#include <vector>

#include <stdlib.h> // mbstowcs_s
#include <windows.h> // OutputDebugStringW

template <typename T>
std::wstring toDebugString(const T& val)
{
    return std::to_wstring(val);
}

std::wstring toDebugString(bool val);

std::wstring toDebugString(const wchar_t* s);

std::wstring toDebugString(const char* s);

std::wstring toDebugString(const std::string& s);

void outputDebugString(std::wstring s);

template <typename Arg0>
void outputDebugString(Arg0&& arg0) 
{
    outputDebugString(toDebugString(std::forward<Arg0>(arg0)));
}

template <typename Arg0, typename ...Args>
void outputDebugString(Arg0&& arg0, Args&&... args)
{
    outputDebugString(toDebugString(std::forward<Arg0>(arg0)), std::forward<Args>(args)...);
}

template <typename Arg0, typename ...Args>
void outputDebugString(std::wstring s, Arg0&& arg0, Args&&... args) 
{
    s += toDebugString(std::forward<Arg0>(arg0));
    outputDebugString(std::move(s), std::forward<Args>(args)...);
}

// internals

class outputDebugStringConstants
{
public:
    static const wchar_t* CLRF_STR() { return L"\r\n"; }
    static const wchar_t* NULL_STR() { return L"(null)"; }
    static const wchar_t* ERR_STR() { return L"(error)"; }
    static const wchar_t* TRUE_STR() { return L"true"; }
    static const wchar_t* FALSE_STR() { return L"false"; }
};

std::wstring toDebugString(const char* s, size_t n)
{
    std::vector<wchar_t> w(n + 1, L'\0');
    size_t ret = 0;
    if (0 != ::mbstowcs_s(&ret, w.data(), w.size(), s, n))
        return outputDebugStringConstants::ERR_STR();
    return w.data();
}

std::wstring toDebugString(const wchar_t* s)
{
    if (s == nullptr)
        return outputDebugStringConstants::NULL_STR();
    return std::wstring(s);
}

std::wstring toDebugString(const char* s)
{
    if (s == nullptr)
        return outputDebugStringConstants::NULL_STR();
    return toDebugString(s, strlen(s));
}

std::wstring toDebugString(const std::string& s)
{
    return toDebugString(s.c_str(), s.length());
}

std::wstring toDebugString(bool val)
{
    if (val)
        return outputDebugStringConstants::TRUE_STR();
    else
        return outputDebugStringConstants::FALSE_STR();
}

void outputDebugString(std::wstring s) 
{
    s += outputDebugStringConstants::CLRF_STR();
    ::OutputDebugStringW(s.c_str());
}
