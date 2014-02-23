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

std::vector<std::wstring> vec_s2ws(std::vector<std::string> const &str);
std::wstring s2ws(std::string const &str);
std::vector<std::string> vec_ws2s(std::vector<std::wstring> const &wstr);
std::string ws2s(std::wstring const &wstr);

} // namespace esa

#endif // ESAPP_CONV_HPP_
