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
#include <type_traits>
#include <vector>

namespace esapp
{

template <typename T2, typename T1>
inline std::vector<T2> transform(std::vector<T1> const &vec,
                                 T2(*fptr)(T1 const &))
{
    decltype(transform(vec, fptr)) result;
    result.reserve(vec.size());
    std::transform(vec.begin(), vec.end(), std::back_inserter(result), fptr);
    return result;
}

inline std::wstring s2ws(std::string const &str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return conv.from_bytes(str);
}

inline std::vector<std::wstring> s2ws(std::vector<std::string> const &vec)
{
    typedef decltype(s2ws(vec))::value_type OutType;
    typedef std::decay<decltype(vec)>::type::value_type InType;
    return transform<OutType, InType>(vec, &s2ws);
}

inline std::string ws2s(std::wstring const &wstr)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(wstr);
}

inline std::vector<std::string> ws2s(std::vector<std::wstring> const &ws_vec)
{
    typedef decltype(ws2s(ws_vec))::value_type OutType;
    typedef std::decay<decltype(ws_vec)>::type::value_type InType;
    return transform<OutType, InType>(ws_vec, &ws2s);
}

} // namespace esapp

#endif // ESAPP_CONV_HPP_
