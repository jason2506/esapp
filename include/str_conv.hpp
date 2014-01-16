#ifndef STR_CONV_HPP_INCLUDEED
#define STR_CONV_HPP_INCLUDEED

#include <string>
#include <vector>

namespace esa
{

std::wstring s2ws(const std::string &str)
{
    std::setlocale(LC_CTYPE, "");

    size_t const length = mbstowcs(NULL, str.c_str(), 0);
    std::vector<wchar_t> buffer(length + 1);
    mbstowcs(&buffer[0], str.c_str(), str.size());

    return std::wstring(buffer.begin(), buffer.end() - 1);
}

std::string ws2s(const std::wstring &wstr)
{
    std::setlocale(LC_CTYPE, "");

    size_t const length = wcstombs(NULL, wstr.c_str(), 0);
    std::vector<char> buffer(length + 1);
    wcstombs(&buffer[0], wstr.c_str(), wstr.size());

    return std::string(buffer.begin(), buffer.end() - 1);
}

} // namespace esa

#endif // STR_CONV_HPP_INCLUDEED
