#include <algorithm>
#include <codecvt>
#include <locale>
#include <string>
#include <vector>

namespace esapp
{

std::vector<std::wstring> vec_s2ws(std::vector<std::string> const &str);
std::wstring s2ws(std::string const &str);
std::vector<std::string> vec_ws2s(std::vector<std::wstring> const &wstr);
std::string ws2s(std::wstring const &wstr);

} // namespace esa
