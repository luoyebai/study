#ifndef MYFMT_HPP_
#define MYFMT_HPP_

#include <iostream>
#include <sstream>
#include <string>

namespace my {

template <class... Args>
constexpr void PrintReplace(std::string &str, Args &&...args) noexcept {
    int pos = 0;
    auto F = [&pos, &str](auto arg) {
        std::string strArg;
        std::stringstream strTmp;
        strTmp << arg;
        strTmp >> strArg;
        pos = str.find("{}", pos);
        if (pos == -1)
            return;
        str.replace(pos, 2, strArg);
        pos += strArg.length() - 1;
        return;
    };
    (F(args), ...);
    return;
}
auto operator""_f(const char *str, size_t size) noexcept {

    return [str, size](auto... args) {
        std::string strResult = std::string(str, size);
        PrintReplace(strResult, args...);
        std::cout << strResult;
        return strResult;
    };
}

} // namespace my
#endif
