#pragma once

#include <string>
#include <utility>
#include <vector>

#include <stdlib.h> // mbstowcs_s
#include <string.h> // strlen

#include <windows.h> // OutputDebugStringW

void platformOutputDebugString(const wchar_t* s)
{
    OutputDebugStringW(s);
}

namespace outputDebugStringConstants
{
constexpr wchar_t* const CLRF_STR = L"\r\n";
constexpr wchar_t* const NULL_STR = L"(null)";
constexpr wchar_t* const ERR_STR = L"(error)";
constexpr wchar_t* const TRUE_STR = L"true";
constexpr wchar_t* const FALSE_STR = L"false";
};

template <typename T>
std::wstring toDebugString(T&& val)
{
    return std::to_wstring(std::forward<T>(val));
}

std::wstring toDebugString(bool val)
{
    if (val)
        return outputDebugStringConstants::TRUE_STR;
    else
        return outputDebugStringConstants::FALSE_STR;
}

std::wstring toDebugString(const wchar_t* s)
{
    if (s == nullptr)
        return outputDebugStringConstants::NULL_STR;
    return std::wstring(s);
}

std::wstring toDebugString(const char* s, size_t n)
{
    std::vector<wchar_t> w(n + 1, L'\0');
    size_t ret = 0;
    if (0 != mbstowcs_s(&ret, w.data(), w.size(), s, n))
        return outputDebugStringConstants::ERR_STR;
    return w.data();
}

std::wstring toDebugString(const char* s)
{
    if (s == nullptr)
        return outputDebugStringConstants::NULL_STR;
    return toDebugString(s, strlen(s));
}

std::wstring toDebugString(const std::string& s)
{
    return toDebugString(s.c_str(), s.length());
}

void outputDebugString(std::wstring s) 
{
    s += outputDebugStringConstants::CLRF_STR;
    platformOutputDebugString(s.c_str());
}

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
