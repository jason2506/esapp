/************************************************
 *  conv.hpp
 *  ESA++
 *
 *  Copyright (c) 2014, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#ifndef ESAPP_CONV_HPP_
#define ESAPP_CONV_HPP_

#include <codecvt>
#include <locale>
#include <algorithm>
#include <string>
#include <vector>

namespace esapp
{

inline std::wstring s2ws(std::string const &str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return conv.from_bytes(str);
}

inline std::vector<std::wstring> vec_s2ws(std::vector<std::string> const &vec)
{
    decltype(vec_s2ws(vec)) ws_vec(vec.size());
    std::transform(vec.begin(), vec.end(), ws_vec.begin(), &s2ws);
    return ws_vec;
}

inline std::string ws2s(std::wstring const &wstr)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(wstr);
}

inline std::vector<std::string> vec_ws2s(std::vector<std::wstring> const &ws_vec)
{
    decltype(vec_ws2s(ws_vec)) vec(ws_vec.size());
    std::transform(ws_vec.begin(), ws_vec.end(), vec.begin(), &ws2s);
    return vec;
}

} // namespace esa

#endif // ESAPP_CONV_HPP_
